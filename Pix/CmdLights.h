#pragma once

#include "Command.h"

class CmdSetLightAmbient : public Command
{
public:
	const char* GetName() override
	{
		return "SetLightAmbient";
	}
	const char* GetDescription() override
	{
		return
			"Set Light Ambient. \n "
			"\n"
			"- sets the ambient color for all lights. \n";
	}
	bool Execute(const std::vector<std::string>& params) override;
};

class CmdSetLightDiffuse : public Command
{
	const char* GetName() override
	{
		return "SetLightDiffuse";
	}
	const char* GetDescription() override
	{
		return
			"Set Light Ambient. \n "
			"\n"
			"- sets the ambient color for all lights. \n";
	}
	bool Execute(const std::vector<std::string>& params) override;
};

class CmdSetLightSpecular : public Command
{
	const char* GetName() override
	{
		return "SetLightSpecular";
	}
	const char* GetDescription() override
	{
		return
			"Set Light Specular. \n "
			"\n"
			"- sets the specular color for all lights. \n";
	}
	bool Execute(const std::vector<std::string>& params) override;
};

class CmdAddDirectionalLight : public Command
{
	const char* GetName() override
	{
		return "AddDirectionalLight";
	}
	const char* GetDescription() override
	{
		return
			"Add Directional Light. (x, y, z)\n "
			"\n"
			"- adds a directional light with the current light ambient, diffuse, and specular colors. \n"
			"- the direction is specified in world space. \n";
	}
	bool Execute(const std::vector<std::string>& params) override;
};

class CmdAddPointLight : public Command
{
	const char* GetName() override
	{
		return "AddPointLight";
	}
	const char* GetDescription() override
	{
		return
			"Add Point Light. (x, y, z, <Constant>, <Linear>, <Quadratic>)\n "
			"\n"
			"- adds a point light with the current light ambient, diffuse, and specular colors. \n"
			"- the position is specified in world space. \n"
			"- attenuation is specified by the constant, linear, and quadratic terms. \n";
	}
	bool Execute(const std::vector<std::string>& params) override;
};

class CmdAddSpotLight : public Command
{
	const char* GetName() override
	{
		return "AddSpotLight";
	}
	const char* GetDescription() override
	{
		return
			"Add Spot Light. (x, y, z, xDir, yDir, zDir, kConstant, kLinear, kQuadratic, angle, decay)\n "
			"\n"
			"- adds a spot light with the current light ambient, diffuse, and specular colors. \n"
			"- the position and direction are specified in world space. \n"
			"- attenuation is specified by the constant, linear, and quadratic terms. \n"
			"- the angle is specified in degrees and is the full angle of the cone. \n"
			"- decay is the rate at which the light intensity falls off as it approaches the edge of the cone. \n";
	}
	bool Execute(const std::vector<std::string>& params) override;
};
