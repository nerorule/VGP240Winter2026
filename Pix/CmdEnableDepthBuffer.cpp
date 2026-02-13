#include "CmdEnableDepthBuffer.h"
#include "DepthBuffer.h"
#include "VariableCache.h"

bool CmdEnableDepthBuffer::Execute(const std::vector<std::string>& params)
{
	if (params.size() < 1)
	{
		return false;
	}

	VariableCache* vc = VariableCache::Get();
	bool enable = vc->GetBool(params[0])   ;
	DepthBuffer::Get()->SetEnabled(enable);
	return true;
}