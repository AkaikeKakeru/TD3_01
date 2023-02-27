#pragma once
#include "Model.h"
#include "Object3d.h"
#include "Camera.h"

class Player 
	: public Object3d {
public: //�ÓI�����o�֐�
		//�I�u�W�F�N�g����
	static Player* Create(Model* model = nullptr);

public://�����o�֐�
	bool Initialize() override;
	void Update() override;
	void Draw();
	void Finalize();

	//�Փˎ��R�[���o�b�N�֐�
	void OnCollision(const CollisionInfo & info) override;

private:
	Player() = default;
	~Player() = default;
};