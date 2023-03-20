#pragma once
#include "Model.h"
#include "Object3d.h"
#include "Camera.h"

class CollisionManager;
class Fan;

class Player 
	: public Object3d {
public: //静的メンバ関数
		//オブジェクト生成
	static Player* Create(Model* model = nullptr);

public://メンバ関数
	bool Initialize() override;
	void Update() override;
	void Draw();
	void Finalize();

	//衝突時コールバック関数
	void OnCollision(const CollisionInfo & info) override;
public://アクセッサ
	void SetFan(Fan*fan) {
		fan_ = fan;
	}

	//前進フラグ取得
	bool GetIsRun() {
		return IsRun_;
	}

private:
	//衝突マネージャー
	static CollisionManager* collisionManager_;

	Fan* fan_ = nullptr;

	//前進フラグ
	bool IsRun_ = false;

public:
	Player() = default;
	~Player() = default;
};