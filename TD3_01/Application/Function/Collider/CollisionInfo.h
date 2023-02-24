#pragma once
#include "Vector3.h"

class Object3d;
class BaseCollider;

struct CollisionInfo {
public:
	CollisionInfo(Object3d* object, BaseCollider* collider,
		const Vector3& inter) {
		object_ = object;
		collider_ = collider;
		inter_ = inter;
	}

	//�Փˑ���̃I�u�W�F�N�g
	Object3d* object_ = nullptr;
	//�Փˑ���̃R���C�_�[
	BaseCollider* collider_ = nullptr;
	//�Փ˓_
	Vector3 inter_;
};