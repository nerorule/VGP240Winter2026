#pragma once

#include "Command.h"

class CmdSetTexture : public Command
{
public:
	const char* GetName() override
	{
		return "SetTexture";
	}
	const char* GetDescription() override
	{
		return
			"Set Texture (File name)\n "
			"\n"
			"- sets the texture to use on the object";
	}
	bool Execute(const std::vector<std::string>& params) override;
};
