#include "GamePlayScene.h"
#include "SafeDelete.h"
#include "Quaternion.h"
#include "SphereCollider.h"
#include "Collision.h"
#include "CollisionManager.h"
#include "Player.h"
#include "Fan.h"
#include "imgui.h"

DirectXBasis* GamePlayScene::dxBas_ = DirectXBasis::GetInstance();
Input* GamePlayScene::input_ = Input::GetInstance();
DrawBasis* GamePlayScene::drawBas_ = DrawBasis::GetInstance();
ImGuiManager* GamePlayScene::imGuiManager_ = ImGuiManager::GetInstance();

void GamePlayScene::Initialize() {
	Initialize2d();
	Initialize3d();
}

void GamePlayScene::Update() {
	input_->Update();
	imGuiManager_->Begin();

	Update3d();
	Update2d();

#ifdef _DEBUG
	{
		float playerPos[Vector3Count_] = {
			player_->GetPosition().x,
			player_->GetPosition().y,
			player_->GetPosition().z 
		};

		float playerDir[Vector3Count_] = {
			player_->GetRotation().x,
			player_->GetRotation().y,
			player_->GetRotation().z 
		};
		float blockPos[Vector3Count_] = {
			rayObj_->GetPosition().x,
			rayObj_->GetPosition().y,
			rayObj_->GetPosition().z 
		};

		ImGui::Begin("Player");
		ImGui::SetWindowPos(ImVec2(700, 0));
		ImGui::SetWindowSize(ImVec2(500, 100));
		ImGui::InputFloat3("PlayerPos",playerPos );
		ImGui::InputFloat3("PlayerDir",playerDir );
		ImGui::End();

		ImGui::Begin("Block");
		ImGui::SetWindowPos(ImVec2(0, 400));
		ImGui::SetWindowSize(ImVec2(500, 100));
		ImGui::InputFloat3("BlockPos",blockPos );
		ImGui::End();
	}
#endif

	imGuiManager_->End();
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
	//カメラの初期化
	camera_ = new Camera();
	camera_->SetEye({ 0.0f, 10.0f, -50.0f });

	//各種モデル
	playerModel_ = new Model();
	playerModel_ = Model::LoadFromOBJ("plane", false);
	rayModel_ = new Model();
	rayModel_ = Model::LoadFromOBJ("cube", true);
	fanModel_ = new Model();
	fanModel_ = Model::LoadFromOBJ("planeEnemy", false);

	//プレイヤーの初期化
	player_ = Player::Create(playerModel_);
	player_->Initialize();

	player_->SetCamera(camera_);
	player_->SetScale({ 1.0f, 1.0f, 1.0f });

	player_->SetRotation(CreateRotationVector(
		{ 0.0f,1.0f,0.0f }, ConvertToRadian(180.0f)));
	player_->SetCamera(camera_);


	//レイの初期値
	ray_.start_ = { 0.0f, 0.0f, 50.0f};
	ray_.dir_ = { 0,0,-1 };

	rayObj_ = Object3d::Create();
	rayObj_->Initialize();
	rayObj_->SetModel(rayModel_);
	rayObj_->SetPosition(ray_.start_);
	rayObj_->SetScale({ 2, 2, 2 });
	rayObj_->SetCamera(camera_);

	skydome_ = new Skydome();
	skydome_->Initialize(camera_);

	//ライト生成
	lightGroup_ = new LightGroup();
	lightGroup_ = LightGroup::Create();
	lightGroup_->SetAmbientColor({ 1.0f,1.0f,1.0f });
	Object3d::SetLight(lightGroup_);
}

void GamePlayScene::Initialize2d() {
	drawBas_->LoadTexture(1, "texture.png");
	sprite_->Initialize(drawBas_, 1);

	sprite_->SetAnchorPoint({ 0.5f, 0.5f });
	sprite_->SetSize({ 64.0f,64.0f });
}

void GamePlayScene::Update3d() {
	{
		//imGuiからのライトパラメータを反映
		lightGroup_->SetDirLightDir(0,lightDir_);
	}

	//// カメラ移動
	//if (input_->PressKey(DIK_W) ||
	//	input_->PressKey(DIK_S) ||
	//	input_->PressKey(DIK_D) ||
	//	input_->PressKey(DIK_A)) {
	//	if (input_->PressKey(DIK_W)) { camera_->MoveVector({ 0.0f,+1.0f,0.0f }); }
	//	else if (input_->PressKey(DIK_S)) { camera_->MoveVector({ 0.0f,-1.0f,0.0f }); }
	//	if (input_->PressKey(DIK_D)) { camera_->MoveVector({ +1.0f,0.0f,0.0f }); }
	//	else if (input_->PressKey(DIK_A)) { camera_->MoveVector({ -1.0f,0.0f,0.0f }); }
	//}

	RaycastHit raycastHit_;

	lightGroup_->Update();
	camera_->Update();

	skydome_->Update();
	player_->Update();

	//レイキャストをチェック
	if (collisionManager_->Raycast(ray_, &raycastHit_)) {
		rayObj_->SetPosition(raycastHit_.inter_);
		rayObj_->Update();
	}

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
}

void GamePlayScene::Draw2d() {
	sprite_->Draw();
}

void GamePlayScene::Finalize() {
	player_->Finalize();
	SafeDelete(player_);
	SafeDelete(playerModel_);

	skydome_->Finalize();
	SafeDelete(skydome_);

	SafeDelete(rayObj_);
	SafeDelete(rayModel_);

	SafeDelete(sprite_);

	SafeDelete(lightGroup_);
	SafeDelete(camera_);
}
