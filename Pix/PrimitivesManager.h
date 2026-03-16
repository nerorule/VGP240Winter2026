#pragma once

#include "Vertex.h"

enum class Topology
{
	Point,
	Line,
	Triangle
};

enum class CullMode
{
	None,  // no culling used
	Back,  // cull items facing away from camera
	Front  // cull items facing the camera
};

class PrimitivesManager
{
public:
	static PrimitivesManager* Get();

	void OnNewFrame();
	void SetCullMode(CullMode mode);
	void SetCorrectUV(bool correctUV);

	// Start accepting vertices
	bool BeginDraw(Topology topology, bool applyTransform = false);
	// Add vertices to the list, only if drawing is enabled
	void AddVertex(const Vertex& vertex);
	// Send all the stored vertices to the rasterizer as specified topology
	bool EndDraw();

private:
	PrimitivesManager();

	std::vector<Vertex> mVertexBuffer;
	Topology mTopology = Topology::Point;
	CullMode mCullMode = CullMode::None;
	bool mDrawBegin = false;
	bool mApplyTransform = false;
	bool mCorrectUV = false;
};