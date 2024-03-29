#include "GamePlayScene.h"
#include "SafeDelete.h"
#include "Quaternion.h"
#include "SphereCollider.h"
#include "Collision.h"
#include "CollisionManager.h"
#include "Player.h"
#include "Fan.h"
#include "imgui.h"
#include "CollisionAttribute.h"

#include "SceneManager.h"

DirectXBasis* GamePlayScene::dxBas_ = DirectXBasis::GetInstance();
Input* GamePlayScene::input_ = Input::GetInstance();
ImGuiManager* GamePlayScene::imGuiManager_ = ImGuiManager::GetInstance();

void GamePlayScene::Initialize() {
	Initialize2d();
	Initialize3d();
}

void GamePlayScene::Update() {
	input_->Update();
	
	Update3d();
	Update2d();

#ifdef _DEBUG
	{
		imGuiManager_->Begin();

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

		float rayDir0[Vector3Count_] = {
			fan_[0]->GetRay()->dir_.x,
			fan_[0]->GetRay()->dir_.y,
			fan_[0]->GetRay()->dir_.z
		};
		float rayPos0[Vector3Count_] = {
			fan_[0]->GetRay()->start_.x,
			fan_[0]->GetRay()->start_.y,
			fan_[0]->GetRay()->start_.z
		};

		float rayDir1[Vector3Count_] = {
			fan_[1]->GetRay()->dir_.x,
			fan_[1]->GetRay()->dir_.y,
			fan_[1]->GetRay()->dir_.z
		};
		float rayPos1[Vector3Count_] = {
			fan_[1]->GetRay()->start_.x,
			fan_[1]->GetRay()->start_.y,
			fan_[1]->GetRay()->start_.z
		};

		float rayDir2[Vector3Count_] = {
			fan_[2]->GetRay()->dir_.x,
			fan_[2]->GetRay()->dir_.y,
			fan_[2]->GetRay()->dir_.z
		};
		float rayPos2[Vector3Count_] = {
			fan_[2]->GetRay()->start_.x,
			fan_[2]->GetRay()->start_.y,
			fan_[2]->GetRay()->start_.z
		};
		float rayInter[Vector3Count_] = {
			interRay_.x,
			interRay_.y,
			interRay_.z
		};

		float fanDir0[Vector3Count_] = {
			fan_[0]->GetRotation().x,
			fan_[0]->GetRotation().y,
			fan_[0]->GetRotation().z
		};
		float fanPos0[Vector3Count_] = {
			fan_[0]->GetPosition().x,
			fan_[0]->GetPosition().y,
			fan_[0]->GetPosition().z
		};

		float fanDir1[Vector3Count_] = {
			fan_[1]->GetRotation().x,
			fan_[1]->GetRotation().y,
			fan_[1]->GetRotation().z
		};
		float fanPos1[Vector3Count_] = {
			fan_[1]->GetPosition().x,
			fan_[1]->GetPosition().y,
			fan_[1]->GetPosition().z
		};

		float fanDir2[Vector3Count_] = {
			fan_[2]->GetRotation().x,
			fan_[2]->GetRotation().y,
			fan_[2]->GetRotation().z
		};
		float fanPos2[Vector3Count_] = {
			fan_[2]->GetPosition().x,
			fan_[2]->GetPosition().y,
			fan_[2]->GetPosition().z
		};

		float rayCol[1] = {
			colRay_
		};

		ImGui::Begin("Player");
		ImGui::SetWindowPos(ImVec2(700, 0));
		ImGui::SetWindowSize(ImVec2(500, 100));
		ImGui::InputFloat3("PlayerPos", playerPos);
		ImGui::InputFloat3("PlayerDir", playerDir);
		ImGui::End();

		ImGui::Begin("Fan");
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(ImVec2(500, 200));
		ImGui::InputFloat3("FanPos", fanPos0);
		ImGui::InputFloat3("FanDir", fanDir0);
		ImGui::InputFloat3("FanPos", fanPos1);
		ImGui::InputFloat3("FanDir", fanDir1);
		ImGui::InputFloat3("FanPos", fanPos2);
		ImGui::InputFloat3("FanDir", fanDir2);
		ImGui::End();

		ImGui::Begin("Ray");
		ImGui::SetWindowPos(ImVec2(0, 500));
		ImGui::SetWindowSize(ImVec2(500, 200));
		ImGui::InputFloat3("RayPos", rayPos0);
		ImGui::InputFloat3("RayDir", rayDir0);
		ImGui::InputFloat3("RayPos", rayPos1);
		ImGui::InputFloat3("RayDir", rayDir1);
		ImGui::InputFloat3("RayPos", rayPos2);
		ImGui::InputFloat3("RayDir", rayDir2);
		ImGui::InputFloat("RayCol", rayCol);
		ImGui::End();

	}imGuiManager_->End();
#endif

	
}

