#pragma once
#include "KamataEngine.h"
using namespace KamataEngine;
class TitleScene {
public:
	void Initialize();
	void Update();
	void Draw();
	void SetTextureHandle(uint32_t textureHandle) { textureHandle_ = textureHandle; }
	bool IsFinished() const { return finished_; }
	~TitleScene();

private:
	uint32_t textureHandle_ = 0u;
	bool finished_ = false;
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	Camera camera_;
};