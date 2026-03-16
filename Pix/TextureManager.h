#pragma once

#include "Texture.h"

class TextureManager
{
public:
	static TextureManager* Get();

	void Clear();
	void SetTexture(const std::string& fileName);

	X::Color SampleColor(const X::Color& uv) const;

private:
	std::vector<std::unique_ptr<Texture>> mTextures;
	const Texture* mCurrentTexture = nullptr;
};
