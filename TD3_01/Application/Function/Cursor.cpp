#include "Cursor.h"
#include "Input.h"
#include <WinApp.h>
#include <cassert>

Vector3 Cursor::Get3DRethiclePosition(const bool isOnScreen) {
	assert(camera_);

	//マウスの座標を取得
	mousePosition_ = Input::GetInstance()->GetMousePosition();

	CreateMatrixInverseViewPort();

	CheckRayDirection();

	Vector3 position;

	if (rayDirection_.y <= 0) {
		float Lray = Vector3Dot(rayDirection_, Vector3(0, 1, 0));
		float LP0 = Vector3Dot(-posNear_, Vector3(0, 1, 0));
		position = posNear_ + (LP0 / Lray) * rayDirection_;
	}
	else {
		position = posFar_;
	}

	return position;
}

void Cursor::CreateMatrixInverseViewPort(){
	matInverseVPV_ = Matrix4Identity();

	Matrix4 matVPV = camera_->GetViewProjectionMatrix() * 
		camera_->GetViewPortMatrix();

	//上を逆行列化
	matInverseVPV_ = Matrix4Inverse(matVPV);
}

void Cursor::CheckRayDirection(){
	//ニア
	posNear_ = Vector3(
		mousePosition_.x,
		mousePosition_.y,
		0.0f);
	//ファー
	posFar_ = Vector3(
		mousePosition_.x,
		mousePosition_.y,
		1.0f);

	posNear_ = Vector3TransformCoord(posNear_, matInverseVPV_);
	posFar_ = Vector3TransformCoord(posFar_, matInverseVPV_);

	//レイ
	rayDirection_ = posFar_ - posNear_;
	rayDirection_ = Vector3Normalize(rayDirection_);
}