void GamePlayScene::Draw() {

	Object3d::PreDraw(dxBas_->GetCommandList().Get());
	Draw3d();
	Object3d::PostDraw();

	ParticleManager::PreDraw(dxBas_->GetCommandList().Get());
	DrawParticle();
	ParticleManager::PostDraw();

	drawBas_->PreDraw();
	Draw2d();
	drawBas_->PostDraw();
}

void GamePlayScene::Initialize3d() {
	audio_ = Audio::GetInstance();
	audio_->Initialize();

	collisionManager_ = CollisionManager::GetInstance();
	//カメラの初期化
	camera_ = new Camera();

	//視点等セット
	//camera_->SetEye({ 15.0f, 70.0f, -40.0f });
	//camera_->SetTarget({ 15.0f,20.0f,10.0f });
	///*
	//camera_->SetEye({ 0.0f, 90.0f, -100.0f });
	//camera_->SetTarget({ 0.0f,25.0f,0.0f });
	//*/
	//camera_->SetUp({ 0.0f, 1.0f, 0.0f });

	cameraFixed_.SetEye({ 15.0f, 70.0f, -40.0f });
	cameraFixed_.SetTarget({ 15.0f,20.0f,10.0f });
	cameraFixed_.SetUp({ 0.0f, 1.0f, 0.0f });


	camera_->SetEye(cameraFixed_.GetEye());
	camera_->SetTarget(cameraFixed_.GetTarget());
	camera_->SetUp(cameraFixed_.GetUp());

	//各種モデル
	playerModel_ = new Model();
	playerModel_ = Model::LoadFromOBJ("PaperPlane", true);
	fanModel_ = new Model();
	fanModel_ = Model::LoadFromOBJ("Fan", true);
	fanModelConst_ = new Model();
	fanModelConst_ = Model::LoadFromOBJ("constfan", true);

	//プレイヤーの初期化
	player_ = Player::Create(playerModel_);

	player_->SetCamera(camera_);
	player_->SetScale({ 1.0f, 1.0f, 1.0f });

	player_->SetRotation(CreateRotationVector(
		{ 0.0f,1.0f,0.0f }, ConvertToRadian(180.0f)));

	player_->SetCamera(camera_);
	player_->Update();

	for (int i = 0; i < FanCount_; i++) {
		//ファンの初期化
		fan_[i] = Fan::Create(fanModel_, audio_);
		fan_[i]->SetScale({ 1.0f,1.0f,1.0f });
		fan_[i]->SetCamera(camera_);
	}

	//ファン下向き時の数値設定
	fan_[0]->SetFanDirection(Fan::Down);
	fan_[0]->SetIsControl(true);

	//ファン左向き時の数値設定
	fan_[1]->SetFanDirection(Fan::Down);
	fan_[1]->SetIsControl(true);

	//ファン右向き時の数値設定
	fan_[2]->SetFanDirection(Fan::Down);
	fan_[2]->SetIsControl(true);

	fan_[3]->SetFanDirection(Fan::Down);
	fan_[3]->SetIsControl(true);

	fan_[4]->SetFanDirection(Fan::Down);
	fan_[4]->SetIsControl(true);

	skydome_ = new Skydome();
	skydome_->Initialize(camera_);

	//ライト生成
	lightGroup_ = new LightGroup();
	lightGroup_ = LightGroup::Create();
	lightGroup_->SetAmbientColor({ 1.0f,1.0f,1.0f });
	Object3d::SetLight(lightGroup_);

	//パーティクル
	particle1_ = Particle::LoadFromParticleTexture("particle6.png");
	pm1_ = ParticleManager::Create();
	pm1_->SetParticleModel(particle1_);
	pm1_->SetCamera(camera_);

	particle2_ = Particle::LoadFromParticleTexture("particle1.png");
	pm2_ = ParticleManager::Create();
	pm2_->SetParticleModel(particle2_);
	pm2_->SetCamera(camera_);

	wind_ = Particle::LoadFromParticleTexture("particle1.png");
	windpm_ = ParticleManager::Create();
	windpm_->SetParticleModel(wind_);
	windpm_->SetCamera(camera_);

	//音
	stageBGM = audio_->SoundLoadWave("Resource/sound/stage.wav");
	hitSE = audio_->SoundLoadWave("Resource/sound/hit.wav");
	clearSE = audio_->SoundLoadWave("Resource/sound/stageclear.wav");
	doneSE = audio_->SoundLoadWave("Resource/sound/done.wav");
	resetSE = audio_->SoundLoadWave("Resource/sound/reset.wav");
	backSE = audio_->SoundLoadWave("Resource/sound/back.wav");
	runSE = audio_->SoundLoadWave("Resource/sound/active.wav");
	windSE = audio_->SoundLoadWave("Resource/sound/wind.wav");
	
	audio_->SoundPlayWave(audio_->GetXAudio2().Get(), stageBGM, true);

	//ステージ生成
	stage_ = new Stage();
	stage_->Initialize(camera_);

	ParameterPlayer(positionPlayer, player_->GetStartDirection(), 0);
	ParamaterFun(positionFan[0], positionFan[1], positionFan[2], positionFan[3], positionFan[4]);
	audio_->SoundPlayWave(audio_->GetXAudio2().Get(), windSE, true);

	isPause_ = false;
	isReally_ = false;
	isrule_ = false;
}

