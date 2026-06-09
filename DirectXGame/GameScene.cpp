#include "GameScene.h"
#include "new.h" 
#include <cmath>

using namespace KamataEngine;

// --- シーンの初期化 ---
void GameScene::Initialize() {
	blockModel_ = Model::Create();
	textureHandle_ = TextureManager::Load("./Resources/cube/cube.jpg");
	textureHandle2_ = TextureManager::Load("./Resources/SkyDome/sky_sphere.png");
	texturePlayer_ = TextureManager::Load("./Resources/mario.png");
	model_ = Model::Create();
	camera_.Initialize();

	// 天球モデルの読み込み
	modelSkydome = Model::CreateFromOBJ("skydome", true);

	// 天球の生成と初期化 
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome, textureHandle2_, &camera_);

	player_ = new Player();
	player_->Initialize(model_, texturePlayer_, &camera_);
	//player_->worldTransform_.translation_ = {-1.0f, 2.0f, 0.0f};
	
	const uint32_t kNumBlockHorizontal = 20;
	const uint32_t kNumBlockVertical = 10;
	const float kBlockWidth = 1.0f;
	const float kBlockHeight = 1.0f;

	worldTransformBlocks_.resize(kNumBlockVertical);

	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();

			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * static_cast<float>(j);
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * static_cast<float>(i);
		}
	}

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
		
	}else {
		camera_.TransferMatrix();
	}

	// 天球の更新
	skydome_->Update();

	for (size_t i = 0; i < worldTransformBlocks_.size(); ++i) {
		for (size_t j = 0; j < worldTransformBlocks_[i].size(); ++j) {	
		UpdateWorldTransform(*worldTransformBlocks_[i][j]);
		}
	}

	player_->Update();
}

// --- シーンの描画 ---
void GameScene::Draw() {
	Model::PreDraw();
	skydome_->Draw();
	player_->Draw();

	for (size_t i = 0; i < worldTransformBlocks_.size(); ++i) {
		for (size_t j = 0; j < worldTransformBlocks_[i].size(); ++j) {
			model_->Draw(*worldTransformBlocks_[i][j], camera_, textureHandle_);
		}
	}

	Model::PostDraw();
}

// --- デストラクタ ---
GameScene::~GameScene() {
	if (blockModel_ != nullptr) {
		delete blockModel_;
	}

	for (size_t i = 0; i < worldTransformBlocks_.size(); ++i) {
		for (size_t j = 0; j < worldTransformBlocks_[i].size(); ++j) {
			delete worldTransformBlocks_[i][j];
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
	delete debugCamera_;
	debugCamera_ = nullptr;

}