#define NOMINMAX
#include "Player.h"
#include "cassert"
#include "new.h"
#include <Windows.h>
#include <algorithm>
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

	// 左右移動操作
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
		// 左右加速
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			acceleration.x += kAcceleration;
			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;
				turnFirstrotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = kTimeTurn; // ← 方向転換タイマーをリセット
			}
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
				turnFirstrotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = kTimeTurn; // ← 方向転換タイマーをリセット
			}
			acceleration.x -= kAcceleration;
		}
		velocity_ += acceleration;
		// 速度制限
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	} else if(onGround_) {
		// キーを離しているときは減速
		velocity_.x *= (1.0f - kAcceleration);
	}

	// 旋回制御
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f; // フレームレート60fpsを想定
		if (turnTimer_ <= 0.0f) {
			turnTimer_ = 0.0f;
		}
		float destinationRotationYTable[] = {
		    std::numbers::pi_v<float> / 2.0f,       // 右向き
		    std::numbers::pi_v<float> * 3.0f / 2.0f // 左向き
		};
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		worldTransform_.rotation_.y = destinationRotationY;
	}

// --- ジャンプ・重力処理 ---
	if (onGround_) {
		// 接地中：上キーでジャンプ
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			groundY_ = worldTransform_.translation_.y; // ジャンプする直前の高さを記憶
			velocity_.y = kJumpAcceleration;
			onGround_ = false;
		} else {
			velocity_.y = 0.0f;
		}
	} else {
		// 空中：重力を適用
		velocity_ += Vector3{0.0f, -kGravityAcceleration, 0.0f};
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	// --- 着地判定 ---
	bool landing = false;
	if (velocity_.y < 0.0f) { // 落下中
		if (worldTransform_.translation_.y <= groundY_) {
			landing = true;
		}
	}

	if (!onGround_ && landing) {
		worldTransform_.translation_.y = groundY_; // ジャンプ前の高さに戻す
		velocity_.x *= (1.0f - kAcceleration);
		velocity_.y = 0.0f;
		onGround_ = true;
	}

	worldTransform_.TransferMatrix();
	// アフェイン変換行列の計算してメンバ変数に代入する定数バッファに転送する
	UpdateWorldTransform(worldTransform_);
}

void Player::Draw() { model_->Draw(worldTransform_, *camera_, textureHandle_); }