#include "GameScene.h"

using namespace KamataEngine;

GameScene::~GameScene() {
	
	delete sprite_;

	delete model_;
	delete camera_;
}

void GameScene::Initialize() {
	
	//textureHandle_ = TextureManager::Load("./Resources/uvChecker.png");
	textureHandle_ = TextureManager::Load("./Resources/mario.png");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create();

	worldTransform_.Initialize();
	camera_ = new Camera();
	camera_->Initialize();

}

void GameScene::Update() {
	Vector2 position = sprite_->GetPosition();
	position.x += 2.0f;
	position.y += 1.0f;
	sprite_->SetPosition(position);
}

void GameScene::Draw() {
	Sprite::PreDraw();
	//sprite_->Draw();
	Sprite::PostDraw();
	Model::PreDraw();
	model_->Draw(worldTransform_, *camera_,textureHandle_);
	Model::PostDraw();

}
