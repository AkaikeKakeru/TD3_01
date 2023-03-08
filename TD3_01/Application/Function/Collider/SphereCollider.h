#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

class SphereCollider
	: public BaseCollider, public Sphere {
public: //�����o�֐�
	SphereCollider(Vector3 offset = { 0,0,0 },
		float radius = 1.0f) :
		offset_(offset),
		radius_(radius) {
		//���`����Z�b�g
		shapeType_ = COLLISIONSHAPE_SPHERE;
	}

	//�X�V
	void Update() override;


public://�A�N�Z�b�T
	//���a�擾
	inline float GetRadius() {
		return radius_;
	}

	//�I�t�Z�b�g�擾
	inline const Vector3& GetOffset() {
		return offset_; 
	}

	//���a�Z�b�g
	inline void SetRadius(float radius) {
		radius_ = radius;
	}

	//�I�t�Z�b�g�Z�b�g
	inline void SetOffset(const Vector3& offset) {
		offset_ = offset;
	}

private: //�����o�ϐ�
	//�I�u�W�F�N�g���S����̃I�t�Z�b�g
	Vector3 offset_;
	//���a
	float radius_;
};