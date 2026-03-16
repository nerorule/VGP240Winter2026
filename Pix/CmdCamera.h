#pragma once

#include "Command.h"

class CmdSetCameraPosition : public Command
{
public:
	const char* GetName() override
	{
		return "SetCameraPosition";
	}
	const char* GetDescription() override
	{
		return "SetCameraPosition(x,y,z)\n"
			"\n"
			"- Sets the camera position in world space";
	}
	bool Execute(const std::vector<std::string>& params) override;
};

class CmdSetCameraDirection : public Command
{
public:
	const char* GetName() override
	{
		return "SetCameraDirection";
	}
	const char* GetDescription() override
	{
		return "SetCameraDirection(x,y,z)\n"
			"\n"
			"- Sets the camera direction in world space";
	}
	bool Execute(const std::vector<std::string>& params) override;
};

class CmdSetCameraNearPlane : public Command
{
	public:
	const char* GetName() override
	{
		return "SetCameraNearPlane";
	}
	const char* GetDescription() override
	{
		return "SetCameraNearPlane(nearPlane)\n"
			"\n"
			"- Sets the camera near plane distance";
	}
	bool Execute(const std::vector<std::string>& params) override;
};

class CmdSetCameraFarPlane : public Command
{
public:
const char* GetName() override
{
	return "SetCameraFarPlane";
}
const char* GetDescription() override
{
	return "SetCameraFarPlane(farPlane)\n"
		"\n"
		"- Sets the camera near plane distance";
}
bool Execute(const std::vector<std::string>& params) override;
};

class CmdSetCameraFOV : public Command
{
public:
	const char* GetName() override
	{
		return "SetCameraFov";
	}
	const char* GetDescription() override
	{
		return "SetCameraFov\n"
			"\n"
			"- sets the camera fov\n";
	}
	bool Execute(const std::vector<std::string>& params) override;
};


