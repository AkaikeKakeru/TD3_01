#pragma once
#include "Model.h"
#include "Object3d.h"
#include "Camera.h"

class Skydome{
public://ƒƒ“ƒoŠÖ”
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private://ƒƒ“ƒo•Ï”
	Object3d* skydomeObj_ = nullptr;
	Model* skydomeModel_ = nullptr;
	Camera* camera_ = nullptr;
};