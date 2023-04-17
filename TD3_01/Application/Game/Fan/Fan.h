#pragma once

#include "Object3d.h"
#include "CollisionPrimitive.h"
#include "Quaternion.h"

#include "Cursor.h"

class Fan
	: public Object3d {
public: //enum
	typedef enum Direction {
		Up = 0,
		Down = 1,
		Right = 2,
		Left = 3,
	} Direction;

public: //静的メンバ関数
	static Fan* Create(Model* model);
public: //メンバ関数
	bool Initialize() override;
	void Update() override;
	void Draw();
	void Finalize();

	//衝突時コールバック関数
	void OnCollision(const CollisionInfo& info) override;

	//照準
	void Reticle();

public://アクセッサ
	Ray* GetRay() {
		return ray_;
	}

	void SetRay(Ray* ray) {
		ray_ = ray;
	}

	void SetFanDirection(const int dirNum) {
		//回転スピード(垂直)
		static const float verticalAngle = ConvertToRadian(90.0f);

		static const Vector3 angleX = { 1.0f,0.0f,0.0f };
		static const Vector3 angleY = { 0.0f,1.0f,0.0f };
		static const Vector3 angleZ = { 0.0f,0.0f,1.0f };

		Vector3 fanAngle = angleY;
		float fanRotaSpeed = 0.0f;
		Vector3 rayDir{};

		switch (dirNum) {
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

	//操作フラグのセット
	void SetIsControl(bool isControl) {
		isControl_ = isControl;
	}

private://メンバ変数
	Ray* ray_ = nullptr;
	//回転ベクトル
	Vector3 rotVector_ = { 0,0,0 };

	//操作フラグ
	bool isControl_ = false;

	//掴みフラグ
	bool isGrab_= false;

	//レティクル用
	WorldTransform worldTransform3dReticle_;

	//レティクルカーソル
	Cursor cursor_;
};