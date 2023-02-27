#include "Player.h"
#include "Quaternion.h"
#include "SafeDelete.h"
#include "Input.h"
#include "SphereCollider.h"
#include <cassert>

Player* Player::Create(Model* model) {
	//オブジェクトのインスタンスを生成
	Player* instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	//モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

bool Player::Initialize() {
	if (!Object3d::Initialize()) {
		return false;
	}

	//コライダ－追加
	float radius = 0.6f;
	//半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(
		Vector3{ 0.0f,radius,0.0f },
		radius)
	);

	return true;
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
		Vector3 position = Object3d::GetPosition();
		// 現在の座標を取得
		Vector3 rot = Object3d::GetRotation();

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
		Object3d::SetRotation(rot);

		// 座標の変更を反映
		Object3d::SetPosition(position);
	}

	camera_->Update();
	Object3d::Update();
}

void Player::Draw() {
	Object3d::Draw();
}

void Player::Finalize() {
}

void Player::OnCollision(const CollisionInfo& info) {
}
