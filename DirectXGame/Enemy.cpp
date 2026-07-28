#include "Enemy.h"
#include <cassert>
#include <numbers>
#include "GameScene.h"
void Enemy::Initialize(Model* model, Camera* camera, const Vector3& position) {
	assert(model);
	model_ = model;
	camera_ = camera;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = {-kWalkSpeed, 0, 0};
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;
	walkTimer_ = 0.0f;
}

void Enemy::Update() {
	worldTransform_.translation_ += velocity_;
	UpdateWorldTransform(worldTransform_);
	walkTimer_ += 1.0f / 60.0f;
	//回転アニメーション
	float param = std::sin(2.0f * std::numbers::pi_v<float> * walkTimer_ / kWalkMotionTime);
	float degree = kWalkMotionAngleStart + (kWalkMotionAngleEnd - kWalkMotionAngleStart) * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = degree * (std::numbers::pi_v<float> / 180.0f); // 度をラジアンに変換

}

void Enemy::Draw() {
	model_->Draw(worldTransform_, *camera_, textureHandle_); 
}

void Enemy::OnCollision(const Player* player) { 
	(void)player; 
}
Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

AABB Enemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}