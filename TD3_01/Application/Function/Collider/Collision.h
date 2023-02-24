#pragma once

#include "CollisionPrimitive.h"

class Collision {
public:

	/// <summary>
	///���Ƌ��̏Փ˔���
	/// </summary>
	/// <param name="sphereA">��A</param>
	/// <param name="sphereB">��B</param>
	/// <param name="inter">��_</param>
	/// <returns>�Փ˂��Ă��邩</returns>
	static bool CheckSphere2Sphere(
		const Sphere& sphereA, const Sphere& sphereB, Vector3* inter = nullptr);

	/// <summary>
	//���ƕ��ʂ̌�������
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="plane">����</param>
	/// <param name="inter">��_</param>
	/// <returns>�������Ă��邩</returns>
	static bool CheckSphere2Plane(
		const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);

	/// <summary>
	//�_�ƎO�p�`�̍ŋߐړ_�����߂�
	/// </summary>
	/// <param name="point">�_</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="closest">�ŋߐړ_(�o�͗p)</param>
	static void ClosestPtPoint2Triangle(
		const Vector3& point, const Triangle& triangle, Vector3* closest);

	/// <summary>
	//���Ɩ@���t���O�p�`�̏Փ˔���
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="inter">��_(�O�p�`��̍ŋߐړ_)</param>
	/// <returns>�Փ˂��Ă��邩</returns>
	static bool CheckSphere2Triangle(
		const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr);

	/// <summary>
	/// ���C�ƕ��ʂ̏Փ˔���
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="plane">����</param>
	/// <param name="distance">����(�o��)</param>
	/// <param name="inter">��_(�o�͗p)</param>
	/// <returns>�Փ˂��Ă��邩</returns>
	static bool CheckRay2Plane(
		const Ray& ray, const Plane& plane,
		float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// ���C�Ɩ@���t���O�p�`�̏Փ˔���
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="">�O�p�`</param>
	/// <param name="distance">����(�o��)</param>
	/// <param name="inter">��_(�o�͗p)</param>
	/// <returns>�Փ˂��Ă��邩</returns>
	static bool CheckRay2Triangle(
		const Ray& ray, const Triangle& triangle,
		float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// ���C�Ƌ��̏Փ˔���
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="sphere">��</param>
	/// <param name="distance">����(�o��)</param>
	/// <param name="inter">��_(�o�͗p)</param>
	/// <returns>�Փ˂��Ă��邩</returns>
	static bool CheckRay2Sphere(
		const Ray& ray, const Sphere& sphere,
		float* distance = nullptr, Vector3* inter = nullptr);

private:
	//�덷�z���p�̔����Ȓl
	static const float EPSILON_;
};