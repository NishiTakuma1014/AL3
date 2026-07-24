#include "GameScene.h"
#include "MapChipField.h"
#include "new.h"
#include <cassert>
#include <cmath>

using namespace KamataEngine;

// --- シーンの初期化 ---
void GameScene::Initialize() {
	blockModel_ = Model::CreateFromOBJ("block", true);
	textureHandle_ = TextureManager::Load("./Resources/block/block.png");
	textureHandle2_ = TextureManager::Load("./Resources/SkyDome/sky_sphere.png");
	texturePlayer_ = TextureManager::Load("./Resources/player/player.png");
	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("./Resources/mapchip.csv");
	model_ = Model::Create();
	camera_.Initialize();

	playerModel_ = Model::CreateFromOBJ("player", true);
	player_ = new Player();

	// 天球モデルの読み込み
	modelSkydome = Model::CreateFromOBJ("skydome", true);

	// 天球の生成と初期化
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome, textureHandle2_, &camera_);

	KamataEngine::Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 18);
	player_->Initialize(playerModel_, &camera_, playerPosition);
	player_->SetTextureHandle(texturePlayer_);
	player_->SetMapChipField(mapChipField_);

	// カメラコントローラーの生成と初期化
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();
	cameraController_->SetMovableArea({0.0f, 50.0f, 0.0f, 50.0f});
	player_->SetCamera(cameraController_->GetCameraPtr()); 

	// ブロック生成（関数化）
	GenerateBlocks();

	debugCamera_ = new DebugCamera(1280, 720);
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

	// 天球の更新
	skydome_->Update();

	for (size_t i = 0; i < worldTransformBlocks_.size(); ++i) {
		for (size_t j = 0; j < worldTransformBlocks_[i].size(); ++j) {

			if (worldTransformBlocks_[i][j] != nullptr) {
				UpdateWorldTransform(*worldTransformBlocks_[i][j]);
			}
		}
	}

	player_->Update();
	cameraController_->Update();
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
	player_->Draw();

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
	delete debugCamera_;
	debugCamera_ = nullptr;
	delete mapChipField_;
	mapChipField_ = nullptr;
	delete cameraController_;
	cameraController_ = nullptr;
}