#include "CmdLights.h"
#include "LightManager.h"
#include "VariableCache.h"

bool CmdSetLightAmbient::Execute(const std::vector<std::string>& params)
{
	if (params.size() < 3)
	{
		return false;
	}

	VariableCache* vc = VariableCache::Get();
	float r = vc->GetFloat(params[0]);
	float g = vc->GetFloat(params[1]);
	float b = vc->GetFloat(params[2]);
	LightManager::Get()->SetLightAmbient({ r, g, b, 1.0f });
	return true;
}

bool CmdSetLightDiffuse::Execute(const std::vector<std::string>& params)
{
	if (params.size() < 3)
	{
		return false;
	}

	VariableCache* vc = VariableCache::Get();
	float r = vc->GetFloat(params[0]);
	float g = vc->GetFloat(params[1]);
	float b = vc->GetFloat(params[2]);
	LightManager::Get()->SetLightAmbient({ r, g, b, 1.0f });
	return true;
}
bool CmdSetLightSpecular::Execute(const std::vector<std::string>& params)
{
	if (params.size() < 3)
	{
		return false;
	}

	VariableCache* vc = VariableCache::Get();
	float r = vc->GetFloat(params[0]);
	float g = vc->GetFloat(params[1]);
	float b = vc->GetFloat(params[2]);
	LightManager::Get()->SetLightAmbient({ r, g, b, 1.0f });
	return true;
}
bool CmdAddDirectionalLight::Execute(const std::vector<std::string>& params)
{
	if (params.size() < 3)
	{
		return false;
	}

	VariableCache* vc = VariableCache::Get();
	float x = vc->GetFloat(params[0]);
	float y = vc->GetFloat(params[1]);
	float z = vc->GetFloat(params[2]);
	LightManager::Get()->AddDirectionalLight({ x, y, z });
	return true;
}
bool CmdAddPointLight::Execute(const std::vector<std::string>& params)
{
	if (params.size() < 3)
	{
		return false;
	}
	VariableCache* vc = VariableCache::Get();
	float x = vc->GetFloat(params[0]);
	float y = vc->GetFloat(params[1]);
	float z = vc->GetFloat(params[2]);
	float kConstant = 1.0f;
	float kLinear = 0.0f;
	float kQuad = 0.0f;
	if (params.size() > 3)
	{
		kConstant = vc->GetFloat(params[3]);
	}
	if (params.size() > 4)
	{
		kLinear = vc->GetFloat(params[4]);
	}
	if (params.size() > 5)
	{
		kQuad = vc->GetFloat(params[5]);
	}
	LightManager::Get()->AddPointLight({ x, y, z }, kConstant, kLinear, kQuad);
	return true;
}
bool CmdAddSpotLight::Execute(const std::vector<std::string>& params)
{
	if (params.size() < 6)
	{
		return false;
	}
	VariableCache* vc = VariableCache::Get();
	float px = vc->GetFloat(params[0]);
	float py = vc->GetFloat(params[1]);
	float pz = vc->GetFloat(params[2]);

	float dx = vc->GetFloat(params[3]);
	float dy = vc->GetFloat(params[4]);
	float dz = vc->GetFloat(params[5]);

	float kConstant = 1.0f;
	float kLinear = 0.0f;
	float kQuad = 0.0f;
	float angle = 0.0f;
	float decay = 0.0f;
	if (params.size() > 6)
	{
		kConstant = vc->GetFloat(params[6]);
	}
	if (params.size() > 7)
	{
		kLinear = vc->GetFloat(params[7]);
	}
	if (params.size() > 8)
	{
		kQuad = vc->GetFloat(params[8]);
	}
	if (params.size() > 9)
	{
		angle = vc->GetFloat(params[9]) * X::Math::kDegToRad;
	}
	if (params.size() > 10)
	{
		decay = vc->GetFloat(params[10]);
	}
	LightManager::Get()->AddSpotLight({ px, py, pz }, { dx, dy, dz }, kConstant, kLinear, kQuad, angle, decay);
	return true;
}

