#include "Switch.h"

void Switch::Initialize(Model* model, Object3d* obj) {
	//3Dオブジェクト読み込み
	obj_ = obj;
	// モデル読み込み
	model_ = model;

	//スイッチが押されているか
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
