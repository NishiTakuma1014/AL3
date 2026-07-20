#define NOMINMAX
#include "CameraController.h"
#include "Player.h"
#include "new.h"
#include <algorithm>

using namespace KamataEngine;

void CameraController::Initialize() {
	camera_.Initialize();
	Reset();
}

void CameraController::Update() {
	if (!target_) {
		return;
	}

	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象の速度を取得
	const Vector3& targetVelocity = target_->GetVelocity();

	// 追従対象とオフセットと追従対象の速度からカメラの目標座標を計算
	Vector3 goalPosition = Add(Add(targetWorldTransform.translation_, targetOffset_), Vector3{targetVelocity.x * kVelocityBias, targetVelocity.y * kVelocityBias, targetVelocity.z * kVelocityBias});

	// 座標補間によりゆったり追従
	camera_.translation_ = Lerp(camera_.translation_, goalPosition, kInterpolationRate);

	// 移動範囲制限（ステージ全体の範囲に収める）
	camera_.translation_.x = std::clamp(camera_.translation_.x, movableArea_.left, movableArea_.right);
	camera_.translation_.y = std::clamp(camera_.translation_.y, movableArea_.top, movableArea_.bottom);

	// 追従対象が画面外に出ないように補正
	camera_.translation_.x = std::max(camera_.translation_.x, targetWorldTransform.translation_.x + kMargin.left);
	camera_.translation_.x = std::min(camera_.translation_.x, targetWorldTransform.translation_.x + kMargin.right);
	camera_.translation_.y = std::max(camera_.translation_.y, targetWorldTransform.translation_.y + kMargin.bottom);
	camera_.translation_.y = std::min(camera_.translation_.y, targetWorldTransform.translation_.y + kMargin.top);

	// 行列を更新する
	camera_.UpdateMatrix();
}

void CameraController::Reset() {
	if (!target_) {
		return;
	}

	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットからカメラの座標を計算
	camera_.translation_ = Add(targetWorldTransform.translation_, targetOffset_);
	camera_.UpdateMatrix();
}