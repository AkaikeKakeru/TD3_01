#include "Skydome.h"
#include "SafeDelete.h"
#include "Quaternion.h"
#include "Input.h"

void Skydome::Initialize() {
	//カメラ生成
	camera_ = new Camera();

	skydomeModel_ = new Model();
	skydomeModel_ = Model::LoadFromOBJ("skydome", false);

	skydomeObj_ = new Object3d();
	skydomeObj_ = Object3d::Create();
	skydomeObj_->SetModel(skydomeModel_);
	skydomeObj_->SetScale({ 30, 30, 30 });
	skydomeObj_->SetCamera(camera_);
}

void Skydome::Update() {
	Input* input_ = Input::GetInstance();

	// オブジェクト回転
	{
		// 現在の座標を取得
		Vector3 rot = skydomeObj_->GetRotation();

		// 移動後の座標を計算

		//回転軸アングル
		//ここではY軸回転を指定
		Quaternion rotation = MakeAxisAngle(
			{ 0.0f,1.0f,0.0f }, ConvertToRadian(1.0f));

		//1F当たりの回転角度を指定
		Vector3 pointY = { 0.0f,ConvertToRadian(1.0f),0.0f };

		rot += RotateVector(pointY, rotation);

		// 座標の変更を反映
		skydomeObj_->SetRotation(rot);
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
	skydomeObj_->Update();
}

void Skydome::Draw() {
	skydomeObj_->Draw();
}

void Skydome::Finalize() {
	SafeDelete(skydomeObj_);
	SafeDelete(skydomeModel_);

	SafeDelete(camera_);
}
