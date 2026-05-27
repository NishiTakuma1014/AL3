#include "GameScene.h"
#include <cmath>

using namespace KamataEngine;

// --- 行列計算関数群 ---
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[i][0] * m2.m[0][j] + m1.m[i][1] * m2.m[1][j] + m1.m[i][2] * m2.m[2][j] + m1.m[i][3] * m2.m[3][j];
		}
	}
	return result;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 mat = {};
	mat.m[0][0] = scale.x;
	mat.m[1][1] = scale.y;
	mat.m[2][2] = scale.z;
	mat.m[3][3] = 1.0f;
	return mat;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 mat = {};
	mat.m[0][0] = 1.0f;
	mat.m[1][1] = 1.0f;
	mat.m[2][2] = 1.0f;
	mat.m[3][3] = 1.0f;
	mat.m[3][0] = translate.x;
	mat.m[3][1] = translate.y;
	mat.m[3][2] = translate.z;
	return mat;
}

Matrix4x4 MakeRotateMatrix(const Vector3& rotate) {
	Matrix4x4 rx = {};
	rx.m[0][0] = 1.0f;
	rx.m[3][3] = 1.0f;
	rx.m[1][1] = std::cos(rotate.x);
	rx.m[1][2] = std::sin(rotate.x);
	rx.m[2][1] = -std::sin(rotate.x);
	rx.m[2][2] = std::cos(rotate.x);
	Matrix4x4 ry = {};
	ry.m[1][1] = 1.0f;
	ry.m[3][3] = 1.0f;
	ry.m[0][0] = std::cos(rotate.y);
	ry.m[0][2] = -std::sin(rotate.y);
	ry.m[2][0] = std::sin(rotate.y);
	ry.m[2][2] = std::cos(rotate.y);
	Matrix4x4 rz = {};
	rz.m[2][2] = 1.0f;
	rz.m[3][3] = 1.0f;
	rz.m[0][0] = std::cos(rotate.z);
	rz.m[0][1] = std::sin(rotate.z);
	rz.m[1][0] = -std::sin(rotate.z);
	rz.m[1][1] = std::cos(rotate.z);
	return Multiply(Multiply(rx, ry), rz);
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	return Multiply(Multiply(MakeScaleMatrix(scale), MakeRotateMatrix(rotate)), MakeTranslateMatrix(translate));
}

// --- シーンの初期化 ---
void GameScene::Initialize() {
	blockModel_ = Model::Create();
	textureHandle_ = TextureManager::Load("./Resources/cube/cube.jpg");
	model_ = Model::Create();
	camera_.Initialize();

	player_ = new Player();
	player_->Initialize(model_, textureHandle_, &camera_);

	const uint32_t kNumBlockHorizontal = 20;
	const uint32_t kNumBlockVertical = 10;
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

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
}

// --- シーンの更新 ---
void GameScene::Update() {
	
	for (size_t i = 0; i < worldTransformBlocks_.size(); ++i) {
		for (size_t j = 0; j < worldTransformBlocks_[i].size(); ++j) {
			WorldTransform* block = worldTransformBlocks_[i][j];

			Matrix4x4 affineMatrix = MakeAffineMatrix(block->scale_, block->rotation_, block->translation_);
			block->matWorld_ = affineMatrix;
			block->TransferMatrix();
		}
	}

	player_->Update();
}

// --- シーンの描画 ---
void GameScene::Draw() {
	Model::PreDraw();

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
}