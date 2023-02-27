#pragma once

#include "CollisionTypes.h"
#include "Object3d.h"

class BaseCollider {
public: //�����o�֐�
	virtual void Update() = 0;

public: //�A�N�Z�b�T
	//�I�u�W�F�N�g�Z�b�g
	inline void SetObject(Object3d* object) {
		object_ = object; }
	//�I�u�W�F�N�g�擾
	inline Object3d* GetObject3d() {
		return object_; }

	//�`��^�C�v�擾
	inline CollisionShapeType GetShapeType() {
		return shapeType_; }

protected: //�����o�ϐ�
	Object3d* object_ = nullptr;
	//�`��^�C�v
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

public: //�R���X�g�f�X�g
	BaseCollider() = default;
	virtual ~BaseCollider() = default;

};