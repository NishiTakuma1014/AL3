#include "KamataEngine.h"

class Player {

public:
	void Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera);
	//void Initialize(Model* model,Camera*camera,const Vector3&position);	
	void Update();
	void Draw();
	void SetPosition(const KamataEngine::Vector3& position) { worldTransform_.translation_ = position; }

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	KamataEngine::Camera* camera_ = nullptr;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	int isDebugCameraActive_ = false;
};
