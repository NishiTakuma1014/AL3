#include "GameScene.h"
#include "2d/ImGuiManager.h"
using namespace KamataEngine;

GameScene::~GameScene() {
	
	delete sprite_;

	delete model_;
	delete camera_;
	delete debugCamera_;
}

void GameScene::Initialize() {
	
	//textureHandle_ = TextureManager::Load("./Resources/uvChecker.png");
	textureHandle_ = TextureManager::Load("./Resources/mario.png");
	soundDataHandle_ = Audio::GetInstance()->LoadWave("./Resources/fanfare.wav");
	Audio::GetInstance()->PlayWave(soundDataHandle_);
	voiceDataHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_, true);
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create();

	worldTransform_.Initialize();
	camera_ = new Camera();
	camera_->Initialize();

	PrimitiveDrawer::GetInstance()->SetCamera(camera_);
	debugCamera_ = new DebugCamera(5,5);
	
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
}

void GameScene::Update() {
 #ifdef _DEBUG
	ImGui::Begin("Debug1");
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::ShowDemoWindow();
	ImGui::Text("KamataEngine %d,%d,%d", 20, 50, 0);
	ImGui::End();
#endif
	Vector2 position = sprite_->GetPosition();
	position.x += 2.0f;
	position.y += 1.0f;
	sprite_->SetPosition(position);
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		Audio::GetInstance()->StopWave(voiceDataHandle_);
	}
	debugCamera_->Update();

}

void GameScene::Draw() {
	Model::PreDraw();
	model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHandle_);
	Model::PostDraw();
	Sprite::PreDraw();
	//sprite_->Draw();
	
	Sprite::PostDraw();
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});
	//model_->Draw(worldTransform_, *camera_,textureHandle_);
	
	

}
