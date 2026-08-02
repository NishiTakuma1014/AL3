#pragma once
#include "CameraController.h"
#include "DeathParticles.h"
#include "Enemy.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Player.h"
#include "Skydome.h"
#include "TitleScene.h"
#include <list>
#include <vector>

class GameScene {
private:
	KamataEngine::Model* blockModel_ = nullptr;
	KamataEngine::Model* playerModel_ = nullptr;
	KamataEngine::Model* enemyModel_ = nullptr;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks2_;
	std::list<Enemy*> enemies_;
	static inline const int32_t kNumEnemies = 1;//敵の複数化

	KamataEngine::Model* modelSkydome = nullptr;
	Skydome* skydome_ = nullptr;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;
	MapChipField* mapChipField_;
	void GenerateBlocks();
	CameraController* cameraController_ = nullptr;
	void CheckAllCollisions();
	DeathParticles* deathParticles_ = nullptr;
	KamataEngine::Model* modelDeathParticle_ = nullptr;

public:
	// ゲームのフェーズ（型）
	enum class Phase {
		kPlay,  // ゲームプレイ
		kDeath, // デス演出
	};

private:
	// ゲームの現在フェーズ（変数）
	Phase phase_ = Phase::kPlay;

	// ゲームシーンが終了したかどうか
	bool finished_ = false;

	void ChangePhase();

public:
	void Initialize();
	void Update();
	void Draw();

	// ゲームシーンが終了したかどうか（main.cppのChangeSceneから参照する）
	bool IsFinished() const { return finished_; }

	~GameScene();
	uint32_t textureDeathParticle_ = 0;
	uint32_t textureHandle_ = 0;
	uint32_t textureHandle2_ = 0;
	uint32_t texturePlayer_ = 0;
	uint32_t textureEnemy_ = 0;
	uint32_t textureTitleScene_ = 0;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera camera_;
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
};