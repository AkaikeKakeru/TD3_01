#pragma once

#include "Object3d.h"
#include "CollisionPrimitive.h"
#include "Quaternion.h"
#include "Particle.h"
#include "ParticleManager.h"


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
	void DrawWind();
	void Finalize();

	//�Փˎ��R�[���o�b�N�֐�
	void OnCollision(const CollisionInfo& info) override;

	//�Ə�
	void Reticle();
	//��
	void ActiveWind(const int dir);

public://�A�N�Z�b�T
	Ray* GetRay() {
		return ray_;
	}

	void SetRay(Ray* ray) {
		ray_ = ray;
	}

	void SetFanDirection(const int dirNum);

	//����t���O�̃Z�b�g
	void SetIsControl(bool isControl) {
		isControl_ = isControl;
	}

	int GetFanDirection() { return direction_; }
	void SetDirection(const int dir) { this->direction_ = dir; }
private://�����o�ϐ�
	//����
	size_t direction_ = Up;
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

	//��
	Particle* wind_ = nullptr;
	ParticleManager* windpm_ = nullptr;

};