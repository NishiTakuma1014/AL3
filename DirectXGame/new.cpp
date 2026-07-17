#include "new.h"
#include <cmath>
#include <KamataEngine.h>
#include <cassert>
using namespace KamataEngine;

// 行列計算の実体
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[i][0] * m2.m[0][j] + m1.m[i][1] * m2.m[1][j] + m1.m[i][2] * m2.m[2][j] + m1.m[i][3] * m2.m[3][j];
		}
	}
	return result;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 mat = {};
	mat.m[0][0] = scale.x;
	mat.m[1][1] = scale.y;
	mat.m[2][2] = scale.z;
	mat.m[3][3] = 1.0f;
	return mat;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 mat = {};
	mat.m[0][0] = 1.0f;
	mat.m[1][1] = 1.0f;
	mat.m[2][2] = 1.0f;
	mat.m[3][3] = 1.0f;
	mat.m[3][0] = translate.x;
	mat.m[3][1] = translate.y;
	mat.m[3][2] = translate.z;
	return mat;
}

Matrix4x4 MakeRotateMatrix(const Vector3& rotate) {
	Matrix4x4 rx = {};
	rx.m[0][0] = 1.0f;
	rx.m[3][3] = 1.0f;
	rx.m[1][1] = std::cos(rotate.x);
	rx.m[1][2] = std::sin(rotate.x);
	rx.m[2][1] = -std::sin(rotate.x);
	rx.m[2][2] = std::cos(rotate.x);

	Matrix4x4 ry = {};
	ry.m[1][1] = 1.0f;
	ry.m[3][3] = 1.0f;
	ry.m[0][0] = std::cos(rotate.y);
	ry.m[0][2] = -std::sin(rotate.y);
	ry.m[2][0] = std::sin(rotate.y);
	ry.m[2][2] = std::cos(rotate.y);

	Matrix4x4 rz = {};
	rz.m[2][2] = 1.0f;
	rz.m[3][3] = 1.0f;
	rz.m[0][0] = std::cos(rotate.z);
	rz.m[0][1] = std::sin(rotate.z);
	rz.m[1][0] = -std::sin(rotate.z);
	rz.m[1][1] = std::cos(rotate.z);

	return Multiply(Multiply(rx, ry), rz);
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	return Multiply(Multiply(MakeScaleMatrix(scale), MakeRotateMatrix(rotate)), MakeTranslateMatrix(translate));
}

KamataEngine::Vector3 operator+(const KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2) { return KamataEngine::Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }

KamataEngine::Vector3& operator+=(KamataEngine::Vector3& v1, const KamataEngine::Vector3& v2) {
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	return v1;
}
void UpdateWorldTransform(WorldTransform& worldTransform) {
	worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
	worldTransform.TransferMatrix();
}