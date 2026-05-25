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
	while (true) {
		if (KamataEngine::Update()) {
			break;
		}

		dxCommon->PreDraw();
		gameScene->Draw();
		dxCommon->PostDraw();
		gameScene->Update();
	}

	delete gameScene;
	gameScene = nullptr;
	KamataEngine::Finalize();
	return 0;
}
