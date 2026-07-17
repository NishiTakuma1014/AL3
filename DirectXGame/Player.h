#pragma once
#include "KamataEngine.h"

class Player {
	static inline const float kAcceleration = 0.1f; // 加速度の定数値
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	void Update();
	void Draw();
	void SetTextureHandle(uint32_t textureHandle) { textureHandle_ = textureHandle; }

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	KamataEngine::Camera* camera_ = nullptr;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	KamataEngine::Vector3 velocity_ = {};
	int isDebugCameraActive_ = false;
};