#pragma once

#include "Object3d.h"
#include "CollisionPrimitive.h"
#include "Quaternion.h"
#include "Particle.h"
#include "ParticleManager.h"


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
	void DrawWind();
	void Finalize();

	//衝突時コールバック関数
	void OnCollision(const CollisionInfo& info) override;

	//照準
	void Reticle();
	//風
	void ActiveWind(const int dir);

public://アクセッサ
	Ray* GetRay() {
		return ray_;
	}

	void SetRay(Ray* ray) {
		ray_ = ray;
	}

	void SetFanDirection(const int dirNum);

	//操作フラグのセット
	void SetIsControl(bool isControl) {
		isControl_ = isControl;
	}

	int GetFanDirection() { return direction_; }
	void SetDirection(const int dir) { this->direction_ = dir; }
private://メンバ変数
	//向き
	size_t direction_ = Up;
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

	//風
	Particle* wind_ = nullptr;
	ParticleManager* windpm_ = nullptr;

};