#pragma once

#include "Object3d.h"
#include "CollisionPrimitive.h"

class Fan 
	: public Object3d{
public: //�ÓI�����o�֐�
	static Fan* Create(Model* model);
public: //�����o�֐�
	bool Initialize() override;
	void Update() override;
	void Draw();
	void Finalize();

	//�Փˎ��R�[���o�b�N�֐�
	void OnCollision(const CollisionInfo & info) override;

public://�A�N�Z�b�T
	Ray GetRay() {
		return ray_;
	}

private://�����o�ϐ�
	Ray ray_;
	//��]�x�N�g��
	Vector3 rotVector_ = { 0,0,0 };
};