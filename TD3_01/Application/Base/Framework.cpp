#include "Framework.h"
#include "Object3d.h"
#include "DrawBasis.h"
#include "ParticleManager.h"
#include "TitleScene.h"
#include <imgui.h>

SceneManager* Framework::sceneManager_ = SceneManager::GetInstance();

void Framework::Run(){
	/// 初期化
	Initialize();

	/// ゲームループ
	while (true) {
		/// 更新
		Update();

		if (IsEndRequest()) {
			//終了リクエストが来たら抜ける
			break;
		}

		/// 描画
		Draw();
	}

	Finalize();
}

void Framework::Initialize(){
	///基盤初期化
	//アプリケーション
	winApp_ = WinApp::GetInstance();
	winApp_->Initialize();

	//DirectX基盤
	dxBas_ = DirectXBasis::GetInstance();
	dxBas_->Initialize(winApp_);

	//Input
	input_ = Input::GetInstance();
	input_->Initialize();

	//音声
	audio_ = Audio::GetInstance();
	audio_->Initialize();

	//ImGuiマネージャー
	imGuiManager_ = ImGuiManager::GetInstance();
	imGuiManager_->Initialize(dxBas_);

	//オブジェクト基盤
	Object3d::StaticInitialize(dxBas_->GetDevice().Get());

	//描画基盤(スプライト)
	DrawBasis::GetInstance();
	DrawBasis::Initialize();

	//パーティクルマネージャー
	ParticleManager::StaticInitialize(dxBas_->GetDevice().Get());

	//ライト静的初期化
	LightGroup::StaticInitialize(dxBas_->GetDevice().Get());
}

void Framework::Update(){
	//windowsのメッセージ処理
	if (winApp_->ProcessMessage()) {
		//ゲームループを抜ける
		isEndRequest_ = true;
	}

	imGuiManager_->Begin();
	ImGui::Begin("How to Play");
	ImGui::SetWindowPos(ImVec2(10, 10));
	ImGui::SetWindowSize(ImVec2(500, 200));
	ImGui::SetWindowFontScale(2.0f);
	ImGui::Text("Arrow : Move of Fan Positon");
	ImGui::Text("WASD : Rota of Fan Direction");
	ImGui::Text("Hold Space : Plane Move Forward");
	ImGui::Text("");
	ImGui::Text("Press ENTER to Start");
	ImGui::End();

#ifdef _DEBUG
#endif
	imGuiManager_->End();
	sceneManager_->Update();
}

void Framework::Finalize(){
	audio_->Finalize();

	imGuiManager_->Finalize();
	sceneManager_->Finalize();
	delete sceneFactory_;
}