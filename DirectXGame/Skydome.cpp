#include "Skydome.h"
#include"new.h"
using namespace KamataEngine;


void Skydome::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera) {
	model_ = model;
	textureHandle2_ = textureHandle;
	camera_ = camera;
	worldTransform_.Initialize();
	
}
void Skydome::Update() { 
	UpdateWorldTransform(worldTransform_);
	worldTransform_.TransferMatrix(); }

void Skydome::Draw() { model_->Draw(worldTransform_, *camera_, textureHandle2_); }