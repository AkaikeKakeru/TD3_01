#pragma once

#include "BaseCollider.h"

class Object3d;

struct RaycastHit {
	//�Փˑ���̃I�u�W�F�N�g
	Object3d* object_ = nullptr;
	//�Փˑ���̃R���C�_�[
	BaseCollider* collider_ = nullptr;
	//�Փ˓_
	Vector3 inter_;
	//�Փ˓_�܂ł̋���
	float distance_ = 0.0f;
};