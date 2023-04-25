#pragma once
#include "BaseScene.h"

#include "Input.h"
#include "DrawBasis.h"
#include "Model.h"
#include "Sprite.h"
#include "Stage.h"
#include "Object3d.h"
#include "Particle.h"
#include "ParticleManager.h"

#include "Camera.h"
#include "LightGroup.h"

#include "ImGuiManager.h"

#include <memory>
#include <List>

#include "Skydome.h"

#include "CollisionPrimitive.h"

class CollisionManager;
class Player;
class Fan;

class GamePlayScene : public BaseScene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	bool CollisionStageFlag(Player* p, Stage* s);
	void ParameterPlayer(const Vector3& playerPos, const int& stageNum);
	void ParamaterFun(const Vector3& fanPos1, const Vector3& fanPos2, const Vector3& fanPos3, const Vector3& fanPos4, const Vector3& fanPos5);
	void ReSetPositionPlayer(const Vector3& playerPos);
	void ReSetPositionFan(const Vector3& fanPos1, const Vector3& fanPos2, const Vector3& fanPos3, const Vector3& fanPos4, const Vector3& fanPos5);

private:
	void Initialize3d();
	void Initialize2d();

	void Update3d();
	void Update2d();

	void Draw3d();
	void DrawParticle();
	void Draw2d();

public://定数
	static const int Vector3Count_ = 3;

	static const int FanCount_ = 5;

private:
	//基盤
	static DirectXBasis* dxBas_;
	//入力
	static Input* input_;
	//スプライト基盤
	static DrawBasis* drawBas_;
	//ImGuiマネージャー
	static ImGuiManager* imGuiManager_;
	
	//カメラ
	Camera* camera_ = nullptr;
	//ライトグループ
	LightGroup* lightGroup_ = nullptr;

	//平行光源の方向初期値
	Vector3 lightDir_ = { 0.0f,-1.0f,1.0f };


	//当たり判定 レイ
	//Ray* ray_;

	//衝突マネージャー
	CollisionManager* collisionManager_ = nullptr;

	bool colRay_ = false;

	Vector3 interRay_ = {};

	bool stageCollision = false;
	/// <summary>
	/// オブジェクト
	/// </summary>

	Stage* stage_ = nullptr;
	const std::string filename_[5] = {
	  "0",
	  "1",
	  "2",
	  "3",
	  "4",
	};

	enum GameScene
	{
		Stage0,
		Stage1,
		Stage2,
		Stage3,
		Stage4,
	};
	size_t scene_ = Stage0;

	Object3d* rayObj_ = nullptr;
	Model* rayModel_ = nullptr;

	Object3d* rayObj_2 = nullptr;

	Player* player_ = nullptr;
	Model* playerModel_ = nullptr;
	
	Fan* fan_[FanCount_] = { nullptr };
	//std::list<Fan> fans_;

	Model* fanModel_ = nullptr;

	Skydome* skydome_ = nullptr;

	Particle* particle1_ = nullptr;
	ParticleManager* pm1_ = nullptr;

	Particle* particle2_ = nullptr;
	ParticleManager* pm2_ = nullptr;
	
	/// <summary>
	/// スプライト
	/// </summary>
	Sprite* sprite_ = new Sprite();

	//ポジション
	Vector3 positionPlayer = { 8.0f,0.0f,20.0f };
	Vector3 positionFan[FanCount_] =
	{
		{ 60.0f,0.0f,50.0f },
		{ 60.0f,0.0f,50.0f},
		{ 60.0f,0.0f,50.0f },
		{ 60.0f,0.0f,50.0f },
		{ 60.0f,0.0f,50.0f },
	};
};