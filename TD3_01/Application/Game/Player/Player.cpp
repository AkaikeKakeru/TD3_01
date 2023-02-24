#include "Player.h"
#include "Quaternion.h"
#include "SafeDelete.h"
#include "Input.h"

void Player::Initialize() {
	camera_player = new Camera();

	planeModel_ = new Model();
	planeModel_ = Model::LoadFromOBJ("plane", true);

	planeObj_ = new Object3d();
	planeObj_ = Object3d::Create();
	planeObj_->SetModel(planeModel_);
	planeObj_->SetScale({ 1, 1, 1 });
	planeObj_->SetRotation(CreateRotationVector(
		{ 0.0f,1.0f,0.0f }, ConvertToRadian(180.0f)));
	planeObj_->SetCamera(camera_player);
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
		Vector3 position = planeObj_->GetPosition();
		// 現在の座標を取得
		Vector3 rot = planeObj_->GetRotation();

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
		planeObj_->SetRotation(rot);

		// 座標の変更を反映
		planeObj_->SetPosition(position);
	}

	camera_player->Update();
	planeObj_->Update();
}

void Player::Draw() {
	planeObj_->Draw();
}

void Player::Finalize() {
	SafeDelete(planeObj_);
	SafeDelete(planeModel_);

	SafeDelete(camera_player);
}
