#include "Player.h"
#include "cassert"
#include "new.h"
#include <numbers>
using namespace KamataEngine;

void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	assert(model);
	model_ = model;
	camera_ = camera;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f; // 180度回転
}

void Player::Update() {

	// 移動処理

	worldTransform_.translation_ += velocity_;
	//左右移動操作
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
		// 左右加速
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			acceleration.x += kAcceleration;
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			acceleration.x -= kAcceleration;
		}
		velocity_ += acceleration;
	}

	worldTransform_.TransferMatrix();
	// アフェイン変換行列の計算してメンバ変数に代入する定数バッファに転送する
	UpdateWorldTransform(worldTransform_);
}

void Player::Draw() {
	model_->Draw(worldTransform_, *camera_, textureHandle_); 
}