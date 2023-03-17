#include "Fan.h"

#include "Input.h"
#include "SphereCollider.h"

#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include <cassert>
#include <Quaternion.h>

Fan* Fan::Create(Model* model) {
	//オブジェクトのインスタンスを生成
	Fan* instance = new Fan();
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

bool Fan::Initialize() {
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

	collider_->SetAttribute(COLLISION_ATTR_RAY);

	return true;
}

void Fan::Update() {
	assert(&ray_);

	Input* input_ = Input::GetInstance();

	// オブジェクト移動

	// 現在の回転を取得
	Vector3 rot = Object3d::GetRotation();

	//回転スピード
	float verticalAngle = ConvertToRadian(90.0f);

	Vector3 angleX = { 1.0f,0.0f,0.0f };
	Vector3 angleY = { 0.0f,1.0f,0.0f };
	Vector3 angleZ = { 0.0f,0.0f,1.0f };

	Quaternion rotQua = { 0,0,0 };

	//移動後の座標を計算
	if (input_->TriggerKey(DIK_W)) {
		//rotQua = DirectionToDirection(rot, angleY * verticalAngle);
		//rotQua = DirectionToDirection(rot, angleY * (verticalAngle * 2));
		//rotVector_ = RotateVector( angleY ,rotQua );

		rotVector_ = CreateRotationVector(angleY, verticalAngle * 2);
		ray_->dir_ = angleZ;
	}

	else if (input_->TriggerKey(DIK_S)) {
		//rotQua = DirectionToDirection(rot, angleY * -verticalAngle);
		//rot = RotateVector( angleY ,rotQua );

		rotVector_ = CreateRotationVector(angleY, 0);
		ray_->dir_ = -angleZ; //{ -verticalAngle ,0,0 };
	}

	if (input_->TriggerKey(DIK_A)) {
		//rotQua = DirectionToDirection(rot, angleY * verticalAngle);
		//rot = RotateVector( angleY ,rotQua );

		rotVector_ = CreateRotationVector(angleY, verticalAngle);
		ray_->dir_ = -angleX; //{ 0,verticalAngle,0 };
	}

	else if (input_->TriggerKey(DIK_D)) {
		//rotQua = DirectionToDirection(rot, angleY * -verticalAngle);
		//rot = RotateVector( angleY ,rotQua );

		rotVector_ = CreateRotationVector(angleY, -verticalAngle);
		ray_->dir_ = angleX; //{ 0,-verticalAngle,0 };
	}

	rot = rotVector_;

	// 回転の変更を反映
	Object3d::SetRotation(rot);

	ray_->start_ = Object3d::GetPosition();

	camera_->Update();
	Object3d::Update();
}

void Fan::Draw() {
	Object3d::Draw();
}

void Fan::Finalize() {
}

void Fan::OnCollision(const CollisionInfo& info) {
}
