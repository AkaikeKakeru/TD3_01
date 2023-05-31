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

#include "Audio.h"

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
	void ParameterPlayer(const Vector3& playerPos, const float direction, const int& stageNum);
	void ParamaterFun(const Vector3& fanPos1, const Vector3& fanPos2, const Vector3& fanPos3, const Vector3& fanPos4, const Vector3& fanPos5);
	void ReSetPositionPlayer(const Vector3& playerPos);
	void ReSetPositionFan(const Vector3& fanPos1, const Vector3& fanPos2, const Vector3& fanPos3, const Vector3& fanPos4, const Vector3& fanPos5);
	//風
	void ActiveWind(const int dir, const Vector3& position);

	
private:
	void Initialize3d();
	void Initialize2d();

	void Update3d();
	void Update2d();

	void Draw3d();
	void DrawParticle();
	void Draw2d();

	//カメラ
	void ClearCameraMove();
	void ResetCameraMove();

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
	const std::string filename_[6] = {
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
	};

	enum GameScene
	{
		Stage0,
		Stage1,
		Stage2,
		Stage3,
		Stage4,
		Stage5,
	};
	size_t scene_ = Stage0;

	Player* player_ = nullptr;
	Model* playerModel_ = nullptr;

	Fan* fan_[FanCount_] = { nullptr };
	//std::list<Fan> fans_;

	Model* fanModel_ = nullptr;
	Model* fanModelConst_ = nullptr;

	Skydome* skydome_ = nullptr;

	Particle* particle1_ = nullptr;
	ParticleManager* pm1_ = nullptr;

	Particle* particle2_ = nullptr;
	ParticleManager* pm2_ = nullptr;
	//風
	Particle* wind_ = nullptr;
	ParticleManager* windpm_ = nullptr;
	
	//音
	Audio* audio_ = nullptr;
	Audio::SoundData stageBGM;
	Audio::SoundData hitSE;
	Audio::SoundData doneSE;
	Audio::SoundData clearSE;
	Audio::SoundData resetSE;
	Audio::SoundData backSE;
	Audio::SoundData runSE;
	Audio::SoundData windSE;

	/// <summary>
	/// スプライト
	/// </summary>
	Sprite* spritePause_ = new Sprite();
	Sprite* spritePauseInfo_ = new Sprite();
	Sprite* spriteResetInfo_ = new Sprite();

	Sprite* rule_ = new Sprite();
	Sprite* rule2_ = new Sprite();
	Sprite* really_ = new Sprite();
	Sprite* stageClear_ = new Sprite();

	//ポジション
	Vector3 positionPlayer = { 4.0f,0.0f,18.0f };
	Vector3 positionFan[FanCount_] =
	{
		{ 60.0f,0.0f,50.0f },
		{ 60.0f,0.0f,50.0f},
		{ 60.0f,0.0f,50.0f },
		{ 60.0f,0.0f,50.0f },
		{ 60.0f,0.0f,50.0f },
	};
	//クリアしたか
	bool isClear_ = false;

	//リセットしたか
	bool isReset_ = false;


	//固定カメラ
	Camera cameraFixed_;

	//カメララープのデフォルト時間
	float defTime_ = 30;
	//カメララープの経過時間
	float timeCameraLerpProgress_ = defTime_;

	//プレイヤーが画面外に出たであろうと判断できる範囲を定数化
	const int Num_OffScreen_Multi_ = 8;

	//プレイヤー強制停止フラグ
	bool isStopForcedPlayer_ = false;
	//プレイヤー強制停止フラグ
	float timeMovingPlayerProgress_ = defTime_ * Num_OffScreen_Multi_;

	//カメララープの実行中フラグ
	bool isCameraLerp_ = false;
	//カメララープの終わりフラグ
	bool isEndCameraLerp_ = false;

	//ポーズしたか
	bool isPause_ = false;
	//タイトルに戻るか
	bool isReally_ = false;
	//ルール説明か
	bool isrule_ = false;
	int ruleCount = 0;
};