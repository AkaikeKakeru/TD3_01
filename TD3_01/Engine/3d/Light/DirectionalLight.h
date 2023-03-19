#pragma once
#include "Vector3.h"

class DirectionalLight {
public://�T�u�N���X
	struct ConstBufferData{
		Vector3 lightv_; //���C�g�ւ̕����x�N�g��
		float pad1_;
		Vector3 lightcolor_;//���C�g�̐F
		unsigned int active_;//�L���t���O
	};

	//���C�g�������擾
	const Vector3& GetLightDir() {
		return lightdir_;
	}

	//���C�g�F���擾
	const Vector3&  GetLightColor() {
		return lightcolor_;
	}

	//���C�g�������Z�b�g
	void SetLightDir(const Vector3& lightdir) {
		//���K�����ăZ�b�g
		lightdir_ = Vector3Normalize(lightdir);
	}

	//���C�g�F���Z�b�g
	void SetLightColor(const Vector3& lightcolor) {
		lightcolor_ = lightcolor;
	}

	//�L���t���O���Z�b�g
	inline void SetActive(bool active) { active_ = active; }

	//�L���`�F�b�N
	inline bool IsActive() { return active_; }

private://�����o�ϐ�
	//���C�g��������(�P�ʃx�N�g��)
	Vector3 lightdir_ = { 1,0,0 };
	//���C�g�F
	Vector3 lightcolor_ = { 1,1,1 };
	//�L���t���O
	bool active_ = false;
};