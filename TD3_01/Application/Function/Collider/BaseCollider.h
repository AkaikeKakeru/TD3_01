#pragma once

#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include "Object3d.h"

class BaseCollider {
public: //メンバ関数
	virtual void Update() = 0;

public: //アクセッサ
	//オブジェクトセット
	inline void SetObject(Object3d* object) {
		object_ = object; }
	//オブジェクト取得
	inline Object3d* GetObject3d() {
		return object_; }

	//形状タイプ取得
	inline CollisionShapeType GetShapeType() {
		return shapeType_; }

	//衝突時コールバック関数
	inline void OnCollision(const CollisionInfo& info) {
		object_->OnCollision(info);
	}

protected: //メンバ変数
	Object3d* object_ = nullptr;
	//形状タイプ
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

public: //コンストデスト
	BaseCollider() = default;
	virtual ~BaseCollider() = default;

};