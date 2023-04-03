#pragma once
#include "Vector2.h"
#include "Matrix4.h"

#include "Object3d.h"
#include "Camera.h"

class Cursor {
public:
	Vector3 Get3DRethiclePosition(const bool isOnScreen);

	void SetCamera(Camera* camera) {
		camera_ = camera;
	}

private:
	void CreateMatrixInverseViewPort();
	void CheckRayDirection();

private:
	Camera* camera_;

	Matrix4 matInverseVPV_ = Matrix4Identity();
	Vector2 mousePosition_ = {};

	Vector3 posNear_ = {};
	Vector3 posFar_ = {};
	Vector3 rayDirection_ = {};
};