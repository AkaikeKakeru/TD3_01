#include "Fan.h"

#include "Input.h"
#include "SphereCollider.h"

#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include <cassert>
#include <Quaternion.h>

Fan* Fan::Create(Model* model) {
	//�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Fan* instance = new Fan();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	//���f���̃Z�b�g
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

bool Fan::Initialize() {
	if (!Object3d::Initialize()) {
		return false;
	}

	//���C�̏����ݒ�
	//���C�̏����l
	ray_.start_ = {0,0,0};
	ray_.dir_ = { 0,0,1};

	//�R���C�_�|�ǉ�
	//float radius = 0.6f;
	////���a�������������畂�������W�����̒��S�ɂ���
	//SetCollider(new SphereCollider(
	//	Vector3{ 0.0f,radius,0.0f },
	//	radius)
	//);

	//collider_->SetAttribute(COLLISION_ATTR_RAY);

	return true;
}

void Fan::Update() {
	assert(&ray_);

	Input* input_ = Input::GetInstance();

	// �I�u�W�F�N�g�ړ�

	// ���݂̉�]���擾
	Vector3 rot = Object3d::GetRotation();

	//��]�X�s�[�h
	float verticalAngle = ConvertToRadian(90.0f);

	Vector3 angleX = { 1.0f,0.0f,0.0f };
	Vector3 angleY = { 0.0f,1.0f,0.0f };
	Vector3 angleZ = { 0.0f,0.0f,1.0f };

	//�ړ���̍��W���v�Z
	if (input_->TriggerKey(DIK_W)) {
		rotVector_ = { verticalAngle,0,0 };
		//CreateRotationVector(angleX, rotSpeed);
	}

	else if (input_->TriggerKey(DIK_S)) {
		rotVector_ = { -verticalAngle ,0,0};
		//CreateRotationVector(angleX, -rotSpeed);
	}

	if (input_->TriggerKey(DIK_A)) {
		rotVector_ = { 0,0,verticalAngle };
		//CreateRotationVector(angleY, rotSpeed);
	}

	else if (input_->TriggerKey(DIK_D)) {
		rotVector_ = { 0,-verticalAngle,0 };
		//CreateRotationVector(angleY, -rotSpeed);
	}

	rot = rotVector_;


	// ��]�̕ύX�𔽉f
	Object3d::SetRotation(rot);

	ray_.start_ = Object3d::GetPosition();
	//ray_.dir_ = Object3d::GetRotation();

	camera_->Update();
	Object3d::Update();
}

void Fan::Draw() {
	Object3d::Draw();
}

void Fan::Finalize() {
}

void Fan::OnCollision(const CollisionInfo& info) {
}
