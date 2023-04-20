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

	static const int FanCount_ = 3;

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
	const std::string filename_[8] = {
	  "0",
	  "1",
	  "2",
	  "3",
	  "4",
	  "5",
	  "6",
	  "7" };

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

};