#include "Switch.h"

void Switch::Initialize(Model* model, Object3d* obj) {
	//3D�I�u�W�F�N�g�ǂݍ���
	obj_ = obj;
	// ���f���ǂݍ���
	model_ = model;

	//�X�C�b�`��������Ă��邩
	isFlag_ = false;
}

void Switch::Update() {
	isFlag_ = false;
}

void Switch::Draw() {
	obj_->Draw(worldTransform_);
}

void Switch::OnCollisionSwitch() {
	isFlag_ = true;
}
