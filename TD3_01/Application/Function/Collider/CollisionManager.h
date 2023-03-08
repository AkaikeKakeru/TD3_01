#pragma once

#include <forward_list>
#include "CollisionPrimitive.h"
#include "RaycastHit.h"

class BaseCollider;

class CollisionManager {
public: //�ÓI�����o�֐�
	static CollisionManager* GetInstance();

public: //�����o�֐�
	//�R���C�_�[�ǉ�
	inline void AddCollider(BaseCollider* collider) {
		colliders_.push_front(collider);
	}

	//�R���C�_�[�̍폜
	inline void RemoveCollider(BaseCollider* collider) {
		colliders_.remove(collider);
	}

	//�S�Ă̏Փ˃`�F�b�N
	void CheckAllCollisions();

	//�Ƃ��ɋ��̎�
	void CheckSphere2Sphere(BaseCollider* colA ,BaseCollider* colB);

	//���C�L���X�g
	bool Raycast(const Ray& ray, RaycastHit* hitInfo = nullptr,
		float maxDistance = D3D12_FLOAT32_MAX);
	//���C�L���X�g(�Փˑ�������)
	bool Raycast(const Ray& ray,unsigned short attribute,
		RaycastHit* hitInfo = nullptr,
		float maxDistance = D3D12_FLOAT32_MAX);

private: //�����o�ϐ�
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator =(const CollisionManager&) = delete;
	//�R���C�_�[�̃��X�g
	std::forward_list<BaseCollider*> colliders_;
};