void GamePlayScene::Initialize2d() {
	//描画基盤
	drawBas_ = DrawBasis::GetInstance();

	drawBas_->LoadTexture(10, "pause.png");
	drawBas_->LoadTexture(11, "tutorial.png");
	drawBas_->LoadTexture(12, "tutorial2.png");
	drawBas_->LoadTexture(13, "pauseinfo.png");
	drawBas_->LoadTexture(14, "resetinfo.png");
	drawBas_->LoadTexture(15, "really.png");
	drawBas_->LoadTexture(16, "stageclear.png");
	drawBas_->LoadTexture(17, "active.png");

	spritePause_->Initialize(drawBas_, 10);
	spritePauseInfo_->Initialize(drawBas_, 13);
	spriteResetInfo_->Initialize(drawBas_, 14);
	spriteActiveInfo_->Initialize(drawBas_, 17);
	rule_->Initialize(drawBas_, 11);
	rule2_->Initialize(drawBas_, 12);
	really_->Initialize(drawBas_, 15);
	stageClear_->Initialize(drawBas_, 16);

	spritePause_->SetColor({ 1.0f,1.0f,1.0f,0.75f });
	spriteResetInfo_->SetPosition({ 900.0f,0.0f });
	spriteActiveInfo_->SetPosition({ 200.0f,600.0f });
}

