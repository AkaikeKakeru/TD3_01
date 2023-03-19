#pragma once

#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include "Object3d.h"

class BaseCollider {
public: //�t�����h�N���X
	friend class CollisionManager;

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

	//�Փˎ��R�[���o�b�N�֐�
	inline void OnCollision(const CollisionInfo& info) {
		object_->OnCollision(info);
	}

	//�����Z�b�g
	inline void SetAttribute(unsigned short attribute) {
		this->attribute_ = attribute;
	}
	//�����ǉ�
	inline void AddAtribute(unsigned short attribute) {
		this->attribute_ |= attribute;
	}
	//������菜��
	inline void RemoveAttribute(unsigned short attribute) {
		this->attribute_ &= !attribute;
	}

protected: //�����o�ϐ�
	Object3d* object_ = nullptr;
	//�`��^�C�v
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

	//�Փˑ���
	unsigned short attribute_ = 0b1111111111111111;

public: //�R���X�g�f�X�g
	BaseCollider() = default;
	virtual ~BaseCollider() = default;

};