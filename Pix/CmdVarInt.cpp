#include "CmdVarInt.h"
#include "VariableCache.h"

bool CmdVarInt::Execute(const std::vector<std::string>& params)
{
	if (params.size() < 3)
	{
		return false;
	}

	VariableCache* vc = VariableCache::Get();
	if (vc->IsVarName(params[0]) || params[1] != "=")
	{
		return false;
	}

	const int value = std::stoi(params[2]);
	const float speed = params.size() > 3 ? std::stof(params[3]) : 1.0f;
	const int min = params.size() > 4 ? std::stoi(params[4]) : -INT_MAX;
	const int max = params.size() > 5 ? std::stoi(params[5]) : INT_MAX;

	vc->AddInt(params[0], value, speed, min, max);
	return true;
}