#pragma once

#include "Command.h"

class CmdBeginDraw : public Command
{
public:
	const char* GetName() override
	{
		return "BeginDraw";
	}
	const char* GetDescription() override
	{
		return "BeginDraw(topology)\n"
			"\n"
			"- Starts storing vertices\n"
			"- stores topology type (Point, Line, Triangle)";
	}
	bool Execute(const std::vector<std::string>& params) override;
};
