#pragma once
#include <KamataEngine.h>


KamataEngine::Matrix4x4 Multiply(const KamataEngine::Matrix4x4& m1, const KamataEngine::Matrix4x4& m2);
KamataEngine::Matrix4x4 MakeScaleMatrix(const KamataEngine::Vector3& scale);
KamataEngine::Matrix4x4 MakeRotateMatrix(const KamataEngine::Vector3& rotate);
KamataEngine::Matrix4x4 MakeTranslateMatrix(const KamataEngine::Vector3& translate);
KamataEngine::Matrix4x4 MakeAffineMatrix(const KamataEngine::Vector3& scale, const KamataEngine::Vector3& rotate, const KamataEngine::Vector3& translate);
void UpdateWorldTransform(KamataEngine::WorldTransform& worldTransform);