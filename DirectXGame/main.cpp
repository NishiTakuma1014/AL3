#include "GameScene.h"
#include <KamataEngine.h>
#include <Windows.h>
using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"GC2D_03_ニシ_タクマ");
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	GameScene* gameScene = new GameScene();
	
	gameScene->Initialize();

	// メインループ
	while (true) {

		// エンジンの更新
		if (KamataEngine::Update()) {

			break;
		}

		gameScene->Update();

		// 描画開始
		dxCommon->PreDraw();

		gameScene->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	delete gameScene;

	gameScene = nullptr;

	KamataEngine::Finalize();

	return 0;
}