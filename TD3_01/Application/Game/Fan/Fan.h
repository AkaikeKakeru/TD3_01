#pragma once

#include "Object3d.h"
#include "CollisionPrimitive.h"
#include "Quaternion.h"

#include "Cursor.h"

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

	//�Ə�
	void Reticle();

public://�A�N�Z�b�T
	Ray* GetRay() {
		return ray_;
	}

	void SetRay(Ray* ray) {
		ray_ = ray;
	}

	void SetFanDirection(Vector3 fanAngle,
		float fanRotaSpeed,
		Vector3 rayDir) {
		// ���݂̉�]���擾
		Vector3 rot = Object3d::GetRotation();

		rotVector_ = CreateRotationVector(fanAngle, fanRotaSpeed);

		rot = fanAngle * fanRotaSpeed;

		// ��]�̕ύX�𔽉f
		Object3d::SetRotation(rot);

		ray_->start_ = Object3d::GetPosition();
		ray_->dir_ = rayDir;
	}

	//����t���O�̃Z�b�g
	void SetIsControl(bool isControl) {
		isControl_ = isControl;
	}

private://�����o�ϐ�
	Ray* ray_ = nullptr;
	//��]�x�N�g��
	Vector3 rotVector_ = { 0,0,0 };

	//����t���O
	bool isControl_ = false;

	//���e�B�N���p
	WorldTransform worldTransform3dReticle_;

	//���e�B�N���J�[�\��
	Cursor cursor_;
};