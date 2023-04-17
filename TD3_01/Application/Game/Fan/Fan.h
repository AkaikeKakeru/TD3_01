#pragma once

#include "Object3d.h"
#include "CollisionPrimitive.h"
#include "Quaternion.h"

#include "Cursor.h"

class Fan
	: public Object3d {
public: //enum
	typedef enum Direction {
		Up = 0,
		Down = 1,
		Right = 2,
		Left = 3,
	} Direction;

public: //�ÓI�����o�֐�
	static Fan* Create(Model* model);
public: //�����o�֐�
	bool Initialize() override;
	void Update() override;
	void Draw();
	void Finalize();

	//�Փˎ��R�[���o�b�N�֐�
	void OnCollision(const CollisionInfo& info) override;

	//�Ə�
	void Reticle();

public://�A�N�Z�b�T
	Ray* GetRay() {
		return ray_;
	}

	void SetRay(Ray* ray) {
		ray_ = ray;
	}

	void SetFanDirection(const int dirNum) {
		//��]�X�s�[�h(����)
		static const float verticalAngle = ConvertToRadian(90.0f);

		static const Vector3 angleX = { 1.0f,0.0f,0.0f };
		static const Vector3 angleY = { 0.0f,1.0f,0.0f };
		static const Vector3 angleZ = { 0.0f,0.0f,1.0f };

		Vector3 fanAngle = angleY;
		float fanRotaSpeed = 0.0f;
		Vector3 rayDir{};

		switch (dirNum) {
		case Up:
			fanRotaSpeed = verticalAngle * 2;
			rayDir = angleZ;
			break;

		case Down:
			fanRotaSpeed = verticalAngle * 0;
			rayDir = -angleZ;
			break;

		case Right:
			fanRotaSpeed = -verticalAngle;
			rayDir = angleX;
			break;

		case Left:
			fanRotaSpeed = verticalAngle;
			rayDir = -angleX;
			break;

		default:
			fanRotaSpeed = ConvertToRadian(30.0f);
			rayDir = angleZ;
			break;
		}

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

	//�͂݃t���O
	bool isGrab_= false;

	//���e�B�N���p
	WorldTransform worldTransform3dReticle_;

	//���e�B�N���J�[�\��
	Cursor cursor_;
};