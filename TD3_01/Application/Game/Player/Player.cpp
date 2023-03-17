#include "Player.h"
#include "Quaternion.h"
#include "SafeDelete.h"
#include "Input.h"
#include "SphereCollider.h"

#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Fan.h"

#include <cassert>
#include "CollisionAttribute.h"

CollisionManager* Player::collisionManager_ = CollisionManager::GetInstance();

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

	collider_->SetAttribute(COLLISION_ATTR_PLAYER);

	return true;
}

void Player::Update() {
	Input* input_ = Input::GetInstance();

	// �I�u�W�F�N�g�ړ�

		// ���݂̍��W���擾
		Vector3 position = Object3d::GetPosition();
		// ���݂̍��W���擾
		Vector3 rot = Object3d::GetRotation();

		//�ړ��X�s�[�h
		float moveSpeed = 0.4f;
		//��]�X�s�[�h
		float rotSpeed = ConvertToRadian(90.0f);

		Vector3 angleX = { 1.0f,0.0f,0.0f };
		Vector3 angleY = { 0.0f,1.0f,0.0f };
		Vector3 angleZ = { 0.0f,0.0f,1.0f };

		//�ړ��x�N�g��
		Vector3 moveVector = { 0,0,0 };
		//��]�x�N�g��
		Vector3 rotVector = { 0,0,0 };

		//�ړ���̍��W���v�Z
		//if (input_->TriggerKey(DIK_UP)) {
		//	rotVector = CreateRotationVector(
		//		angleX, rotSpeed);
		//}

		//else if (input_->TriggerKey(DIK_DOWN)) {
		//	rotVector = CreateRotationVector(
		//		angleX, -rotSpeed);
		//}

		if (input_->TriggerKey(DIK_RIGHT)) {
			rotVector = CreateRotationVector(
				angleY, rotSpeed);
		}

		else if (input_->TriggerKey(DIK_LEFT)) {
			rotVector = CreateRotationVector(
				angleY, -rotSpeed);
		}

		moveVector.z = -moveSpeed;

		rot += rotVector;

		moveVector = Vector3Transform(moveVector, worldTransform_.matWorld_);

		position = moveVector;

		// ���W�̕ύX�𔽉f
		Object3d::SetRotation(rot);

		// ���W�̕ύX�𔽉f
		Object3d::SetPosition(position);

	camera_->Update();
	Object3d::Update();

	//RaycastHit raycastHit;

	//���C�L���X�g���`�F�b�N
	//if (collisionManager_->Raycast(fan_->GetRay(),COLLISION_ATTR_PLAYER, &raycastHit)) {
	//worldTransform_.position_.y += 0.1f;
	//	worldTransform_.UpdateMatrix();

	//	Object3d::Update();
	//}
}

void Player::Draw() {
	Object3d::Draw();
}

void Player::Finalize() {
}

void Player::OnCollision(const CollisionInfo& info) {
}