void GamePlayScene::Update3d() {
	if (!isPause_ && !isrule_)
	{
		colRay_ = false;
		{
			//imGuiからのライトパラメータを反映
			lightGroup_->SetDirLightDir(0, lightDir_);
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

		stageCollision = CollisionStageFlag(player_, stage_);

		

		if (isClear_)//クリアしたら
		{

			//pm1_->Active(particle1_, { camera_->GetEye() }, { 100.0f, 100.0f, 100.0f }, { 0.2f ,0.2f,0.2f }, { 0.0f,0.001f,0.0f }, 5, { 13.0f, 0.0f });
			//pm1_->Active(particle2_, { camera_->GetEye() }, { 100.0f, 100.0f, 100.0f }, { 0.2f ,0.2f,0.2f }, { 0.0f,0.001f,0.0f }, 5, { 13.0f, 0.0f });
			pm1_->ActiveY(particle1_, { 30.0f ,-10.0f,50.0f }, { 150.0f ,0.0f,150.0f }, { 1.4f,5.0f,1.5f }, { 0.0f,0.001f,0.0f }, 10, { 3.0f, 0.0f });
	
			//pm2_->Active(particle2_, 30.0f, 0.2f, 0.001f, 5, { 6.0f,0.0f });

			//ImGui::Begin("Stage Clear!");
			//ImGui::SetWindowPos(ImVec2(10, 10));
			//ImGui::SetWindowSize(ImVec2(500, 200));
			//ImGui::SetWindowFontScale(2.0f);
			//ImGui::Text("Mouse left click Next Stage");
			//ImGui::End();
			if (timeMovingPlayerProgress_ <= 0) {
				isStopForcedPlayer_ = true;
			}

			if (isStopForcedPlayer_) {
				player_->SetIsRun(false);
			}
			else {
				timeMovingPlayerProgress_--;
			}

			if (!isEndCameraLerp_) {
				isCameraLerp_ = true;
			}

			if (input_->TriggerMouse(0))
			{
				audio_->SoundPlayWave(audio_->GetXAudio2().Get(), doneSE, false);
				isEndCameraLerp_ = false;

				camera_->SetEye(cameraFixed_.GetEye());
				camera_->SetTarget(cameraFixed_.GetTarget());
				camera_->SetUp(cameraFixed_.GetUp());

				timeCameraLerpProgress_ = defTime_;
				isCameraLerp_ = false;
				isEndCameraLerp_ = false;

				timeMovingPlayerProgress_ = defTime_ * Num_OffScreen_Multi_;
				isStopForcedPlayer_ = false;

				switch (scene_)
				{
				case Stage0:	
					//ここで次のステージ(ここだとステージ1の値)の値をセット(サンプル)
					positionPlayer = { 28.0f,0.0f,26.0f };
					player_->SetStartDirection(Player::Left);
					ParameterPlayer(positionPlayer, player_->GetStartDirection(), 1);

					positionFan[0] = { 60.0f,0.0f,50.0f };
					positionFan[1] = { -12.0f,0.0f,18.0f };
					positionFan[2] = { -36.0f,0.0f,34.0f };
					positionFan[3] = { 36.0f,0.0f,66.0f };
					positionFan[4] = { 20.0f,0.0f,90.0f };

					fan_[0]->SetIsControl(true);
					fan_[0]->SetFanDirection(Fan::Up);
					fan_[1]->SetIsControl(false);
					fan_[1]->SetFanDirection(Fan::Up);
					fan_[2]->SetIsControl(false);
					fan_[2]->SetFanDirection(Fan::Up);
					fan_[3]->SetIsControl(false);
					fan_[3]->SetFanDirection(Fan::Left);
					fan_[4]->SetIsControl(false);
					fan_[4]->SetFanDirection(Fan::Left);

					ParamaterFun(positionFan[0], positionFan[1], positionFan[2], positionFan[3], positionFan[4]);

					scene_ = Stage1;
					break;

				case Stage1:
					positionPlayer = { -28.0f,0.0f,42.0f };
					player_->SetStartDirection(Player::Down);
					ParameterPlayer(positionPlayer, player_->GetStartDirection(), 2);

					positionFan[0] = { 60.0f,0.0f,66.0f };
					positionFan[1] = { 60.0f,0.0f,34.0f };
					positionFan[2] = { -36.0f,0.0f,26.0f };
					positionFan[3] = { 28.0f,0.0f,18.0f };
					positionFan[4] = { 20.0f,0.0f,90.0f };

					fan_[0]->SetIsControl(true);
					fan_[0]->SetFanDirection(Fan::Up);
					fan_[1]->SetIsControl(true);
					fan_[1]->SetFanDirection(Fan::Up);
					fan_[2]->SetIsControl(false);
					fan_[2]->SetFanDirection(Fan::Right);
					fan_[3]->SetIsControl(false);
					fan_[3]->SetFanDirection(Fan::Up);
					fan_[4]->SetIsControl(false);
					fan_[4]->SetFanDirection(Fan::Left);

					ParamaterFun(positionFan[0], positionFan[1], positionFan[2], positionFan[3], positionFan[4]);
					scene_ = Stage2;
					break;

				case Stage2:
					positionPlayer = { 36.0f,0.0f,82.0f };
					player_->SetStartDirection(Player::Left);
					ParameterPlayer(positionPlayer, player_->GetStartDirection(), 3);

					positionFan[0] = { 60.0f,0.0f,90.0f };
					positionFan[1] = { 60.0f,0.0f,50.0f };
					positionFan[2] = { 60.0f,0.0f,10.0f };
					positionFan[3] = { 12.0f,0.0f,90.0f };
					positionFan[4] = { -12.0f,0.0f,66.0f };

					fan_[0]->SetIsControl(true);
					fan_[0]->SetFanDirection(Fan::Up);
					fan_[1]->SetIsControl(true);
					fan_[1]->SetFanDirection(Fan::Up);
					fan_[2]->SetIsControl(true);
					fan_[2]->SetFanDirection(Fan::Up);
					fan_[3]->SetIsControl(false);
					fan_[3]->SetFanDirection(Fan::Down);
					fan_[4]->SetIsControl(false);
					fan_[4]->SetFanDirection(Fan::Down);

					ParamaterFun(positionFan[0], positionFan[1], positionFan[2], positionFan[3], positionFan[4]);
					scene_ = Stage3;
					break;

				case Stage3:
					positionPlayer = { 28.0f,0.0f,26.0f };
					player_->SetStartDirection(Player::Up);
					ParameterPlayer(positionPlayer, player_->GetStartDirection(), 4);

					positionFan[0] = { 60.0f, 0.0f, 98.0f };
					positionFan[1] = { 60.0f,0.0f,74.0f };
					positionFan[2] = { 60.0f,0.0f,50.0f };
					positionFan[3] = { 60.0f,0.0f,26.0f };
					positionFan[4] = { 60.0f,0.0f,2.0f };
					//ファン下向き時の数値設定
					fan_[0]->SetFanDirection(Fan::Down);
					fan_[0]->SetIsControl(true);

					//ファン左向き時の数値設定
					fan_[1]->SetFanDirection(Fan::Down);
					fan_[1]->SetIsControl(true);

					//ファン右向き時の数値設定
					fan_[2]->SetFanDirection(Fan::Down);
					fan_[2]->SetIsControl(true);

					fan_[3]->SetFanDirection(Fan::Down);
					fan_[3]->SetIsControl(true);

					fan_[4]->SetFanDirection(Fan::Down);
					fan_[4]->SetIsControl(true);
					ParamaterFun(positionFan[0], positionFan[1], positionFan[2], positionFan[3], positionFan[4]);

					scene_ = Stage4;
					break;

				case Stage4:
					positionPlayer = { -4.0f,0.0f,50.0f };
					player_->SetStartDirection(Player::Down);
					ParameterPlayer(positionPlayer, player_->GetStartDirection(), 5);

					positionFan[0] = { 60.0f, 0.0f, 90.0f };
					positionFan[1] = { 60.0f,0.0f,50.0f };
					positionFan[2] = { 60.0f,0.0f,10.0f };
					positionFan[3] = { 28.0f,0.0f,58.0f };
					positionFan[4] = { 36.0f,0.0f,82.0f };
					//ファン下向き時の数値設定
					fan_[0]->SetFanDirection(Fan::Down);
					fan_[0]->SetIsControl(true);

					//ファン左向き時の数値設定
					fan_[1]->SetFanDirection(Fan::Down);
					fan_[1]->SetIsControl(true);

					//ファン右向き時の数値設定
					fan_[2]->SetFanDirection(Fan::Down);
					fan_[2]->SetIsControl(true);

					fan_[3]->SetFanDirection(Fan::Up);
					fan_[3]->SetIsControl(false);

					fan_[4]->SetFanDirection(Fan::Left);
					fan_[4]->SetIsControl(false);
					ParamaterFun(positionFan[0], positionFan[1], positionFan[2], positionFan[3], positionFan[4]);

					scene_ = Stage5;
					break;
				case Stage5:
					if (input_->TriggerMouse(0)) {
						//シーンの切り替えを依頼
						SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
					}

					break;
				}
			}
		}
		else
		{
			if (input_->TriggerKey(DIK_SPACE) && !isReally_)
			{
				if (!player_->GetIsRun())audio_->SoundPlayWave(audio_->GetXAudio2().Get(), runSE, false);
			}
			
			if (stage_->GetIsGoal())
			{
				audio_->SoundPlayWave(audio_->GetXAudio2().Get(), clearSE, false);
				isClear_ = true;
			}
			//player_->Update();

			for (int i = 0; i < FanCount_; i++) {
				fan_[i]->SetStage(stage_);
				//風の演出
				if (!isReset_)
				{
					ActiveWind(fan_[i]->GetFanDirection(), fan_[i]->GetPosition());
				}
				//ファンが動かせるかどうかかどうか
				if (fan_[i]->GetIsControl())
				{
					fan_[i]->SetModel(fanModel_);
				}
				else
				{
					fan_[i]->SetModel(fanModelConst_);
				}
				fan_[i]->Update();
			}
			//リセット
			if (input_->TriggerKey(DIK_R))
			{
				if(!isReset_)audio_->SoundPlayWave(audio_->GetXAudio2().Get(), resetSE, false);
				isReset_ = true;
			}

			if (isReset_) {
				ResetCameraMove();
				
			}
			//Pause機能
			if (input_->TriggerKey(DIK_Q))
			{
				if(!isPause_)audio_->SoundPlayWave(audio_->GetXAudio2().Get(), doneSE, false);
				isPause_ = true;
			}

			player_->OnCollisionStage(stageCollision, positionPlayer);
			//全ての衝突をチェック
			collisionManager_->CheckAllCollisions();

		}

		if (isCameraLerp_) {
			if (!isEndCameraLerp_) {
				ClearCameraMove();
			}
		}

		lightGroup_->Update();
		camera_->Update();


		//レイキャストをチェック
		for (int i = 0; i < FanCount_; i++) {
			if (player_->GetIsRun() == true) {

				if (collisionManager_->Raycast(*fan_[i]->GetRay(), COLLISION_ATTR_PLAYER, &raycastHit_)) {

					if (raycastHit_.distance_ <= 50.0f) {
						raycastHit_.object_->SetRotation(fan_[i]->GetRotation());
						raycastHit_.object_->Update();

						raycastHit_.object_->SetRotation(fan_[i]->GetRotation());
						raycastHit_.object_->Update();

						colRay_ = true;
						interRay_ = raycastHit_.inter_;
					}
				}
			}
		}

		skydome_->Update();
		stage_->Update();
		for (int i = 0; i < FanCount_; i++) {
			fan_[i]->Update();
		}

		player_->Update();

		pm1_->Update();
		pm2_->Update();
		windpm_->Update();
	}
	else if (isrule_)
	{
		if (input_->TriggerKey(DIK_Q))
		{
			audio_->SoundPlayWave(audio_->GetXAudio2().Get(), doneSE, false);

			if (input_->TriggerKey(DIK_Q) && ruleCount == 1) {
				
				isrule_ = false;

			}
			ruleCount++;
		}
	}
	else if (isPause_)
	{
		if (input_->TriggerKey(DIK_W))
		{
			audio_->SoundPlayWave(audio_->GetXAudio2().Get(), doneSE, false);
			ruleCount = 0;
			isrule_ = true;
			isPause_ = false;
		}
		if (input_->TriggerKey(DIK_Q))
		{
			audio_->SoundPlayWave(audio_->GetXAudio2().Get(), backSE, false);
			isPause_ = false;
		}
		if (input_->TriggerKey(DIK_E))
		{
			audio_->SoundPlayWave(audio_->GetXAudio2().Get(), doneSE, false);
			isReally_ = true;
			isPause_ = false;
		}

	}

	if (isReally_)
	{
		//タイトルへ
		if (input_->TriggerKey(DIK_Q))
		{
			//シーンの切り替えを依頼
			SceneManager::GetInstance()->ChangeScene("TITLE");

		}
		//Pauseへ
		if (input_->TriggerKey(DIK_W))
		{
			audio_->SoundPlayWave(audio_->GetXAudio2().Get(), backSE, false);
			isPause_ = true;
			isReally_ = false;
		}
	}
}

void GamePlayScene::Update2d() {
	// 現在の座標を取得
	Vector2 position = input_->GetMousePosition();

	//移動後の座標を計算

	// 座標の変更を反映
	spriteActiveInfo_->Update();
	spritePause_->Update();
	spriteResetInfo_->Update();

}

void GamePlayScene::Draw3d() {
	skydome_->Draw();

	for (int i = 0; i < FanCount_; i++) {
		fan_[i]->Draw();
	}
	player_->Draw();
	stage_->Draw();
}

void GamePlayScene::DrawParticle()
{
	pm1_->Draw();
	pm2_->Draw();

	windpm_->Draw();
}

void GamePlayScene::Draw2d() {
	if (isrule_)
	{
		if (ruleCount == 0)
		{
			rule_->Draw();
		}
		else
		{
			rule2_->Draw();
		}


	}
	else if (isPause_)
	{
		spritePause_->Draw();


	}
	else
	{
		if (!isClear_)
		{
			spritePauseInfo_->Draw();
			spriteResetInfo_->Draw();
			spriteActiveInfo_->Draw();
		}
		else
		{
			stageClear_->Draw();
		}
	}

	if (isReally_)
	{
		really_->Draw();
	}
}

void GamePlayScene::ClearCameraMove() {
	if (timeCameraLerpProgress_ <= 0) {
		timeCameraLerpProgress_ = defTime_;
		isCameraLerp_ = false;
		isEndCameraLerp_ = true;
	}
	else {
		timeCameraLerpProgress_--;

		Vector3 eye = cameraFixed_.GetEye();
		Vector3 target =
			cameraFixed_.GetTarget();

		Vector3 goal = player_->GetPosition()
			+ Vector3{ -20,30,10 };

		Vector3 moveE = EaseInOut(
			goal,
			eye,
			timeCameraLerpProgress_ / defTime_
		);

		Vector3 moveT = EaseInOut(
			player_->GetPosition(),
			target,
			timeCameraLerpProgress_ / defTime_
		);
		
		camera_->SetEye(moveE);

		camera_->SetTarget(moveT);
	}
}

void GamePlayScene::ResetCameraMove() {
	if (timeCameraLerpProgress_ <= 0) {
		timeCameraLerpProgress_ = defTime_;
		ReSetPositionPlayer(positionPlayer);
		ReSetPositionFan(positionFan[0], positionFan[1], positionFan[2], positionFan[3], positionFan[4]);
		isReset_ = false;

		camera_->SetEye(cameraFixed_.GetEye());
		camera_->SetTarget(cameraFixed_.GetTarget());
		camera_->SetUp(cameraFixed_.GetUp());
	}
	else {
		timeCameraLerpProgress_--;

		Vector3 moveE = EaseInOut(
			player_->GetPosition(),
			cameraFixed_.GetEye(),
			timeCameraLerpProgress_ / defTime_
		);

		Vector3 moveT = EaseInOut(
			player_->GetPosition(),
			cameraFixed_.GetTarget(),
			timeCameraLerpProgress_ / defTime_
		);

		camera_->SetEye(moveE);
		camera_->SetTarget(moveT);
	}
}

void GamePlayScene::Finalize() {
	
	//音声
	audio_->Finalize();
	audio_->SoundUnload(&stageBGM);
	audio_->SoundUnload(&hitSE);
	audio_->SoundUnload(&clearSE);
	audio_->SoundUnload(&doneSE);
	audio_->SoundUnload(&resetSE);
	audio_->SoundUnload(&backSE);
	audio_->SoundUnload(&runSE);
	audio_->SoundUnload(&windSE);

	player_->Finalize();
	SafeDelete(player_);
	SafeDelete(playerModel_);

	for (int i = 0; i < FanCount_; i++) {
		fan_[i]->Finalize();
		SafeDelete(fan_[i]);
	}
	SafeDelete(fanModel_);
	SafeDelete(fanModelConst_);

	SafeDelete(stage_);

	skydome_->Finalize();
	SafeDelete(skydome_);
	
	//パーティクル
	SafeDelete(particle1_);
	SafeDelete(pm1_);
	SafeDelete(particle2_);
	SafeDelete(pm2_);
	SafeDelete(wind_);
	SafeDelete(windpm_);

	SafeDelete(spritePause_);
	SafeDelete(spritePauseInfo_);
	SafeDelete(spriteResetInfo_);
	SafeDelete(spriteActiveInfo_);
	SafeDelete(rule_);
	SafeDelete(rule2_);
	SafeDelete(really_);
	SafeDelete(stageClear_);

	SafeDelete(lightGroup_);
	SafeDelete(camera_);
}

bool GamePlayScene::CollisionStageFlag(Player* p, Stage* s)
{
	// 各座標変数の宣言
	Vector3 pPos = p->GetPosition();
	float pRadius = p->GetRadius();
	float bscale = s->GetBlockSize();

	float pX1, pX2, pZ1, pZ2;
	// プレイヤーの矩形座標
	pX1 = pPos.x - pRadius;
	pX2 = pPos.x + pRadius;
	pZ1 = pPos.z - pRadius;
	pZ2 = pPos.z + pRadius;

	// プレイヤーLeftTop座標
	int pLT[2] = { static_cast<int>((pX1 / (bscale * 2) + 5))/* * -1)*/,
		static_cast<int>(((pZ1 / (bscale * 2)) - 11) * -1) };
	int isFloor = 0;

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			// 足元のブロックを判別
			if (s->CheckFloorBlock(pLT[0] + i, pLT[1] + j)) {
				isFloor++;
			}
			if (isFloor == 2) {
				if (!isClear_) {
					p->Stop(positionPlayer, player_->GetStartDirection());
				}
			}
			s->CheckBlock(pLT[0] + i, pLT[1] + j);
			// 各座標変数の宣言
			Vector3 bPos = s->GetBlockPosition(pLT[0] + i, pLT[1] + j);
			float bRadius = s->GetRadius();
			float bX1, bX2, bZ1, bZ2;
			// ブロックの矩形座標
			bX1 = bPos.x - bRadius;
			bX2 = bPos.x + bRadius;
			bZ1 = bPos.z - bRadius;
			bZ2 = bPos.z + bRadius;

			// 当たり判定
			if (pX1 < bX2 && pX2 > bX1 && pZ1 < bZ2 && pZ2 > bZ1) {
				audio_->SoundPlayWave(audio_->GetXAudio2().Get(), hitSE, false);
				pm2_->Active(particle2_, pPos, { 0.0f ,0.0f,25.0f }, { 3.0f,3.0f,3.0f }, { 0.0f,0.001f,0.0f }, 100, { 1.0f, 0.0f });
				return true;

			}
		}
	}

	return false;
}

