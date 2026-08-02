#include "GameScene.h"
#include "MapChipField.h"
#include "new.h"
#include"DeathParticles.h"
#include <cassert>
#include <cmath>
#include"TitleScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {
	blockModel_ = Model::CreateFromOBJ("block", true);
	textureHandle_ = TextureManager::Load("./Resources/block/block.png");
	textureHandle2_ = TextureManager::Load("./Resources/SkyDome/sky_sphere.png");
	texturePlayer_ = TextureManager::Load("./Resources/player/player.png");
	textureEnemy_ = TextureManager::Load("./Resources/enemy/enemy.png");
	textureDeathParticle_ = TextureManager::Load("./Resources/deathParticle/white1x1.png");
	textureTitleScene_ = TextureManager::Load("./Resources/titleFont/white1x1.png");
	//titleScene_ = new TitleScene();
	//titleScene_->SetTextureHandle(textureTitleScene_);
	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("./Resources/mapchip.csv");
	model_ = Model::Create();
	camera_.Initialize();
	// playerの生成/初期化
	playerModel_ = Model::CreateFromOBJ("player", true);
	player_ = new Player();
	KamataEngine::Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 18);
	player_->Initialize(playerModel_, &camera_, playerPosition);
	player_->SetTextureHandle(texturePlayer_);
	player_->SetMapChipField(mapChipField_);
	// tetileScene_の初期化
	//TitleModel_ = Model::CreateFromOBJ("title", true);

	// 敵モデルの読み込み
	enemyModel_ = Model::CreateFromOBJ("enemy", true);

	// 敵の複数生成
	for (int32_t i = 0; i < kNumEnemies; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(4 + i * 3, 18);
		newEnemy->Initialize(enemyModel_, &camera_, enemyPosition);
		newEnemy->SetTextureHandle(textureEnemy_);
		newEnemy->SetMapChipField(mapChipField_);
		enemies_.push_back(newEnemy);
	}
	// デスパーティクル用モデル・テクスチャの読み込み（発生自体は死亡時に行う）
	modelDeathParticle_ = Model::CreateFromOBJ("deathParticle", true);
	textureDeathParticle_ = TextureManager::Load("./Resources/deathParticle/white1x1.png");

	// 天球モデルの読み込み
	modelSkydome = Model::CreateFromOBJ("skydome", true);

	// 天球の生成と初期化
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome, textureHandle2_, &camera_);

	// カメラコントローラーの生成と初期化
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();
	cameraController_->SetMovableArea({0.0f, 50.0f, 0.0f, 50.0f});
	player_->SetCamera(cameraController_->GetCameraPtr());

	// 敵にもカメラを設定
	for (Enemy* enemy : enemies_) {
		enemy->SetCamera(cameraController_->GetCameraPtr());
	}

	phase_ = Phase::kPlay;

	// ブロック生成（関数化）
	GenerateBlocks();

	debugCamera_ = new DebugCamera(1280, 720);
}
void GameScene::CheckAllCollisions() {

	{
		AABB aabb1, aabb2;

		// 自キャラの座標
		aabb1 = player_->GetAABB();

		// 自キャラと全ての敵の当たり判定
		for (Enemy* enemy : enemies_) {
			aabb2 = enemy->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabb1, aabb2)) {
				// 自キャラの衝突時コールバックを呼び出す
				player_->OnCollision(enemy);
				// 敵の衝突時コールバックを呼び出す
				enemy->OnCollision(player_);
			}
		}
	}

}
// --- シーンの更新 ---
void GameScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_A)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {
		camera_.TransferMatrix();
	}

	switch (phase_) {
	case Phase::kPlay:
		// 天球の更新
		skydome_->Update();

		// ブロックの更新
		for (size_t i = 0; i < worldTransformBlocks_.size(); ++i) {
			for (size_t j = 0; j < worldTransformBlocks_[i].size(); ++j) {
				if (worldTransformBlocks_[i][j] != nullptr) {
					UpdateWorldTransform(*worldTransformBlocks_[i][j]);
				}
			}
		}

		// 自キャラの更新
		player_->Update();

		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// カメラコントローラーの更新
		cameraController_->Update();

		// 全ての当たり判定
		CheckAllCollisions();
		break;

	case Phase::kDeath:
		// 天球の更新
		skydome_->Update();

		// ブロックの更新
		for (size_t i = 0; i < worldTransformBlocks_.size(); ++i) {
			for (size_t j = 0; j < worldTransformBlocks_[i].size(); ++j) {
				if (worldTransformBlocks_[i][j] != nullptr) {
					UpdateWorldTransform(*worldTransformBlocks_[i][j]);
				}
			}
		}

		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// デスパーティクルの更新
		if (deathParticles_) {
			deathParticles_->Update();
		}
		break;
	}

	// フェーズの切り替え要求
	ChangePhase();
}

