#pragma once

#include "MathHelper.h"

class Camera
{
public:
	static Camera* Get();
	void OnNewFrame();
	void SetPosition(const Vector3& pos);
	void SetDirection(const Vector3& dir);
	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);
	void SetFOV(float fov);

	Matrix4 GetViewMatrix() const;
	Matrix4 GetProjectionMatrix() const;

private:
	Vector3 mPosition;
	Vector3 mDirection;
	float mNearPlane;
	float mFarPlane;
	float mFOV;

};
