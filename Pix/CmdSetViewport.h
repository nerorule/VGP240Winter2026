#pragma once

#include "Command.h"

class CmdSetViewport : public Command
{
public:
	const char* GetName() override
	{
		return "SetViewport";
	}
	const char* GetDescription() override
	{
		return
			"SetViewport(x, y, width, height, <show>) \n"
			"\n"
			"- create a viewport";
	}
	bool Execute(const std::vector<std::string>& params) override;
};