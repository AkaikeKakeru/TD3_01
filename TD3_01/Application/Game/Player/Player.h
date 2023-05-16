#pragma once
#include "Model.h"
#include "Object3d.h"
#include "Camera.h"

#include "Quaternion.h"

class CollisionManager;
class Fan;

class Player 
	: public Object3d {
public: //静的メンバ関数
		//オブジェクト生成
	static Player* Create(Model* model = nullptr);

public: //enum
	typedef enum Direction {
		Up = 0,
		Down = 1,
		Right = 2,
		Left = 3,
	} Direction;

public://メンバ関数
	bool Initialize() override;
	void Update() override;
	void Draw();
	void Finalize();

	//衝突時コールバック関数
	void OnCollision(const CollisionInfo & info) override;

	void OnCollisionStage(const bool& collisionFlag, const Vector3& pos);
public://アクセッサ
	void SetFan(Fan*fan) {
		fan_ = fan;
	}

	//前進フラグ取得
	bool GetIsRun() {
		return IsRun_;
	}
	void Stop(const Vector3& pos,const float direction);
	const Vector3& GetPosition() const { return worldTransform_.position_; }
	float GetRadius() const { return radius_; }

	void SetDirection(const int dirNum) {
		//回転スピード(垂直)
		static const float verticalAngle = ConvertToRadian(90.0f);

		static const Vector3 angleX = { 1.0f,0.0f,0.0f };
		static const Vector3 angleY = { 0.0f,1.0f,0.0f };
		static const Vector3 angleZ = { 0.0f,0.0f,1.0f };

		Vector3 angle = angleY;
		float rotaSpeed = 0.0f;

		switch (dirNum) {
		case Up:
			rotaSpeed = verticalAngle * 2;
			break;

		case Down:
			rotaSpeed = verticalAngle * 0;
			break;

		case Right:
			rotaSpeed = -verticalAngle;
			break;

		case Left:
			rotaSpeed = verticalAngle;
			break;

		default:
			rotaSpeed = ConvertToRadian(30.0f);
			break;
		}

		// 現在の回転を取得
		Vector3 rot = Object3d::GetRotation();

		rotVector_ = CreateRotationVector(angle, rotaSpeed);

		rot = angle * rotaSpeed;

		// 回転の変更を反映
		Object3d::SetRotation(rot);
	}
	//開始時方向ゲッタ
	float GetStartDirection() {
		return startDir_;
	}

	//開始時方向セッタ
	void SetStartDirection(float startDir) {
		startDir_ = startDir;
	}
private:
	//衝突マネージャー
	static CollisionManager* collisionManager_;

	Fan* fan_ = nullptr;

	//前進フラグ
	bool IsRun_ = false;

	//WorldTransform worldtransform_;
	// 前フレーム座標
	Vector3 prePos_{};
	//半径
	float radius_ = 1.0f;

	//回転ベクトル
	Vector3 rotVector_ = { 0,0,0 };

	//開始時方向
	float startDir_ = Up;
public:
	Player() = default;
	~Player() = default;
};