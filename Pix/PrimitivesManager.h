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
	None, // no culling used
	Back, // cull items facing away from the camera
	Front // cull items facing towards the camera
};

class PrimitivesManager
{
public:
	static PrimitivesManager* Get();

	void OnNewFrame();
	void SetCullMode(CullMode mode);

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
};