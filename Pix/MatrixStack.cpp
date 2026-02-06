#include "MatrixStack.h"

MatrixStack* MatrixStack::Get()
{
	static MatrixStack instance;
	return &instance;
}

void MatrixStack::OnNewFrame()
{
	mCombineTransform = Matrix4::Identity();
	mMatrices.clear();
}

void MatrixStack::PushTranslation(const Vector3& d)
{
	Matrix4 translationMatrix = Matrix4::Translation(d);
	mMatrices.push_back(translationMatrix);
	mCombineTransform = translationMatrix * mCombineTransform;
}

void MatrixStack::PushRotationX(float radians)
{
	Matrix4 rotationMatrix = Matrix4::RotationX(radians);
	mMatrices.push_back(rotationMatrix);
	mCombineTransform = rotationMatrix * mCombineTransform;
}

void MatrixStack::PushRotationY(float radians)
{
	Matrix4 rotationMatrix = Matrix4::RotationY(radians);
	mMatrices.push_back(rotationMatrix);
	mCombineTransform = rotationMatrix * mCombineTransform;
}

void MatrixStack::PushRotationZ(float radians)
{
	Matrix4 rotationMatrix = Matrix4::RotationZ(radians);
	mMatrices.push_back(rotationMatrix);
	mCombineTransform = rotationMatrix * mCombineTransform;
}

void MatrixStack::PushScale(const Vector3& s)
{
	Matrix4 scaleMatrix = Matrix4::Scale(s);
	mMatrices.push_back(scaleMatrix);
	mCombineTransform = scaleMatrix * mCombineTransform;
}

void MatrixStack::PopMatrix()
{
	if (!mMatrices.empty())
	{
		Matrix4 matrix = mMatrices.back();
		mMatrices.pop_back();
		mCombineTransform = MathHelper::Inverse(matrix) * mCombineTransform;
	}
}

const Matrix4& MatrixStack::GetTransform() const
{
	return mCombineTransform;
}