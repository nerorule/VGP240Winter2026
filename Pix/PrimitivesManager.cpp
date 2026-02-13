#include "PrimitivesManager.h"
#include "Rasterizer.h"
#include "Clipper.h"
#include "MatrixStack.h"
#include "Camera.h"

extern float gResolutionX;
extern float gResolutionY;

namespace
{
	Matrix4 GetScreenTransform()
	{
		const float hw = gResolutionX * 0.5f;
		const float hh = gResolutionY * 0.05f;
		return {
			hw, 0.0f, 0.0f, 0.0f,
			0.0f, -hh, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			hw, hh, 0.0f, 1.0f
		};
	}

	Vector3 CreateFaceNormal(const std::vector<Vertex>& traingles)
	{
		// to create a face normal
		// we take the clockwise direction and do a cross product
		// so 0-1(a-b), 0-2(a-c) for the directions
		// crooss product
		// return normal
		Vector3 abDir = traingles[1].pos - traingles[0].pos;
		Vector3 acDir = traingles[2].pos - traingles[0].pos;
		Vector3 faceNormal = MathHelper::Normalize(MathHelper::Cross(abDir, acDir));
		return faceNormal;
	}
	bool CullTraingle(CullMode mode, const std::vector<Vertex>& triangleInNDC)
	{
		if (mode == CullMode::None)
		{
			return false;
		}
		Vector3 faceNormal = CreateFaceNormal(triangleInNDC);
		if (mode == CullMode::Back)
		{
			return faceNormal.z > 0.0f;
		}
		if (mode == CullMode::Front)
		{
			return faceNormal.z < 0.0f;
		}
		return false;
	}
}

PrimitivesManager* PrimitivesManager::Get()
{
	// Singleton instance (only called once)
	static PrimitivesManager sInstance;
	// returs the instance
	return &sInstance;
}

PrimitivesManager::PrimitivesManager()
{
	
}

void PrimitivesManager::OnNewFrame()
{
	mCullMode = CullMode::Back;
}

void PrimitivesManager::SetCullMode(CullMode mode)
{
	mCullMode = mode;
}

bool PrimitivesManager::BeginDraw(Topology topology, bool applyTransformation)
{
	mVertexBuffer.clear();
	mTopology = topology;
	mApplyTransform = applyTransformation;
	mDrawBegin = true;
	return true;
}

void PrimitivesManager::AddVertex(const Vertex& vertex)
{
	if (mDrawBegin)
	{
		mVertexBuffer.push_back(vertex);
	}
}

bool PrimitivesManager::EndDraw()
{
	if (!mDrawBegin)
	{
		return false;
	}
	// to start, the triangle vertices are in local space

	// this matrix transforms the local vertices to the world space
	Matrix4 matWorld = MatrixStack::Get()->GetTransform();
	// this matrix transforms the world space to the view space
	Matrix4 matView = Camera::Get()->GetViewMatrix();
	// this matrix transforms the view space to the projection space
	Matrix4 matProj = Camera::Get()->GetProjectionMatrix();
	// this matrix transforms the projection space to the screen space
	Matrix4 matScreen = GetScreenTransform();
	// this will get the calculation to NDC space, which is the space after projection and before screen transform
	Matrix4 matNDC = matWorld * matView * matProj;

	Rasterizer* rasterizer = Rasterizer::Get();
	switch (mTopology)
	{
	case Topology::Point:
	{
		for (uint32_t i = 0; i < mVertexBuffer.size(); ++i)
		{
			if (!Clipper::Get()->ClipPoint(mVertexBuffer[i]))
			{
				rasterizer->DrawPoint(mVertexBuffer[i]);
			}
		}
	}
	break;
	case Topology::Line:
	{
		for (uint32_t i = 1; i < mVertexBuffer.size(); i += 2)
		{
			if (!Clipper::Get()->ClipLine(mVertexBuffer[i - 1], mVertexBuffer[i]))
			{
				rasterizer->DrawLine(mVertexBuffer[i - 1], mVertexBuffer[i]);
			}
		}
	}
	break;
	case Topology::Triangle:
	{
		for (uint32_t i = 2; i < mVertexBuffer.size(); i += 3)
		{
			std::vector<Vertex> traingle = { mVertexBuffer[i - 2], mVertexBuffer[i - 1], mVertexBuffer[i] };
			if (mApplyTransform)
			{
				// convert triangle position to NDC space
				for (uint32_t v = 0; v < traingle.size(); ++v)
				{
					traingle[v].pos = MathHelper::TransformCoord(traingle[v].pos, matNDC);
				}

				// while in ndc space, we can see if the face is facing
				if (CullTraingle(mCullMode, traingle))
				{
					continue;
				}

				// convert ndc space to screen space
				for (uint32_t v = 0; v < traingle.size(); ++v)
				{
					traingle[v].pos = MathHelper::TransformCoord(traingle[v].pos, matScreen);
					// flatten only on screen space and only pixel x,y values
					MathHelper::FlattenVectorScreenCoord(traingle[v].pos);
				}
			}
			if (!Clipper::Get()->ClipTriangle(traingle))
			{
				for (uint32_t v = 2; v < traingle.size(); ++v)
				{
					rasterizer->DrawTriangle(traingle[0], traingle[v - 1], traingle[v]);
				}
			}
		}
	}
	break;
	default:
		break;
	}
}