#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"

CollisionManager* CollisionManager::GetInstance() {
	static CollisionManager instance;
	return &instance;
}

void CollisionManager::CheckAllCollisions() {
	std::forward_list<BaseCollider*>::iterator itA;
	std::forward_list<BaseCollider*>::iterator itB;

	//�S�Ă̑g�ݍ��킹�ɂ��đ�������
	itA = colliders_.begin();
	for (; itA != colliders_.end(); ++itA) {
		itB = itA;
		++itB;
		for (; itB != colliders_.end(); ++itB) {
			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

			if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE
				&& colB->GetShapeType() == COLLISIONSHAPE_SPHERE) {
				//�Ƃ��ɋ�
				CheckSphere2Sphere(colA, colB);
			}
		}
	}
}

void CollisionManager::CheckSphere2Sphere(BaseCollider* colA, BaseCollider* colB) {
	Sphere* SphereA = dynamic_cast<Sphere*>(colA);
	Sphere* SphereB = dynamic_cast<Sphere*>(colB);
	Vector3 inter;

	if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter)) {
		colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
		colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
	}
}

bool CollisionManager::Raycast(const Ray& ray, RaycastHit* hitInfo,
	float maxDistance) {
	return Raycast(ray,0xffff,hitInfo,maxDistance);
}

bool CollisionManager::Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo, float maxDistance) {
	bool result = false;

	//�����p�C�e���[�^
	std::forward_list<BaseCollider*>::iterator it;
	//���܂łōł��߂��R���C�_�[���L�^���邽�߂̃C�e���[�^
	std::forward_list<BaseCollider*>::iterator it_hit;
	//���܂łōł��߂��R���C�_�[�̋������L�^����ϐ�
	float distance = maxDistance;
	//���܂łōł��߂��R���C�_�[�Ƃ̌�_���L�^���邽�߂̕ϐ�
	Vector3 inter;

	//�S�ẴR���C�_�[�Ƒ�������`�F�b�N
	it = colliders_.begin();
	for (; it != colliders_.end(); ++it) {
		BaseCollider* colA = *it;

		//����������Ȃ���΃X�L�b�v
		if (!(colA->attribute_ & attribute)) {
			continue;
		}

		//���̏ꍇ
		if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE) {
			Sphere* sphere = dynamic_cast<Sphere*>(colA);
			float tempDistance;
			Vector3 tempInter;

			//������Ȃ���Ώ��O
			if (!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempInter))continue;

			//�������ŏ��łȂ���Ώ��O
			if (tempDistance >= distance)continue;

			//���܂łōł��߂��̂ŋL�^�����
			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
	}

	//�ŏI�I�ɉ����ɓ������Ă����猋�ʂ���������
	if (result && hitInfo) {
		hitInfo->distance_ = distance;
		hitInfo->inter_ = inter;
		hitInfo->collider_ = *it_hit;
		hitInfo->object_ = hitInfo->collider_->GetObject3d();
	}

	return result;
}
