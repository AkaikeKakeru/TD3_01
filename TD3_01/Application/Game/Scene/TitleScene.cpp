#include "TitleScene.h"
#include "SafeDelete.h"
#include "ParticleManager.h"

#include "Framework.h"
#include "SceneManager.h"

DirectXBasis* TitleScene::dxBas_ = DirectXBasis::GetInstance();
Input* TitleScene::input_ = Input::GetInstance();

void TitleScene::Initialize() {
	/// 描画初期化
	audio_ = Audio::GetInstance();
	audio_->Initialize();

	//描画基盤
	drawBas_ = DrawBasis::GetInstance();
	drawBas_->Initialize();

	drawBas_->LoadTexture(0, "title.png");
	drawBas_->LoadTexture(1, "tutorial.png");
	drawBas_->LoadTexture(2, "tutorial2.png");

	//描画スプライト
	sprite_->Initialize(drawBas_, 0);
	sprite2_->Initialize(drawBas_, 1);
	sprite3_->Initialize(drawBas_, 2);
	
	//音
	titleBGM = audio_->SoundLoadWave("Resource/sound/title.wav");
	doneSE = audio_->SoundLoadWave("Resource/sound/done.wav");
	audio_->SoundPlayWave(audio_->GetXAudio2().Get(), titleBGM, true);

	spriteChange = false;
	ruleCount = 0;
}

void TitleScene::Update() {
	input_->Update();

	sprite_->Update();

	if (input_->TriggerKey(DIK_SPACE))
	{
		if (!spriteChange)audio_->SoundPlayWave(audio_->GetXAudio2().Get(), doneSE, false);
		spriteChange = true;
	}
	if (spriteChange)
	{
		if (input_->TriggerKey(DIK_Q))
		{
			audio_->SoundPlayWave(audio_->GetXAudio2().Get(), doneSE, false);
			if (input_->TriggerKey(DIK_Q) && ruleCount == 1) {
				//シーンの切り替えを依頼
				SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
				ruleCount = 0;
			}
			ruleCount++;
		}

	}

}

void TitleScene::Draw() {

	//スプライト本命処理
	drawBas_->PreDraw();
	if (spriteChange)
	{
		if (ruleCount == 0)
		{
			sprite2_->Draw();
		}
		else
		{
			sprite3_->Draw();
		}
	}
	else
	{
		sprite_->Draw();
	}

	drawBas_->PostDraw();
}

void TitleScene::Finalize() {
	//音声
	audio_->Finalize();
	audio_->SoundUnload(&titleBGM);
	audio_->SoundUnload(&doneSE);
	SafeDelete(sprite_);
	SafeDelete(sprite2_);
	SafeDelete(sprite3_);

}