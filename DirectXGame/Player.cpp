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

void Player::CeilingCollision(const CollisionMapInfo& info) {
	// 天井に当たった？
	if (info.ceilingCollision) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}

void Player::InputMove() {
	// 左右移動操作
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
		// 左右加速
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			// 左移動中の右入力
			if (velocity_.x < 0.0f) {
				// 速度と逆方向に入力中は急ブレーキ
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x += kAcceleration;

			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;
				turnFirstrotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = kTimeTurn; // ← 方向転換タイマーをリセット
			}
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			// 右移動中の左入力
			if (velocity_.x > 0.0f) {
				// 速度と逆方向に入力中は急ブレーキ
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x -= kAcceleration;

			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
				turnFirstrotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = kTimeTurn; // ← 方向転換タイマーをリセット
			}
		}
		// 加速/減速
		velocity_ += acceleration;

		// 最大速度制限
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	} else {
		// 非入力時は移動減衰をかける
		velocity_.x *= (1.0f - kAttenuation);
	}

	// ジャンプ入力（接地中に上キーでジャンプ）
	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			groundY_ = worldTransform_.translation_.y; // ジャンプする直前の高さを記憶
			velocity_.y = kJumpAcceleration;
			onGround_ = false;
		} else {
			velocity_.y = 0.0f;
		}
	}
}

void Player::MoveByCollisionResult(const CollisionMapInfo& info) {
	// 移動
	worldTransform_.translation_ += info.moveAmount;
}

void Player::Update() {

	// 移動入力
	InputMove();

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

	// --- 重力処理（空中にいる間だけ） ---
	if (!onGround_) {
		velocity_ += Vector3{0.0f, -kGravityAcceleration, 0.0f};
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	// 衝突判定を初期化
	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.moveAmount = velocity_;

	// マップ衝突チェック
	CollisionMap(collisionMapInfo);

	// 判定結果を反映して移動させる
	MoveByCollisionResult(collisionMapInfo);

	// 天井に接触している場合の処理
	CeilingCollision(collisionMapInfo);

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

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorners] = {
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}, // kLeftTop（左上）
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, // kRightTop（右上）
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, // kLeftBottom（左下）
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, // kRightBottom（右下）
	};
	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::CollisionMap(CollisionMapInfo& info) {
	// 上方向判定
	// 上昇あり？
	if (info.moveAmount.y <= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorners> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveAmount, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false;

	// 左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.moveAmount.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - kHeight / 2.0f);
		// 天井に当たったことを記録する
		info.ceilingCollision = true;
	}
}