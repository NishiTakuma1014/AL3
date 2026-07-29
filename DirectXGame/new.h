#pragma once
#include <KamataEngine.h>


KamataEngine::Matrix4x4 Multiply(const KamataEngine::Matrix4x4& m1, const KamataEngine::Matrix4x4& m2);
KamataEngine::Matrix4x4 MakeScaleMatrix(const KamataEngine::Vector3& scale);
KamataEngine::Matrix4x4 MakeRotateMatrix(const KamataEngine::Vector3& rotate);
KamataEngine::Matrix4x4 MakeTranslateMatrix(const KamataEngine::Vector3& translate);
KamataEngine::Matrix4x4 MakeAffineMatrix(const KamataEngine::Vector3& scale, const KamataEngine::Vector3& rotate, const KamataEngine::Vector3& translate);
KamataEngine::Vector3 operator+(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);
KamataEngine::Vector3& operator+=(KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);
KamataEngine::Vector3 Add(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2);
KamataEngine::Vector3 Lerp(const KamataEngine::Vector3& start, const KamataEngine::Vector3& end, float t);
void UpdateWorldTransform(KamataEngine::WorldTransform& worldTransform);
KamataEngine::Matrix4x4 MakeRotateZMatrix(float radian);
KamataEngine::Vector3 Transform(const KamataEngine::Vector3& vector, const KamataEngine::Matrix4x4& matrix);