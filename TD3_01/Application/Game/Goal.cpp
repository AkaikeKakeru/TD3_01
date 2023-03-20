#include "Goal.h"
#include "Quaternion.h"
#include "SafeDelete.h"
#include "Input.h"
#include "SphereCollider.h"

#include "CollisionManager.h"
#include "CollisionAttribute.h"

#include <cassert>
#include "CollisionAttribute.h"

CollisionManager* Goal::collisionManager_ = CollisionManager::GetInstance();

Goal* Goal::Create(Model* model) {
	Goal* instance = new Goal();

	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	//モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

bool Goal::Initialize() {
	if (!Object3d::Initialize()) {
		return false;
	}

	isGoal_ = false;

	//コライダ－追加
	float radius = 0.6f;
	//半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(
		Vector3{ 0.0f,radius,0.0f },
		radius)
	);

	collider_->SetAttribute(COLLISION_ATTR_GOAL);

	return true;
}

void Goal::Update() {
	Input* input_ = Input::GetInstance();


	Object3d::Update();
}

void Goal::Draw() {
	Object3d::Draw();
}

void Goal::Finalize() {
	//collider_->RemoveAttribute(COLLISION_ATTR_GOAL);
}

void Goal::OnCollision(const CollisionInfo& info) {
	isGoal_ = true;
}
