#pragma once
#include "Model.h"
#include "Object3d.h"
#include "Camera.h"

class Player{
public://ƒƒ“ƒoŠÖ”
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private://ƒƒ“ƒo•Ï”
	Object3d* planeObj_ = nullptr;
	Model* planeModel_ = nullptr;
	Camera* camera_player = nullptr;
};