#include "Player.h"
#include "cassert"
#include"new.h"
using namespace KamataEngine;

void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera) {
    assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	camera_ = camera;
	worldTransform_.Initialize();
}

void Player::Update() {

	worldTransform_.TransferMatrix();
	// アフェイン変換行列の計算してメンバ変数に代入する定数バッファに転送する
	UpdateWorldTransform(worldTransform_);

}

void Player::Draw() {

	
	model_->Draw(worldTransform_, *camera_, textureHandle_);
}
