#pragma once
#include "BaseScene.h"

#include "Input.h"
#include "DrawBasis.h"
#include "Sprite.h"

#include "Audio.h"


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