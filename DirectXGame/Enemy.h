#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"
#include <array>
#include"new.h"
#include"Player.h"
using namespace KamataEngine;

/// <summary>
/// 敵
/// </summary>
class Player;
class Enemy {
public:
	void Initialize(Model* model, Camera* camera, const Vector3& position);
	void SetCamera(KamataEngine::Camera* camera) { camera_ = camera; }
	void Update();
	void Draw();
	void SetTextureHandle(uint32_t textureHandle) { textureHandle_ = textureHandle; }
	MapChipField* mapChipField_ = nullptr;                                             
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; } 
	static inline const float kWalkSpeed = 0.02f; //歩行の速さ	
	static inline const float kWalkMotionAngleStart = -30.0f; 	//最初の角度
	static inline const float kWalkMotionAngleEnd = 30.0f;//最後の角度
	static inline const float kWalkMotionTime = 1.0f;//アニメーションの時間
	float walkTimer_ = 0.0f;//経過時間
	void OnCollision(const Player* player);
	KamataEngine::Vector3 GetWorldPosition();
	AABB GetAABB();            

	static inline const float kWidth = 0.8f;  
	static inline const float kHeight = 0.8f; 

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	KamataEngine::Camera* camera_ = nullptr;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	KamataEngine::Vector3 velocity_ = {};
	int isDebugCameraActive_ = false;
	static inline const float kAcceleration = 0.1f;
	static inline const float kBlank = 0.001f; // ブロックとの隙間
};