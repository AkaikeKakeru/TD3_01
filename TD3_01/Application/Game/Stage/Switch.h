#pragma once
#include "Model.h"
#include "Object3d.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class Switch {
public:
	// ������
	void Initialize(Model* model, Object3d* obj);

	// �X�V
	void Update();

	// �`��
	void Draw();

	// �X�C�b�`�I��
	void OnCollisionSwitch();

private:
	// ���f��
	Object3d* obj_ = nullptr;
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	int32_t textureHandle_ = 0u;

	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	// �t���O
	bool isFlag_ = false;

public: // �A�N�Z�b�T
	bool GetFlag() { return isFlag_; }
	void SetPosition(Vector3 pos) {
		// ������
		worldTransform_.Initialize();
		// �X�P�[���ݒ�
		worldTransform_.scale_ = { 2.0f, 2.0f, 2.0f };
		// ���W�ݒ�
		worldTransform_.position_ = pos;
		// �s��X�V
		worldTransform_.matWorld_ = Matrix4Identity();
		worldTransform_.matWorld_ *= Matrix4WorldTransform(worldTransform_.scale_,worldTransform_.rotation_,worldTransform_.position_);
		worldTransform_.UpdateMatrix();
		obj_->SetWorldTransform(worldTransform_);
	}
};
