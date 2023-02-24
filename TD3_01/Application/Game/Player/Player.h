#pragma once
#include "Model.h"
#include "Object3d.h"
#include "Camera.h"

class Player{
public://�����o�֐�
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private://�����o�ϐ�
	Object3d* object_ = nullptr;
	Model* model_ = nullptr;
	Camera* camera_ = nullptr;
};