#include "Skydome.h"
#include "SafeDelete.h"
#include "Quaternion.h"
#include "Input.h"

void Skydome::Initialize(Camera* camera) {
	camera_ = camera;

	model_ = new Model();
	model_ = Model::LoadFromOBJ("skydome", true);

	object_ = new Object3d();
	object_ = Object3d::Create();
	object_->SetModel(model_);
	object_->SetScale({ 512.0f, 512.0f, 512.0f });
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
		Vector3 pointY = { 0.0f,ConvertToRadian(0.1f),0.0f };

		rot += RotateVector(pointY, rotation);

		// 座標の変更を反映
		object_->SetRotation(rot);
	}

	object_->Update();
}

void Skydome::Draw() {
	object_->Draw();
}

void Skydome::Finalize() {
	SafeDelete(object_);
	SafeDelete(model_);
}
