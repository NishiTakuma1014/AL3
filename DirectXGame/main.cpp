#include <Windows.h>
#include<KamataEngine.h>
#include"GameScene.h"

using namespace KamataEngine;
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	KamataEngine::Initialize(L"GC1D_03_ニシ_タクマ");

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// ゲームシーンのインスタンスを生成
	GameScene* gameScene = new GameScene();
	gameScene->Initialize();
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	while (true) {
		if (KamataEngine::Update()) {
			break;
		}
		imguiManager->Begin();
		gameScene->Update();
		imguiManager->End();
		dxCommon->PreDraw();
		gameScene->Draw();
		AxisIndicator::GetInstance()->Draw();
		imguiManager->Draw();
		dxCommon->PostDraw();
		
	}

	delete gameScene;
	gameScene = nullptr;
	KamataEngine::Finalize();
	return 0;
}
