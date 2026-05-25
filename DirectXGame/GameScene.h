#pragma once
#include "KamataEngine.h"
#include "Player.h"

// ゲームシーン
class GameScene {

public:
	void Initialize();
	void Update();
	void Draw();

	~GameScene();

	uint32_t textureHandle_ = 0;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera camera_;
	Player* player_ = nullptr;
};