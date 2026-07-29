#include "DeathParticles.h"
#include "new.h" 
#include<numbers>
#include<array>
#include<algorithm>
void DeathParticles::Initialize(Model* model, Camera* camera, const Vector3& position) {
	model_ = model;
	camera_ = camera;

	// ワールド変換の初期化
	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
	// 色変更オブジェクトの初期化
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void DeathParticles::Update() {
	// 終了なら何もしない
	if (isFinished_) {
		return;
	}

	// 8個分の速度ベクトルを計算して、それぞれの速度だけ座標を進める
	for (uint32_t i = 0; i < kNumParticles; ++i) {
		// 基本となる速度ベクトル
		Vector3 velocity = {kSpeed, 0, 0};
		// 回転角を計算する
		float angle = kAngleUnit * i;
		// Z軸まわり回転行列
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		// 基本ベクトルを回転させて速度ベクトルを得る
		velocity = Transform(velocity, matrixRotation);
		// 移動処理
		worldTransforms_[i].translation_ += velocity;


	}

	// ワールド変換の更新
	for (WorldTransform& worldTransform : worldTransforms_) {
		UpdateWorldTransform(worldTransform);
	}

	// カウンターを1フレーム分の秒数進める
	counter_ += 1.0f / 60.0f;

	// 存続時間の上限に達したら
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		// 終了扱いにする
		isFinished_ = true;
	}
	// アルファ値をcounter_の進み具合に応じて減らしていく
	color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);
	// 色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);

}
void DeathParticles::Draw() {
	// 終了なら何もしない
	if (isFinished_) {
		return;
	}

	// モデルの描画（色変更オブジェクトを渡して色変更描画にする）
	for (auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *camera_, &objectColor_);
	}
}