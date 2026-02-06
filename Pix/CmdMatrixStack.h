#pragma once

#include "Command.h"

class CmdPushTranslation : public Command
{
public:
	const char* GetName() override
	{
		return "PushTranslation";
	}
	const char* GetDescription() override
	{
		return 
			"PushTranslation(x,y,z)"
			"\n"
			"- pushes a translation matrix to the matrix stack";
	}
	bool Execute(const std::vector<std::string>& params) override;
};

class CmdPushRotationX : public Command
{
	public:
	const char* GetName() override
	{
		return "PushRotationX";
	}
	const char* GetDescription() override
	{
		return 
			"PushRotationX(Degrees)"
			"\n"
			"- pushes a rotation X matrix into the matrix stack";
	}
	bool Execute(const std::vector<std::string>& params) override;
};

class CmdPushRotationY : public Command
{
public:
	const char* GetName() override
	{
		return "PushRotationY";
	}
	const char* GetDescription() override
	{
		return
			"PushRotationY(Degrees)"
			"\n"
			"- pushes a rotation Y matrix into the matrix stack";
	}
	bool Execute(const std::vector<std::string>& params) override;
};

class CmdPushRotationZ : public Command
{
public:
	const char* GetName() override
	{
		return "PushRotationZ";
	}
	const char* GetDescription() override
	{
		return
			"PushRotationZ(Degrees)"
			"\n"
			"- pushes a rotation Z matrix into the matrix stack";
	}
	bool Execute(const std::vector<std::string>& params) override;
};

class CmdPushScaling : public Command
{
	public:
	const char* GetName() override
	{
		return "PushScale";
	}
	const char* GetDescription() override
	{
		return
			"PushScale(x,y,z)"
			"\n"
			"- pushes a scale matrix into the matrix stack";
	}
	bool Execute(const std::vector<std::string>& params) override;
};

class CmdPopMatrix : public Command
{
public:
	const char* GetName() override
	{
		return "PopMatrix";
	}
	const char* GetDescription() override
	{
		return
			"PopMatrix()"
			"\n"
			"- pops the last matrix on the matrix stack";
	}
	bool Execute(const std::vector<std::string>& params) override;
};
