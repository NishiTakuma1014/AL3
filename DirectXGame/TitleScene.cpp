#include "TitleScene.h"
#include <numbers>
#include"new.h"
void TitleScene::Initialize() {
	finished_ = false;
	camera_.Initialize();
	model_ = Model::CreateFromOBJ("player", true);
	textureHandle_ = TextureManager::Load(".//Resources/player/player.png");
	worldTransform_.Initialize();
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};
	worldTransform_.rotation_.y = std::numbers::pi_v<float>;
}

void TitleScene::Update() {
	
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		finished_ = true;
	}
	UpdateWorldTransform(worldTransform_);
}

void TitleScene::Draw() {
	Model::PreDraw();

	model_->Draw(worldTransform_, camera_, textureHandle_);

	Model::PostDraw();
}

TitleScene::~TitleScene() {
	delete model_;
	model_ = nullptr;
}