#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include "Skydome.h" 
#include <vector>
#include "MapChipField.h"
#include"CameraController.h"
#include"Enemy.h"
#include <list>   
#include"DeathParticles.h"
class GameScene {
private:
	KamataEngine::Model* blockModel_ = nullptr;
	KamataEngine::Model* playerModel_ = nullptr;
	KamataEngine::Model* enemyModel_ = nullptr;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks2_;
	std::list<Enemy*> enemies_;
	static inline const int32_t kNumEnemies = 3; 
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
	void Initialize();
	void Update();
	void Draw();

	~GameScene();
	uint32_t textureDeathParticle_ = 0;
	uint32_t textureHandle_ = 0;
	uint32_t textureHandle2_ = 0;
	uint32_t texturePlayer_ = 0;
	uint32_t textureEnemy_ = 0;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera camera_;
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
};