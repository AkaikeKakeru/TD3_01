#include "Skydome.h"
#include "SafeDelete.h"
#include "Quaternion.h"
#include "Input.h"

void Skydome::Initialize(Camera* camera) {
	camera_ = camera;

	model_ = new Model();
	model_ = Model::LoadFromOBJ("skydome", false);

	object_ = new Object3d();
	object_ = Object3d::Create();
	object_->SetModel(model_);
	object_->SetScale({ 30, 30, 30 });
	object_->SetCamera(camera_);
}

void Skydome::Update() {
	Input* input_ = Input::GetInstance();

	// �I�u�W�F�N�g��]
	{
		// ���݂̍��W���擾
		Vector3 rot = object_->GetRotation();

		// �ړ���̍��W���v�Z

		//��]���A���O��
		//�����ł�Y����]���w��
		Quaternion rotation = MakeAxisAngle(
			{ 0.0f,1.0f,0.0f }, ConvertToRadian(1.0f));

		//1F������̉�]�p�x���w��
		Vector3 pointY = { 0.0f,ConvertToRadian(1.0f),0.0f };

		rot += RotateVector(pointY, rotation);

		// ���W�̕ύX�𔽉f
		object_->SetRotation(rot);
	}

	// �J�����ړ�
	if (input_->PressKey(DIK_W) ||
		input_->PressKey(DIK_S) ||
		input_->PressKey(DIK_D) ||
		input_->PressKey(DIK_A)) {
		if (input_->PressKey(DIK_W)) { camera_->MoveVector({ 0.0f,+1.0f,0.0f }); }
		else if (input_->PressKey(DIK_S)) { camera_->MoveVector({ 0.0f,-1.0f,0.0f }); }
		if (input_->PressKey(DIK_D)) { camera_->MoveVector({ +1.0f,0.0f,0.0f }); }
		else if (input_->PressKey(DIK_A)) { camera_->MoveVector({ -1.0f,0.0f,0.0f }); }
	}

	camera_->Update();
	object_->Update();
}

void Skydome::Draw() {
	object_->Draw();
}

void Skydome::Finalize() {
	SafeDelete(object_);
	SafeDelete(model_);
}