void GamePlayScene::ParameterPlayer(const Vector3& playerPos, const float direction, const int& stageNum)
{
	Vector3 pos = playerPos;
	player_->SetPosition(pos);
	player_->Update();
	player_->Stop(pos, direction);

	stage_->StageInitialize(filename_[stageNum]);
	isClear_ = false;
	isReset_ = false;
}

void GamePlayScene::ParamaterFun(const Vector3& fanPos1, const Vector3& fanPos2, const Vector3& fanPos3, const Vector3& fanPos4, const Vector3& fanPos5)
{
	Vector3 pos[FanCount_] = { {fanPos1},{fanPos2},{fanPos3},{fanPos4},{fanPos5} };
	for (size_t i = 0; i < FanCount_; i++)
	{
		fan_[i]->SetPosition(pos[i]);
		fan_[i]->Update();
	}
	
}

void GamePlayScene::ReSetPositionPlayer(const Vector3& playerPos)
{
	Vector3 pos = playerPos;
	player_->SetPosition(pos);
	player_->SetDirection(player_->GetStartDirection());
	player_->Stop(pos, player_->GetStartDirection());
}

void GamePlayScene::ReSetPositionFan(const Vector3& fanPos1, const Vector3& fanPos2, const Vector3& fanPos3, const Vector3& fanPos4, const Vector3& fanPos5)
{
	Vector3 pos[FanCount_] = { {fanPos1},{fanPos2},{fanPos3},{fanPos4},{fanPos5} };
	for (size_t i = 0; i < FanCount_; i++)
	{
		fan_[i]->SetPosition(pos[i]);

	}
	
}

