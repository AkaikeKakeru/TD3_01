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

	//オブジェクト基盤
	Object3d::StaticInitialize(dxBas_->GetDevice().Get());
	ParticleManager::StaticInitialize(dxBas_->GetDevice().Get());

	//オブジェクトモデル

	//カメラ生成
	camera_ = new Camera();
	camera_->SetEye({ 0,10,-20 });

	planeModel_ = new Model();
	planeModel_ = Model::LoadFromOBJ("PaperPlane", false);

	skydomeModel_ = new Model();
	skydomeModel_ = Model::LoadFromOBJ("skydome", false);


	planeObj_ = new Object3d();
	planeObj_ = Object3d::Create();
	planeObj_->SetModel(planeModel_);
	planeObj_->SetCamera(camera_);

	skydomeObj_ = new Object3d();
	skydomeObj_ = Object3d::Create();
	skydomeObj_->SetModel(skydomeModel_);
	skydomeObj_->SetCamera(camera_);

	//ライト生成
	lightGroup_ = new LightGroup();
	lightGroup_ = LightGroup::Create();
	lightGroup_->SetAmbientColor({ 1.0f,1.0f,1.0f });
	Object3d::SetLight(lightGroup_);

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

	camera_->Update();
	lightGroup_->Update();

	skydomeObj_->Update();
	planeObj_->Update();

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
	//モデル本命処理
	Object3d::PreDraw(dxBas_->GetCommandList().Get());

	//skydomeObj_->Draw();
	planeObj_->Draw();

	Object3d::PostDraw();

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
	SafeDelete(planeObj_);
	SafeDelete(skydomeObj_);
	SafeDelete(planeModel_);
	SafeDelete(skydomeModel_);
	SafeDelete(sprite_);
	SafeDelete(sprite2_);
	SafeDelete(sprite3_);

	SafeDelete(lightGroup_);
	SafeDelete(camera_);
}