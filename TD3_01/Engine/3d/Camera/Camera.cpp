#include "Camera.h"
#include "Vector4.h"
#include <cmath>
#include <WinApp.h>

Camera::Camera() {
	viewProjection_.Initialize();

	//行列の計算
	viewProjection_.UpdateMatrix();

	//合成
	matViewProjection_ = viewProjection_.matView_ * viewProjection_.matProjection_;

	//ビューポート行列
	matViewPort_ = Matrix4Identity();
	matViewPort_.m[0][0] = static_cast<float>(WinApp::Win_Width) / 2;
	matViewPort_.m[1][1] = static_cast<float>(-(WinApp::Win_Height)) / 2;
	matViewPort_.m[3][0] = static_cast<float>(WinApp::Win_Width) / 2;
	matViewPort_.m[3][1] = static_cast<float>(WinApp::Win_Height) / 2;
}

Camera::~Camera() {
}

void Camera::Update() {
	if (viewDirty_ || projectionDirty_) {
		// 再計算必要なら
		if (viewDirty_) {
			//ビュー行列の計算
			viewProjection_.UpdateViewMatrix();
			viewDirty_ = false;
		}

		// 再計算必要なら
		if (projectionDirty_) {
			//プロジェクション行列の計算
			viewProjection_.UpdateProjectionMatrix();
			projectionDirty_ = false;
		}
		//合成
		matViewProjection_ = viewProjection_.matView_ * viewProjection_.matProjection_;
	}

	viewProjection_.TransferMatrix();
}

void Camera::MoveVector(const Vector3& move) {
	Vector3 eye_moved = GetEye();
	Vector3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void Camera::MoveEyeVector(const Vector3& move) {
	Vector3 eye_moved = GetEye();
	Vector3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	SetEye(eye_moved);
}
