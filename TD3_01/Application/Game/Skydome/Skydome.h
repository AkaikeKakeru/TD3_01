#pragma once
#include "Model.h"
#include "Object3d.h"
#include "Camera.h"

class Skydome{
public://ƒƒ“ƒoŠÖ”
	void Initialize(Camera* camera);
	void Update();
	void Draw();
	void Finalize();

private://ƒƒ“ƒo•Ï”
	Object3d* object_ = nullptr;
	Model* model_ = nullptr;
	Camera* camera_ = nullptr;
};