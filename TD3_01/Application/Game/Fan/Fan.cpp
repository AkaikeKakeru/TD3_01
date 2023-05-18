#include "Fan.h"

#include "Input.h"
#include "SphereCollider.h"

#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include <cassert>
#include <WinApp.h>

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

	ray_ = new Ray();
	ray_->start_ = Object3d::GetPosition();
	ray_->dir_ = Object3d::GetRotation();

	//コライダ－追加
	float radius = 0.6f;
	//半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(
		Vector3{ 0.0f,radius,0.0f },
		radius)
	);

	collider_->SetAttribute(COLLISION_ATTR_RAY);

	isControl_ = false;
	isGrab_ = false;

	worldTransform3dReticle_.Initialize();

	stage_ = nullptr;

	return true;
}

void Fan::Update() {
	assert(&ray_);

	Input* input_ = Input::GetInstance();

	// オブジェクト移動
	// 現在の座標を取得
	Vector3 move = Object3d::GetPosition();

	//移動スピード
	float moveSpeed = 0.4f;

	// 現在の回転を取得
	Vector3 rot = Object3d::GetRotation();

	if (isControl_) {
		//回転スピード(垂直)
		float verticalAngle = ConvertToRadian(90.0f);

		Vector3 angleX = { 1.0f,0.0f,0.0f };
		Vector3 angleY = { 0.0f,1.0f,0.0f };
		Vector3 angleZ = { 0.0f,0.0f,1.0f };

		Quaternion rotQua = { 0,0,0 };

		Reticle();

		if (input_->TriggerMouse(0)) {
			if (Object3d::GetPosition().x < worldTransform3dReticle_.position_.x + 10.0f &&
				Object3d::GetPosition().x > worldTransform3dReticle_.position_.x - 10.0f &&
				Object3d::GetPosition().z < worldTransform3dReticle_.position_.z + 10.0f &&
				Object3d::GetPosition().z > worldTransform3dReticle_.position_.z - 10.0f) {
				isGrab_ = true;
			}
		}

		if (isGrab_) {
			if (input_->PressMouse(0)) {
				Object3d::SetPosition(worldTransform3dReticle_.position_);
			}
			else {
				//再計算
				Vector3 localPos = Object3d::GetPosition();

				Vector3 localPosRema = {
					static_cast<float>(static_cast<int>(localPos.x) % static_cast<int>(stage_->GetRadius())),
					localPos.y,
					static_cast<float>(static_cast<int>(localPos.z) % static_cast<int>(stage_->GetRadius()))
				};

				localPos = {
					localPos.x - localPosRema.x,
					localPos.y,
					localPos.z - localPosRema.z
				};

				Object3d::SetPosition(localPos);

				isGrab_ = false;
			}
		}

		if (isGrab_) {
			//移動後の座標を計算
			if (input_->TriggerKey(DIK_W)) {
				rotVector_ = CreateRotationVector(angleY, verticalAngle * 2);

				rot = angleY * verticalAngle * 2;
				direction_ = Up;
				ray_->dir_ = angleZ;
			}

			else if (input_->TriggerKey(DIK_S)) {
				rotVector_ = CreateRotationVector(angleY, 0);

				rot = angleY * verticalAngle * 0;
				direction_ = Down;
				ray_->dir_ = -angleZ; //{ -verticalAngle ,0,0 };
			}

			if (input_->TriggerKey(DIK_A)) {
				rotVector_ = CreateRotationVector(angleY, verticalAngle);

				rot = angleY * verticalAngle;
				direction_ = Left;
				ray_->dir_ = -angleX; //{ 0,verticalAngle,0 };
			}

			else if (input_->TriggerKey(DIK_D)) {
				rotVector_ = CreateRotationVector(angleY, -verticalAngle);

				rot = angleY * -verticalAngle;
				direction_ = Right;
				ray_->dir_ = angleX; //{ 0,-verticalAngle,0 };
			}


			// 回転の変更を反映
			Object3d::SetRotation(rot);
		}
	}

	ray_->start_ = Object3d::GetPosition();
	camera_->Update();

	Object3d::Update();
}

void Fan::Draw() {
	Object3d::Draw();
}


void Fan::Finalize() {
	delete ray_;

}

void Fan::OnCollision(const CollisionInfo& info) {
}

void Fan::Reticle() {
	assert(camera_);

	cursor_.SetCamera(camera_);

	worldTransform3dReticle_.position_ =
		cursor_.Get3DRethiclePosition();


	int surplusX = static_cast<int>(worldTransform3dReticle_.position_.x) % 8;
	int surplusY = static_cast<int>(worldTransform3dReticle_.position_.y) % 8;
	int surplusZ = static_cast<int>(worldTransform3dReticle_.position_.z) % 8;

	worldTransform3dReticle_.position_ = {
		static_cast<float>((static_cast<int>(worldTransform3dReticle_.position_.x) - surplusX)),
		static_cast<float>((static_cast<int>(worldTransform3dReticle_.position_.y) - surplusY)),
		static_cast<float>((static_cast<int>(worldTransform3dReticle_.position_.z) - surplusZ))
	};
}

void Fan::SetFanDirection(const int dirNum) {
	this->direction_ = dirNum;
	//回転スピード(垂直)
	static const float verticalAngle = ConvertToRadian(90.0f);

	static const Vector3 angleX = { 1.0f,0.0f,0.0f };
	static const Vector3 angleY = { 0.0f,1.0f,0.0f };
	static const Vector3 angleZ = { 0.0f,0.0f,1.0f };

	Vector3 fanAngle = angleY;
	float fanRotaSpeed = 0.0f;
	Vector3 rayDir{};


	switch (direction_) {
	case Up:
		fanRotaSpeed = verticalAngle * 2;
		rayDir = angleZ;
		break;

	case Down:
		fanRotaSpeed = verticalAngle * 0;
		rayDir = -angleZ;
		break;

	case Right:
		fanRotaSpeed = -verticalAngle;
		rayDir = angleX;
		break;

	case Left:
		fanRotaSpeed = verticalAngle;
		rayDir = -angleX;
		break;

	default:
		fanRotaSpeed = ConvertToRadian(30.0f);
		rayDir = angleZ;
		break;
	}

	// 現在の回転を取得
	Vector3 rot = Object3d::GetRotation();

	rotVector_ = CreateRotationVector(fanAngle, fanRotaSpeed);

	rot = fanAngle * fanRotaSpeed;

	// 回転の変更を反映
	Object3d::SetRotation(rot);

	ray_->start_ = Object3d::GetPosition();
	ray_->dir_ = rayDir;
}
