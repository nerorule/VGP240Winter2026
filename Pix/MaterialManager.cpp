#include "MaterialManager.h"

MaterialManager* MaterialManager::Get()
{
	static MaterialManager sInstance;
	return &sInstance;
}

void MaterialManager::OnNewFrame()
{
	mEmmissive = X::Colors::Black;
	mAmbient = X::Colors::White;
	mDiffuse = X::Colors::White;
	mSpecular = X::Colors::White;
	mShininess = 10.0f;
}

void MaterialManager::SetMaterialEmmissive(const X::Color& color)
{
	mEmmissive = color;
}

void MaterialManager::SetMaterialAmbient(const X::Color& color)
{
	mAmbient = color;
}

void MaterialManager::SetMaterialDiffuse(const X::Color& color)
{
	mDiffuse = color;
}

void MaterialManager::SetMaterialSpecular(const X::Color& color)
{
	mSpecular = color;
}

void MaterialManager::SetMaterialShininess(float shininess)
{
	mShininess = shininess;
}

const X::Color& MaterialManager::GetMaterialEmmissive() const
{
	return mEmmissive;
}

const X::Color& MaterialManager::GetMaterialAmbient() const
{
	return mAmbient;
}

const X::Color& MaterialManager::GetMaterialDiffuse() const
{
	return mDiffuse;
}

const X::Color& MaterialManager::GetMaterialSpecular() const
{
	return mSpecular;
}

const float MaterialManager::GetMaterialShininess() const
{
	return mShininess;
}
