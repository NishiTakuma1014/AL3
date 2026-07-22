#pragma once
#include "KamataEngine.h"

class Player;

class CameraController {
public:
	void Initialize();
	void Update();
	void Reset();

	void SetTarget(Player* target) { target_ = target; }

	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float top = 0.0f;
		float bottom = 1.0f;
	};
	void SetMovableArea(const Rect& area) { movableArea_ = area; }

	const KamataEngine::Camera& GetCamera() const { return camera_; }
	KamataEngine::Camera* GetCameraPtr() { return &camera_; }

private:
	Player* target_ = nullptr;
	KamataEngine::Camera camera_;

	// 追従対象とカメラの座標の差（オフセット）
	KamataEngine::Vector3 targetOffset_ = {0.0f, 0.0f, -15.0f};

	Rect movableArea_ = {0.0f, 100.0f, 0.0f, 100.0f};

	// 追従対象を画面内に収めるためのマージン
	static inline const Rect kMargin = {-6.0f, 6.0f, -4.0f, 4.0f};

	static inline const float kInterpolationRate = 0.1f; // 補間率の定数値
	static inline const float kVelocityBias = 0.3f;      // 速度による目標座標補正の重み
};