#pragma once
#include "Model.h"
#include "Object3d.h"
#include "Camera.h"

class Skydome{
public://�����o�֐�
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private://�����o�ϐ�
	Object3d* skydomeObj_ = nullptr;
	Model* skydomeModel_ = nullptr;
	Camera* camera_ = nullptr;
};