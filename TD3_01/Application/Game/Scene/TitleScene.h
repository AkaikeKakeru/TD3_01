#pragma once
#include "BaseScene.h"

#include "Input.h"
#include "DrawBasis.h"
#include "Model.h"
#include "Sprite.h"
#include "Object3d.h"

#include "Camera.h"
#include "LightGroup.h"

#include "SceneManager.h"

#include "ParticleManager.h"

class TitleScene : public BaseScene{
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
	/// <summary>
	/// オブジェクト
	/// </summary>
	/// <summary>
	Object3d* planeObj_ = nullptr;
	Model* planeModel_ = nullptr;

	Object3d* skydomeObj_ = nullptr;
	Model* skydomeModel_ = nullptr;
	/// スプライト
	/// </summary>
	Sprite* sprite_ = new Sprite();

	//パーティクルマネージャー
	ParticleManager* particleMan_ = nullptr;
	int particleNum_ = 0;
};