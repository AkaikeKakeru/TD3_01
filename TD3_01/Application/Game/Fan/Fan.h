#pragma once

#include "Object3d.h"
#include "CollisionPrimitive.h"
#include "Quaternion.h"

#include "Cursor.h"

#include "Stage.h"

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

	void SetFanDirection(const int dirNum);

	//操作フラグのセット
	bool GetIsControl() {
		return isControl_;
	}

	//操作フラグのセット
	void SetIsControl(bool isControl) {
		isControl_ = isControl;
	}


	//ステージのセット
	void SetStage(Stage* stage) {
		stage_ = stage;
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

	//ステージ
	Stage* stage_;
	//ステージのデフォルト座標
	Vector3 defaultFloorPos_;
};