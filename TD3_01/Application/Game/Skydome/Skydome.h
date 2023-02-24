#pragma once
#include "Model.h"
#include "Object3d.h"
#include "Camera.h"

class Skydome{
public://メンバ関数
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private://メンバ変数
	Object3d* skydomeObj_ = nullptr;
	Model* skydomeModel_ = nullptr;
	Camera* camera_ = nullptr;
};