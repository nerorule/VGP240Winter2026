#pragma once

#include "Command.h"

class CmdSetCorrectUV : public Command
{
public:
	const char* GetName() override
	{
		return "SetCorrectUV";
	}
	const char* GetDescription() override
	{
		return
			"SetCorrectUV(CorrectUV)\n"
			"\n"
			"- enables/disables correct uv for textured objects";
	}
	bool Execute(const std::vector<std::string>& params) override;
};