#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"

namespace MathHelper
{
	constexpr float DegToRad = 3.1416f / 180.0f;

	bool CheckEqual(float a, float b);

	void FlattenVectorScreenCoord(Vector3& v);

	float MagnitudeSquared(const Vector2& v);
	float MagnitudeSquared(const Vector3& v);

	float Magnitude(const Vector2& v);
	float Magnitude(const Vector3& v);

	Vector2 Normalize(const Vector2& v);
	Vector3 Normalize(const Vector3& v);

	float Dot(const Vector2& a, const Vector2& b);
	float Dot(const Vector3& a, const Vector3& b);

	Vector3 Cross(const Vector3& a, const Vector3& b);

	// translates vector position and rotation
	Vector3 TransformCoord(const Vector3& v, const Matrix4& m);
	// only translates vector rotation
	Vector3 TransformNormal(const Vector3& v, const Matrix4& m);

	// inverse matrix, used to remove matrix, m* inverse(m) = Identity
	Matrix4 Inverse(const Matrix4& m);
	// flips matrix, generally for swapping between row-major and column-major
	Matrix4 Transpose(const Matrix4& m);
	// used to get the inverse matrix
	Matrix4 Adjoint(const Matrix4& m);
	// used in calculating the inverse matrix
	float Determinant(const Matrix4& m);
}