#pragma once
#include "KamataEngine.h"
#include <array>
#include<numbers>
using namespace KamataEngine;

/// <summary>
/// デス演出用パーティクル
/// </summary>
class DeathParticles {
public:
	// パーティクルの個数
	static inline const uint32_t kNumParticles = 8;
	void Initialize(Model* model, Camera* camera, const Vector3& position);
	void Update();
	void Draw();
	void SetTextureHandle(uint32_t textureHandle) { textureHandle_ = textureHandle; }
	static inline const float kDuration = 2.0f;//存続時間
	static inline const float kSpeed = 0.2f;
	static inline const float kAngleUnit = std::numbers::pi_v<float> * 2.0f / kNumParticles; 

private:
	Model* model_ = nullptr;
	Camera* camera_ = nullptr;
	uint32_t textureHandle_ = 0u;
	std::array<WorldTransform, kNumParticles> worldTransforms_;
	// 終了フラグ
	bool isFinished_ = false;
	// 経過時間カウント
	float counter_ = 0.0f;
	// 色変更オブジェクト
	ObjectColor objectColor_;
	// 色の数値
	Vector4 color_;
};