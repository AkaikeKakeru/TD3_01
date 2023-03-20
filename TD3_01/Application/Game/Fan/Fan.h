#pragma once

#include "Object3d.h"
#include "CollisionPrimitive.h"
#include "Quaternion.h"

class Fan 
	: public Object3d{
public: //静的メンバ関数
	static Fan* Create(Model* model);
public: //メンバ関数
	bool Initialize() override;
	void Update() override;
	void Draw();
	void Finalize();

	//衝突時コールバック関数
	void OnCollision(const CollisionInfo & info) override;

public://アクセッサ
	Ray* GetRay() {
		return ray_;
	}

	void SetRay(Ray* ray) {
		ray_ = ray;
	}

	void SetFanDirection(Vector3 fanAngle,
		float fanRotaSpeed,
		Vector3 rayDir) {
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
	Ray* ray_;
	//回転ベクトル
	Vector3 rotVector_ = { 0,0,0 };

	//操作フラグ
	bool isControl_ = false;
};