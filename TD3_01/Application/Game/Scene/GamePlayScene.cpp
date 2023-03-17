#include "GamePlayScene.h"
#include "SafeDelete.h"
#include "Quaternion.h"
#include "SphereCollider.h"
#include "Collision.h"
#include "CollisionManager.h"
#include "Player.h"

DirectXBasis* GamePlayScene::dxBas_ = DirectXBasis::GetInstance();
Input* GamePlayScene::input_ = Input::GetInstance();
DrawBasis* GamePlayScene::drawBas_ = DrawBasis::GetInstance();

void GamePlayScene::Initialize() {
	Initialize2d();
	Initialize3d();
}

void GamePlayScene::Update() {
	input_->Update();

	Update3d();
	Update2d();
}

void GamePlayScene::Draw() {
	Object3d::PreDraw(dxBas_->GetCommandList().Get());
	Draw3d();
	Object3d::PostDraw();

	drawBas_->PreDraw();
	Draw2d();
	drawBas_->PostDraw();
}

void GamePlayScene::Initialize3d() {
	collisionManager_ = CollisionManager::GetInstance();

	camera_ = new Camera();
	

	playerModel_ = new Model();
	playerModel_ = Model::LoadFromOBJ("plane", true);

	player_ = Player::Create(playerModel_);
	player_->Initialize();

	player_->SetCamera(camera_);
	player_->SetScale({ 1.0f, 1.0f, 1.0f });

	player_->SetRotation(CreateRotationVector(
		{ 0.0f,1.0f,0.0f }, ConvertToRadian(180.0f)));
	player_->SetCamera(camera_);

	rayModel_ = new Model();
	rayModel_ = Model::LoadFromOBJ("cube", true);


	//レイの初期値
	ray_.start_ = { 0.0f, 0.0f, 50.0f};
	ray_.dir_ = { 0,0,-1};

	rayObj_ = Object3d::Create();
	rayObj_->Initialize();
	rayObj_->SetModel(rayModel_);
	rayObj_->SetPosition(ray_.start_);
	rayObj_->SetScale({ 2, 2, 2 });
	rayObj_->SetCamera(camera_);

	skydome_ = new Skydome();
	skydome_->Initialize(camera_);

	//ライト生成
	light_ = new Light();
	light_ = Light::Create();
	light_->SetLightColor({ 1.0f,1.0f,1.0f });
	Object3d::SetLight(light_);

	//ステージ生成
	stage_ = new Stage();
	stage_->Initialize(camera_);
	stage_->StageInitialize(filename_[1]);
}

void GamePlayScene::Initialize2d() {
	drawBas_->LoadTexture(1, "texture.png");
	sprite_->Initialize(drawBas_, 1);

	sprite_->SetAnchorPoint({ 0.5f, 0.5f });
	sprite_->SetSize({ 64.0f,64.0f });
}

void GamePlayScene::Update3d() {
	{
		static Vector3 lightDir = { 0.0f,1.0f,5.0f };

		//	if (input_->PressKey(DIK_W) ||
		//		input_->PressKey(DIK_S) ||
		//		input_->PressKey(DIK_D) ||
		//		input_->PressKey(DIK_A)) {
		//		if (input_->PressKey(DIK_W)) { lightDir.y += 1.0f; }
		//		else if (input_->PressKey(DIK_S)) { lightDir.y -= 1.0f; }
		//		if (input_->PressKey(DIK_D)) { lightDir.x += 1.0f; }
		//		else if (input_->PressKey(DIK_A)) { lightDir.x -= 1.0f; }
		//	}
		light_->SetLightDir(lightDir);
	}

	// カメラ移動
	if (input_->PressKey(DIK_W) ||
		input_->PressKey(DIK_S) ||
		input_->PressKey(DIK_D) ||
		input_->PressKey(DIK_A)) {
		if (input_->PressKey(DIK_W)) { camera_->MoveVector({ 0.0f,+1.0f,0.0f }); }
		else if (input_->PressKey(DIK_S)) { camera_->MoveVector({ 0.0f,-1.0f,0.0f }); }
		if (input_->PressKey(DIK_D)) { camera_->MoveVector({ +1.0f,0.0f,0.0f }); }
		else if (input_->PressKey(DIK_A)) { camera_->MoveVector({ -1.0f,0.0f,0.0f }); }
	}

	RaycastHit raycastHit_;

	//レイキャストをチェック
	if (collisionManager_->Raycast(ray_, &raycastHit_)) {
		rayObj_->SetPosition(raycastHit_.inter_);
		rayObj_->Update();
	}
	
	light_->Update();
	camera_->Update();
	skydome_->Update();
	player_->Update();
	stage_->Update();
	//全ての衝突をチェック
	collisionManager_->CheckAllCollisions();
}

void GamePlayScene::Update2d() {
	// 現在の座標を取得
	Vector2 position = input_->GetMousePosition();

	//移動後の座標を計算

	// 座標の変更を反映
	sprite_->SetPosition(position);

	sprite_->Update();
}

void GamePlayScene::Draw3d() {
	skydome_->Draw();
	rayObj_->Draw();
	player_->Draw();
	stage_->Draw();
}

void GamePlayScene::Draw2d() {
	sprite_->Draw();
}

void GamePlayScene::Finalize() {
	player_->Finalize();
	SafeDelete(player_);
	SafeDelete(playerModel_);

	skydome_->Finalize();
	SafeDelete(stage_);
	SafeDelete(skydome_);

	SafeDelete(rayObj_);
	SafeDelete(rayModel_);

	SafeDelete(sprite_);

	SafeDelete(light_);
	SafeDelete(camera_);
}
