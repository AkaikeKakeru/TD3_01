#pragma once
#include "BaseScene.h"

#include "Input.h"
#include "DrawBasis.h"
#include "Model.h"
#include "Sprite.h"
#include "Object3d.h"

#include "Camera.h"
#include "LightGroup.h"

#include "ImGuiManager.h"

#include <memory>
#include <List>

#include "Skydome.h"

#include "CollisionPrimitive.h"

class CollisionManager;
class Player;

class GamePlayScene : public BaseScene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	void Initialize3d();
	void Initialize2d();

	void Update3d();
	void Update2d();

	void Draw3d();
	void Draw2d();

private:
	//基盤
	static DirectXBasis* dxBas_;
	//入力
	static Input* input_;
	//スプライト基盤
	static DrawBasis* drawBas_;
	//ImGuiマネージャー
	static ImGuiManager* imGuiManager_;
	
	//カメラ
	Camera* camera_ = nullptr;
	//ライトグループ
	LightGroup* lightGroup_ = nullptr;


	//当たり判定 レイ
	Ray ray_;

	//衝突マネージャー
	CollisionManager* collisionManager_ = nullptr;

	/// <summary>
	/// オブジェクト
	/// </summary>
	

	Object3d* rayObj_ = nullptr;
	Model* rayModel_ = nullptr;

	Player* player_ = nullptr;
	Model* playerModel_ = nullptr;

	Skydome* skydome_ = nullptr;

	/// <summary>
	/// スプライト
	/// </summary>
	Sprite* sprite_ = new Sprite();

};