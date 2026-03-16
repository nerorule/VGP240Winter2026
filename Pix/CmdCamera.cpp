#include "CmdCamera.h"
#include "Command.h"
#include "VariableCache.h"
#include "Camera.h"
#include "MathHelper.h"

bool CmdSetCameraPosition::Execute(const std::vector<std::string>& params)
{
	// Need x, y, z
	if (params.size() < 3)
	{
		return false;
	}

	VariableCache* vc = VariableCache::Get();
	const float x = vc->GetFloat(params[0]);
	const float y = vc->GetFloat(params[1]);
	const float z = vc->GetFloat(params[2]);

	Camera::Get()->SetPosition({ x, y, z });
	return true;
}

bool CmdSetCameraNearPlane::Execute(const std::vector<std::string>& params)
{
	// Need near plane value
	if (params.size() < 1)
	{
		return false;
	}

	const float nearPlane = VariableCache::Get()->GetFloat(params[0]);
	Camera::Get()->SetNearPlane(nearPlane);
	return true;
}

bool CmdSetCameraFarPlane::Execute(const std::vector<std::string>& params)
{
	// Need far plane value
	if (params.size() < 1)
	{
		return false;
	}

	const float farPlane = VariableCache::Get()->GetFloat(params[0]);
	Camera::Get()->SetFarPlane(farPlane);
	return true;
}

bool CmdSetCameraFOV::Execute(const std::vector<std::string>& params)
{
	// Need fov value (interpreted as degrees, converted to radians)
	if (params.size() < 1)
	{
		return false;
	}

	const float fovDegrees = VariableCache::Get()->GetFloat(params[0]);
	const float fovRadians = fovDegrees * MathHelper::DegToRad;
	Camera::Get()->SetFOV(fovRadians);
	return true;
}

bool CmdSetCameraDirection::Execute(const std::vector<std::string>& params)
{
	// Need x, y, z
	if (params.size() < 3)
	{
		return false;
	}

	VariableCache* vc = VariableCache::Get();
	const float x = vc->GetFloat(params[0]);
	const float y = vc->GetFloat(params[1]);
	const float z = vc->GetFloat(params[2]);

	Camera::Get()->SetDirection({ x, y, z });
	return true;
}