#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include <vector>

// ゲームシーン
class GameScene {
	KamataEngine::Model* blockModel_ = nullptr;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks2_;

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