#include "Skydome.h"
#include "SafeDelete.h"
#include "Quaternion.h"
#include "Input.h"

void Skydome::Initialize() {
	//�J��������
	camera_ = new Camera();

	skydomeModel_ = new Model();
	skydomeModel_ = Model::LoadFromOBJ("skydome", false);

	skydomeObj_ = new Object3d();
	skydomeObj_ = Object3d::Create();
	skydomeObj_->SetModel(skydomeModel_);
	skydomeObj_->SetScale({ 30, 30, 30 });
	skydomeObj_->SetCamera(camera_);
}

void Skydome::Update() {
	Input* input_ = Input::GetInstance();

	// �I�u�W�F�N�g��]
	{
		// ���݂̍��W���擾
		Vector3 rot = skydomeObj_->GetRotation();

		// �ړ���̍��W���v�Z

		//��]���A���O��
		//�����ł�Y����]���w��
		Quaternion rotation = MakeAxisAngle(
			{ 0.0f,1.0f,0.0f }, ConvertToRadian(1.0f));

		//1F������̉�]�p�x���w��
		Vector3 pointY = { 0.0f,ConvertToRadian(1.0f),0.0f };

		rot += RotateVector(pointY, rotation);

		// ���W�̕ύX�𔽉f
		skydomeObj_->SetRotation(rot);
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
	skydomeObj_->Update();
}

void Skydome::Draw() {
	skydomeObj_->Draw();
}

void Skydome::Finalize() {
	SafeDelete(skydomeObj_);
	SafeDelete(skydomeModel_);

	SafeDelete(camera_);
}
