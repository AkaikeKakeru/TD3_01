#pragma once
#include "Model.h"
#include "Object3d.h"
#include "Camera.h"

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

private:
	Player() = default;
	~Player() = default;
};