// --- シーンの描画 ---
void GameScene::Draw() {
	Model::PreDraw();
	skydome_->Draw();

	for (size_t i = 0; i < worldTransformBlocks_.size(); ++i) {
		for (size_t j = 0; j < worldTransformBlocks_[i].size(); ++j) {
			if (worldTransformBlocks_[i][j] != nullptr) {
				blockModel_->Draw(*worldTransformBlocks_[i][j], cameraController_->GetCamera(), textureHandle_);
			}
		}
	}

	// 自キャラの描画（デス演出中は非表示）
	if (phase_ == Phase::kPlay) {
		player_->Draw();
	}

	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	// パーティクル
	if (deathParticles_) {
		deathParticles_->Draw();
	}

	Model::PostDraw();
}

// --- ブロックの生成 ---
void GameScene::GenerateBlocks() {
	const uint32_t numBlockVertical = mapChipField_->GetNumBlockVertical();
	const uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVertical);

	for (uint32_t i = 0; i < numBlockVertical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);

		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			MapChipType chipType = mapChipField_->GetMapChipTypeByIndex(j, i);
			if (chipType == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->Initialize();

				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			} else {
				worldTransformBlocks_[i][j] = nullptr;
			}
		}
	}
}

void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kPlay:
		// 自キャラがデス状態
		if (player_->IsDead()) {
			// デス演出フェーズに切り替え
			phase_ = Phase::kDeath;
			// 自キャラの座標を取得
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();
			// 自キャラの座標にデスパーティクルを発生、初期化
			deathParticles_ = new DeathParticles();
			deathParticles_->Initialize(modelDeathParticle_, &camera_, deathParticlesPosition);
			deathParticles_->SetTextureHandle(textureDeathParticle_);
		}
		break;

	case Phase::kDeath:
		// デスパーティクルが有効で、かつパーティクルの演出が終了したら
		// ゲームシーンの終了フラグを立てる
		if (deathParticles_ && deathParticles_->IsFinished()) {
			finished_ = true;
		}
		break;
	}
}

// --- デストラクタ ---
GameScene::~GameScene() {
	if (blockModel_ != nullptr) {
		delete blockModel_;
	}

	for (size_t i = 0; i < worldTransformBlocks_.size(); ++i) {
		for (size_t j = 0; j < worldTransformBlocks_[i].size(); ++j) {
			if (worldTransformBlocks_[i][j] != nullptr) {
				delete worldTransformBlocks_[i][j];
			}
		}
		worldTransformBlocks_[i].clear();
	}
	worldTransformBlocks_.clear();

	delete model_;
	model_ = nullptr;
	delete player_;
	player_ = nullptr;
	delete skydome_;
	skydome_ = nullptr;
	delete modelSkydome;
	modelSkydome = nullptr;
	delete playerModel_;
	playerModel_ = nullptr;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	enemies_.clear();
	delete debugCamera_;
	debugCamera_ = nullptr;
	delete mapChipField_;
	mapChipField_ = nullptr;
	delete cameraController_;
	cameraController_ = nullptr;

	if (deathParticles_) {
		delete deathParticles_;
		deathParticles_ = nullptr;
	}
	delete modelDeathParticle_;
	modelDeathParticle_ = nullptr;
}