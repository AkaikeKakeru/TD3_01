#pragma once
#include "Model.h"
#include "Object3d.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class Switch {
public:
	// 初期化
	void Initialize(Model* model, Object3d* obj);

	// 更新
	void Update();

	// 描画
	void Draw();

	// スイッチオン
	void OnCollisionSwitch();

private:
	// モデル
	Object3d* obj_ = nullptr;
	Model* model_ = nullptr;
	// テクスチャハンドル
	int32_t textureHandle_ = 0u;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// フラグ
	bool isFlag_ = false;

public: // アクセッサ
	bool GetFlag() { return isFlag_; }
	void SetPosition(Vector3 pos) {
		// 初期化
		worldTransform_.Initialize();
		// スケール設定
		worldTransform_.scale_ = { 2.0f, 2.0f, 2.0f };
		// 座標設定
		worldTransform_.position_ = pos;
		// 行列更新
		worldTransform_.matWorld_ = Matrix4Identity();
		worldTransform_.matWorld_ *= Matrix4WorldTransform(worldTransform_.scale_,worldTransform_.rotation_,worldTransform_.position_);
		worldTransform_.UpdateMatrix();
		obj_->SetWorldTransform(worldTransform_);
	}
};
