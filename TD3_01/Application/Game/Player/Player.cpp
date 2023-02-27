#include "Player.h"
#include "Quaternion.h"
#include "SafeDelete.h"
#include "Input.h"
#include "SphereCollider.h"
#include <cassert>

Player* Player::Create(Model* model) {
	//�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Player* instance = new Player();
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

bool Player::Initialize() {
	if (!Object3d::Initialize()) {
		return false;
	}

	//�R���C�_�|�ǉ�
	float radius = 0.6f;
	//���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(
		Vector3{ 0.0f,radius,0.0f },
		radius)
	);

	return true;
}

void Player::Update() {
	Input* input_ = Input::GetInstance();

	// �I�u�W�F�N�g�ړ�
	if (input_->PressKey(DIK_UP) ||
		input_->PressKey(DIK_DOWN) ||
		input_->PressKey(DIK_RIGHT) ||
		input_->PressKey(DIK_LEFT))
	{
		// ���݂̍��W���擾
		Vector3 position = Object3d::GetPosition();
		// ���݂̍��W���擾
		Vector3 rot = Object3d::GetRotation();

		//��]�x�N�g��
		Vector3 rotVector = {};

		//�ړ��X�s�[�h
		float moveSpeed = 0.5f;
		//��]�X�s�[�h
		float rotSpeed = ConvertToRadian(1.0f);

		Vector3 angleX = { 1.0f,0.0f,0.0f };
		Vector3 angleZ = { 0.0f,0.0f,1.0f };

		//�ړ���̍��W���v�Z
		if (input_->PressKey(DIK_UP)) {
			// �ړ���̍��W���v�Z
			position.y += moveSpeed;

			rotVector = CreateRotationVector(
				angleX, rotSpeed);
		}

		else if (input_->PressKey(DIK_DOWN)) {
			position.y -= moveSpeed;

			rotVector = CreateRotationVector(
				angleX, -rotSpeed);
		}

		if (input_->PressKey(DIK_RIGHT)) {
			position.x += moveSpeed;

			rotVector = CreateRotationVector(
				angleZ, rotSpeed);
		}

		else if (input_->PressKey(DIK_LEFT)) {
			position.x -= moveSpeed;

			rotVector = CreateRotationVector(
				angleZ, -rotSpeed);
		}

		rot += rotVector;

		// ���W�̕ύX�𔽉f
		Object3d::SetRotation(rot);

		// ���W�̕ύX�𔽉f
		Object3d::SetPosition(position);
	}

	camera_->Update();
	Object3d::Update();
}

void Player::Draw() {
	Object3d::Draw();
}

void Player::Finalize() {
}

void Player::OnCollision(const CollisionInfo& info) {
}