void GamePlayScene::ActiveWind(const int dir, const Vector3& position)
{
	switch (dir) {
	case Fan::Direction::Up:
		//上方向に風の演出
		windpm_->ActiveZ(wind_, position, { 8.0f ,8.0f,0.0f }, { 0.0f,0.0f,4.0f }, { 0.0f,0.001f,0.0f }, 1, { 2.0f, 0.0f });
		break;

	case Fan::Direction::Down:
		//下方向に風の演出
		windpm_->ActiveZ(wind_, position, { 8.0f ,8.0f,0.0f }, { 0.0f,0.0f,-4.0f }, { 0.0f,0.001f,0.0f }, 1, { 2.0f, 0.0f });
		break;

	case Fan::Direction::Right:
		//右方向に風の演出
		windpm_->ActiveX(wind_, position, { 0.0f ,8.0f,8.0f }, { 4.0f,0.0f,0.0f }, { 0.0f,0.001f,0.0f }, 1, { 2.0f, 0.0f });
		break;

	case Fan::Direction::Left:
		//左方向に風の演出
		windpm_->ActiveX(wind_, position, { 0.0f ,8.0f,8.0f }, { -4.0f,0.0f,0.0f }, { 0.0f,0.001f,0.0f }, 1, { 2.0f, 0.0f });
		break;

	}
	
}