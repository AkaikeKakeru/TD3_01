#include "Skydome.h"
#include "SafeDelete.h"
#include "Quaternion.h"
#include "Input.h"

void Skydome::Initialize(Camera* camera) {
	camera_ = camera;

	model_ = new Model();
	model_ = Model::LoadFromOBJ("skydome", true);

	object_ = new Object3d();
	object_ = Object3d::Create();
	object_->SetModel(model_);
	object_->SetScale({ 512.0f, 512.0f, 512.0f });
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
		Vector3 pointY = { 0.0f,ConvertToRadian(0.1f),0.0f };

		rot += RotateVector(pointY, rotation);

		// ���W�̕ύX�𔽉f
		object_->SetRotation(rot);
	}

	object_->Update();
}

void Skydome::Draw() {
	object_->Draw();
}

void Skydome::Finalize() {
	SafeDelete(object_);
	SafeDelete(model_);
}
