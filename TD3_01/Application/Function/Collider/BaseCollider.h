#pragma once

#include "CollisionTypes.h"
#include "Object3d.h"

class BaseCollider {
public: //メンバ関数
	virtual void Update() = 0;

public: //アクセッサ
	//オブジェクトセット
	inline void SetObject3d(Object3d* object) {
		object3d_ = object; }
	//オブジェクト取得
	inline Object3d* GetObject3d() {
		return object3d_; }

	//形状タイプ取得
	inline CollisionShapeType GetShapeType() {
		return shapeType_; }

protected: //メンバ変数
	Object3d* object3d_ = nullptr;
	//形状タイプ
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

public: //コンストデスト
	BaseCollider() = default;
	virtual ~BaseCollider() = default;

};