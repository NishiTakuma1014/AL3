#pragma once
#include "KamataEngine.h"
class Skydome {

public:
	void Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera);
	void Update();
	void Draw();

	private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	uint32_t textureHandle2_ = 0u;
	KamataEngine::Camera* camera_ = nullptr;
};

