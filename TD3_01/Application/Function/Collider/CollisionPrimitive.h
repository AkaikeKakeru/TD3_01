#pragma once
#include "Vector3.h"

//��
struct Sphere {
	//���S
	Vector3 center_ = { 0,0,0 };

	//���a
	float radius_ = 1.0f;
};

//����
struct Plane {
	//�@��
	Vector3 normal_ = { 0,1,0 };

	//���_����̋���
	float distance_ = 0.0f;
};

//�@���t���O�p�`(���v��肪�\��)
class Triangle{
public:
	//�@���̎Z�o
	void ComputeNormal();

public:
	//���_���W3��
	Vector3 p0_;
	Vector3 p1_;
	Vector3 p2_;
	//�@���x�N�g��
	Vector3 normal_;
};

//���C(������)
class Ray{
public:
	//�n�_���W
	Vector3 start_ = {0,0,0};
	//����
	Vector3 dir_ = {1,0,0};
};
