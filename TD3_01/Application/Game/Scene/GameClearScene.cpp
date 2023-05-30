#include "GameClearScene.h"
#include "SafeDelete.h"
#include "Random.h"

DirectXBasis* GameClearScene::dxBas_ = DirectXBasis::GetInstance();
Input* GameClearScene::input_ = Input::GetInstance();

void GameClearScene::Initialize()
{
	//カメラ生成
	camera_ = new Camera();
	camera_->SetEye({ 0,10,-30 });

	planeModel_ = new Model();
	planeModel_ = Model::LoadFromOBJ("PaperPlane", false);

	planeObj_ = new Object3d();
	planeObj_ = Object3d::Create();
	planeObj_->SetModel(planeModel_);
	planeObj_->SetCamera(camera_);

	//ライト生成
	lightGroup_ = new LightGroup();
	lightGroup_ = LightGroup::Create();
	lightGroup_->SetAmbientColor({ 1.0f,1.0f,1.0f });
	Object3d::SetLight(lightGroup_);

	//描画基盤
	drawBas_ = DrawBasis::GetInstance();
	drawBas_->Initialize();

	drawBas_->LoadTexture(20, "gameclear.png");

	sprite_->Initialize(drawBas_, 20);

	//パーティクル
	particle1_ = Particle::LoadFromParticleTexture("particle6.png");
	pm1_ = ParticleManager::Create();
	pm1_->SetParticleModel(particle1_);
	pm1_->SetCamera(camera_);

	change = false;
}

void GameClearScene::Update()
{
	rot += {0.0f, 0.0f, 0.05f};
	planeObj_->SetRotation(rot);

	input_->Update();

	camera_->Update();
	lightGroup_->Update();

	planeObj_->Update();

	sprite_->Update();

	if (input_->TriggerMouse(0)) {
		//シーンの切り替えを依頼
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	pm1_->ActiveZ(particle1_, { 0.0f,0.0f,0.0f }, { 100.0f, 50.0f, 0.0f }, { 0.0f ,0.0f,4.0f }, { 0.0f,0.001f,0.0f }, 7, { RandomOutput(1.0f,5.0f), 0.0f });
	pm1_->Update();

}

void GameClearScene::Draw()
{
	//スプライト本命処理
	drawBas_->PreDraw();
	sprite_->Draw();
	drawBas_->PostDraw();//モデル本命処理

	Object3d::PreDraw(dxBas_->GetCommandList().Get());
	planeObj_->Draw();

	Object3d::PostDraw();

	ParticleManager::PreDraw(dxBas_->GetCommandList().Get());
	pm1_->Draw();
	ParticleManager::PostDraw();
}

void GameClearScene::Finalize()
{
	SafeDelete(planeModel_);
	SafeDelete(planeObj_);
	SafeDelete(sprite_);
	//パーティクル
	SafeDelete(particle1_);
	SafeDelete(pm1_);
}
