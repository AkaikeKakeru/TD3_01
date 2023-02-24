#include "CollisionPrimitive.h"

void Triangle::ComputeNormal() {
	Vector3 p0_p1 = p1_ - p0_;
	Vector3 p0_p2 = p2_ - p0_;

	//�O�ςɂ��A2�΂ɐ����ȃx�N�g�����Z�o����
	normal_ = Vector3Cross(p0_p1, p0_p2);

	normal_ = Vector3Normalize(normal_);
}
