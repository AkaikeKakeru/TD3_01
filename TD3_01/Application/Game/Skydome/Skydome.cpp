#include "Skydome.h"
#include "SafeDelete.h"
#include "Quaternion.h"
#include "Input.h"

void Skydome::Initialize(Camera* camera) {
	camera_ = camera;

	model_ = new Model();
	model_ = Model::LoadFromOBJ("skydome", false);

	object_ = new Object3d();
	object_ = Object3d::Create();
	object_->SetModel(model_);
	object_->SetScale({ 30, 30, 30 });
	object_->SetCamera(camera_);
}

void Skydome::Update() {
	Input* input_ = Input::GetInstance();

	// オブジェクト回転
	{
		// 現在の座標を取得
		Vector3 rot = object_->GetRotation();

		// 移動後の座標を計算

		//回転軸アングル
		//ここではY軸回転を指定
		Quaternion rotation = MakeAxisAngle(
			{ 0.0f,1.0f,0.0f }, ConvertToRadian(1.0f));

		//1F当たりの回転角度を指定
		Vector3 pointY = { 0.0f,ConvertToRadian(1.0f),0.0f };

		rot += RotateVector(pointY, rotation);

		// 座標の変更を反映
		object_->SetRotation(rot);
	}

	// カメラ移動
	if (input_->PressKey(DIK_W) ||
		input_->PressKey(DIK_S) ||
		input_->PressKey(DIK_D) ||
		input_->PressKey(DIK_A)) {
		if (input_->PressKey(DIK_W)) { camera_->MoveVector({ 0.0f,+1.0f,0.0f }); }
		else if (input_->PressKey(DIK_S)) { camera_->MoveVector({ 0.0f,-1.0f,0.0f }); }
		if (input_->PressKey(DIK_D)) { camera_->MoveVector({ +1.0f,0.0f,0.0f }); }
		else if (input_->PressKey(DIK_A)) { camera_->MoveVector({ -1.0f,0.0f,0.0f }); }
	}

	camera_->Update();
	object_->Update();
}

void Skydome::Draw() {
	object_->Draw();
}

void Skydome::Finalize() {
	SafeDelete(object_);
	SafeDelete(model_);
}
