#pragma once
#include "BaseScene.h"

#include "Input.h"
#include "DrawBasis.h"
#include "Model.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Particle.h"
#include "ParticleManager.h"

#include "Camera.h"
#include "LightGroup.h"
#include "Audio.h"

#include "SceneManager.h"

class GameClearScene : public BaseScene {
public://構造体

public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
private:
	static DirectXBasis* dxBas_;
	static Input* input_;
	DrawBasis* drawBas_ = nullptr;

	Camera* camera_ = nullptr;
	LightGroup* lightGroup_ = nullptr;

	//切り替えフラグ
	bool change = false;
	
	//オブジェクト
	Object3d* planeObj_ = nullptr;
	Model* planeModel_ = nullptr;

	// スプライト
	Sprite* sprite_ = new Sprite();
	Sprite* spriteBack_ = new Sprite();

	//パーティクル
	Particle* particle1_ = nullptr;
	ParticleManager* pm1_ = nullptr;
	Vector3 rot = { 0.0f,0.0f,0.0f };

	Audio* audio_ = nullptr;
	Audio::SoundData clearBGM;

};