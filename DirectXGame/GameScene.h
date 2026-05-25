#pragma once
#include <KamataEngine.h>

class GameScene {
	KamataEngine::DebugCamera *debugCamera_ = nullptr;

private: 
	uint32_t textureHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera* camera_;

public:
	GameScene() = default;
	float inputFloat3[3] = {0, 0, 0};
	uint32_t soundDataHandle_ = 0;
	uint32_t voiceDataHandle_ = 0;
	~GameScene();
	void Initialize();

	void Update();
	void Draw();
};