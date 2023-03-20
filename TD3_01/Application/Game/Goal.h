#pragma once
#include "Model.h"
#include "Object3d.h"
#include "Camera.h"

class CollisionManager;

class Goal	
	: public Object3d {
public: //�ÓI�����o�֐�
		//�I�u�W�F�N�g����
	static Goal* Create(Model* model = nullptr);

public://�����o�֐�
	bool Initialize();
	void Update();
	void Draw();
	void Finalize();

	//�Փˎ��R�[���o�b�N�֐�
	void OnCollision(const CollisionInfo & info) override;

	//�S�[���t���O�擾
	bool GetIsGoal() {
		return isGoal_;
	}

private://�����o�ϐ�
	Object3d* object_ = nullptr;
	Model* model_ = nullptr;
	Camera* camera_ = nullptr;

private:
	//�Փ˃}�l�[�W���[
	static CollisionManager* collisionManager_;

	//�S�[���������t���O
	bool isGoal_ = false;

public:
	Goal() = default;
	~Goal() = default;
};
