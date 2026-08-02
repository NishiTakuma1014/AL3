#include "GameScene.h"
#include "Player.h"
#include "Skydome.h"
#include "TitleScene.h"
#include <KamataEngine.h>
#include <Windows.h>
using namespace KamataEngine;

// シーンの種類
enum class Scene {
	kUnknown = 0,

	kTitle,
	kGame,
};

// 現在のシーン
Scene scene = Scene::kUnknown;

// 各シーンのインスタンス
TitleScene* titleScene = nullptr;
GameScene* gameScene = nullptr;

// シーン切り替え・更新・描画（関数化）
void ChangeScene();
void UpdateScene();
void DrawScene();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"GC2D_03_ニシ_タクマ");
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();


	scene = Scene::kTitle;
	titleScene = new TitleScene();
	titleScene->Initialize();

	// メインループ
	while (true) {

		if (KamataEngine::Update()) {

			break;
		}

		// シーン切り替え
		ChangeScene();
		// 現在シーンの更新
		UpdateScene();

		// 描画開始
		dxCommon->PreDraw();

		// 現在シーンの描画
		DrawScene();

		// 描画終了
		dxCommon->PostDraw();
	}

	// シーン解放処理（どちらのシーンが生き残っていてもよいように両方delete）
	delete titleScene;
	delete gameScene;

	titleScene = nullptr;
	gameScene = nullptr;

	KamataEngine::Finalize();

	return 0;
}

// --- シーンの切り替え ---
void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinished()) {
			// シーン変更
			scene = Scene::kGame;
			// 旧シーンの解放
			delete titleScene;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			gameScene = new GameScene();
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		if (gameScene->IsFinished()) {
			// シーン変更
			scene = Scene::kTitle;
			// 旧シーンの解放
			delete gameScene;
			gameScene = nullptr;
			// 新シーンの生成と初期化
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;
	}
}

// --- シーンの更新 ---
void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	}
}

// --- シーンの描画 ---
void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	}
}