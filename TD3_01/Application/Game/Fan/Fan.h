#pragma once

#include "Object3d.h"
#include "CollisionPrimitive.h"

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
	Ray GetRay() {
		return ray_;
	}

private://メンバ変数
	Ray ray_;
	//回転ベクトル
	Vector3 rotVector_ = { 0,0,0 };
};