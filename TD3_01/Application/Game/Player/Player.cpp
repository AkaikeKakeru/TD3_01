#include "Player.h"
#include "Quaternion.h"
#include "SafeDelete.h"
#include "Input.h"

void Player::Initialize() {
	camera_player = new Camera();

	planeModel_ = new Model();
	planeModel_ = Model::LoadFromOBJ("plane", true);

	planeObj_ = new Object3d();
	planeObj_ = Object3d::Create();
	planeObj_->SetModel(planeModel_);
	planeObj_->SetScale({ 1, 1, 1 });
	planeObj_->SetRotation(CreateRotationVector(
		{ 0.0f,1.0f,0.0f }, ConvertToRadian(180.0f)));
	planeObj_->SetCamera(camera_player);
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
		Vector3 position = planeObj_->GetPosition();
		// ���݂̍��W���擾
		Vector3 rot = planeObj_->GetRotation();

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
		planeObj_->SetRotation(rot);

		// ���W�̕ύX�𔽉f
		planeObj_->SetPosition(position);
	}

	camera_player->Update();
	planeObj_->Update();
}

void Player::Draw() {
	planeObj_->Draw();
}

void Player::Finalize() {
	SafeDelete(planeObj_);
	SafeDelete(planeModel_);

	SafeDelete(camera_player);
}
