#pragma once
#include "Model.h"
#include "Object3d.h"
#include "Camera.h"

class CollisionManager;

class Goal	
	: public Object3d {
public: //静的メンバ関数
		//オブジェクト生成
	static Goal* Create(Model* model = nullptr);

public://メンバ関数
	bool Initialize();
	void Update();
	void Draw();
	void Finalize();

	//衝突時コールバック関数
	void OnCollision(const CollisionInfo & info) override;

	//ゴールフラグ取得
	bool GetIsGoal() {
		return isGoal_;
	}

private://メンバ変数
	Object3d* object_ = nullptr;
	Model* model_ = nullptr;
	Camera* camera_ = nullptr;

private:
	//衝突マネージャー
	static CollisionManager* collisionManager_;

	//ゴールしたかフラグ
	bool isGoal_ = false;

public:
	Goal() = default;
	~Goal() = default;
};
