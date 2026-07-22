#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"
#include <array>
class MapChipField;
class Player {
	// マップとの当たり判定情報
	struct CollisionMapInfo {
		bool ceilingCollision = false;         // 天井衝突フラグ
		bool landing = false;                  // 着地フラグ
		bool wallCollision = false;            // 壁接触フラグ
		KamataEngine::Vector3 moveAmount = {}; // 移動量
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	void Update();
	void Draw();
	void SetTextureHandle(uint32_t textureHandle) { textureHandle_ = textureHandle; }
	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }
	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }
	void SetCamera(KamataEngine::Camera* camera) { camera_ = camera; }
	static inline const float kLimitRunSpeed = 0.5f; // 走る速度の制限値
	enum class LRDirection { kRight, kLeft };
	LRDirection lrDirection_ = LRDirection::kRight;                                    // 左右方向の向き
	float turnFirstrotationY_ = 0.0f;                                                  // 初期の回転角度
	float turnTimer_ = 0.0f;                                                           // 回転のタイマー
	static inline const float kTimeTurn = 0.3f;                                        // 回転にかかる時間
	bool onGround_ = true;                                                             // 地面に接地しているかどうか
	static inline const float kGravityAcceleration = 0.05f;                            // 重力加速度
	static inline const float kLimitFallSpeed = 0.5f;                                  // 落下速度の制限値
	static inline const float kJumpAcceleration = 0.5f;                                // ジャンプ加速度
	static inline const float kAttenuation = 0.2f;                                     // 速度減衰率
	float groundY_ = 0.0f;                                                             // 地面のY座標
	MapChipField* mapChipField_ = nullptr;                                             // マップチップフィールドのポインタ
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; } // マップチップフィールドの設定
	static inline const float kWidth = 0.8f;                                           // プレイヤーの幅
	static inline const float kHeight = 0.8f;                                          // プレイヤーの高さ
	void InputMove();                                                                  // 移動入力処理
	void CollisionMap(CollisionMapInfo& collisionMapInfo);                             // マップ衝突判定
	void MoveByCollisionResult(const CollisionMapInfo& info);                          // 判定結果を反映して移動させる
	void CeilingCollision(const CollisionMapInfo& info);                               // 天井に接触している場合の処理
	enum Corner { kLeftTop, kRightTop, kLeftBottom, kRightBottom, kNumCorners };       // プレイヤーの角の列挙型
	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);

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