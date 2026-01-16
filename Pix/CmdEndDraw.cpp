#include "CmdEndDraw.h"
#include "PrimitivesManager.h"

bool CmmEndDraw::Execute(const std::vector<std::string>& params)
{
	// send vertices to rasterizer
	return PrimitivesManager::Get()->EndDraw();
}

