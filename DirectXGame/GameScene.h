#pragma once
#include <KamataEngine.h>

class GameScene {
private: 
	uint32_t textureHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera* camera_;

public:
	GameScene() = default;
	~GameScene();
	void Initialize();

	void Update();
	void Draw();
};