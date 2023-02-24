#include "Player.h"
#include "Quaternion.h"
#include "SafeDelete.h"
#include "Input.h"

void Player::Initialize() {
	camera_ = new Camera();

	model_ = new Model();
	model_ = Model::LoadFromOBJ("plane", true);

	object_ = new Object3d();
	object_ = Object3d::Create();
	object_->SetModel(model_);
	object_->SetScale({ 1, 1, 1 });
	object_->SetRotation(CreateRotationVector(
		{ 0.0f,1.0f,0.0f }, ConvertToRadian(180.0f)));
	object_->SetCamera(camera_);
}

void Player::Update() {
	Input* input_ = Input::GetInstance();

	// オブジェクト移動
	if (input_->PressKey(DIK_UP) ||
		input_->PressKey(DIK_DOWN) ||
		input_->PressKey(DIK_RIGHT) ||
		input_->PressKey(DIK_LEFT))
	{
		// 現在の座標を取得
		Vector3 position = object_->GetPosition();
		// 現在の座標を取得
		Vector3 rot = object_->GetRotation();

		//回転ベクトル
		Vector3 rotVector = {};

		//移動スピード
		float moveSpeed = 0.5f;
		//回転スピード
		float rotSpeed = ConvertToRadian(1.0f);

		Vector3 angleX = { 1.0f,0.0f,0.0f };
		Vector3 angleZ = { 0.0f,0.0f,1.0f };

		//移動後の座標を計算
		if (input_->PressKey(DIK_UP)) {
			// 移動後の座標を計算
			position.y += moveSpeed;

			rotVector = CreateRotationVector(
				angleX, rotSpeed);
		}

		else if (input_->PressKey(DIK_DOWN)) {
			position.y -= moveSpeed;

			rotVector = CreateRotationVector(
				angleX, -rotSpeed);
		}

		if (input_->PressKey(DIK_RIGHT)) {
			position.x += moveSpeed;

			rotVector = CreateRotationVector(
				angleZ, rotSpeed);
		}

		else if (input_->PressKey(DIK_LEFT)) {
			position.x -= moveSpeed;

			rotVector = CreateRotationVector(
				angleZ, -rotSpeed);
		}

		rot += rotVector;

		// 座標の変更を反映
		object_->SetRotation(rot);

		// 座標の変更を反映
		object_->SetPosition(position);
	}

	camera_->Update();
	object_->Update();
}

void Player::Draw() {
	object_->Draw();
}

void Player::Finalize() {
	SafeDelete(object_);
	SafeDelete(model_);

	SafeDelete(camera_);
}
