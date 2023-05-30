#pragma once
#include "BaseScene.h"

#include "Input.h"
#include "DrawBasis.h"
#include "Model.h"
#include "Sprite.h"
#include "Object3d.h"

#include "Camera.h"
#include "LightGroup.h"

#include "Audio.h"

#include "SceneManager.h"

class TitleScene : public BaseScene {
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
	//音
	Audio* audio_ = nullptr;
	Audio::SoundData titleBGM;
	Audio::SoundData doneSE;
	/// スプライト
	/// </summary>
	Sprite* sprite_ = new Sprite();
	Sprite* sprite2_ = new Sprite();
	Sprite* sprite3_ = new Sprite();
	bool spriteChange = false;
	int ruleCount = 0;
};