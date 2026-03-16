#include "PrimitivesManager.h"
#include "Rasterizer.h"
#include "Clipper.h"
#include "MatrixStack.h"
#include "Camera.h"
#include "LightManager.h"

extern float gResolutionX;
extern float gResolutionY;

namespace
{
	Matrix4 GetScreenTransform()
	{
		const float hw = gResolutionX * 0.5f;
		const float hh = gResolutionY * 0.5f;
		return {
			  hw, 0.0f, 0.0f, 0.0f,
			0.0f,  -hh, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			  hw,   hh, 0.0f, 1.0f
		};
	}

	Vector3 CreateFaceNormal(const std::vector<Vertex>& triangle)
	{
		// to create a face normal
		// we take the clockwise directions and do a cross product
		// so 0-1(a-b), 0-2(a-c) for the directions
		// cross product
		// return normal
		Vector3 abDir = triangle[1].pos - triangle[0].pos;
		Vector3 acDir = triangle[2].pos - triangle[0].pos;
		Vector3 faceNormal = MathHelper::Normalize(MathHelper::Cross(abDir, acDir));
		return faceNormal;
	}
	bool CullTriangle(CullMode mode, const std::vector<Vertex>& triangleInNDC)
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
	static PrimitivesManager sInstance;
	return &sInstance;
}
PrimitivesManager::PrimitivesManager()
{
}

void PrimitivesManager::OnNewFrame()
{
	mCullMode = CullMode::None;
	mCorrectUV = false;
}
void PrimitivesManager::SetCullMode(CullMode mode)
{
	mCullMode = mode;
}
void PrimitivesManager::SetCorrectUV(bool correctUV)
{
	mCorrectUV = correctUV;
}

bool PrimitivesManager::BeginDraw(Topology topology, bool applyTransform)
{
	mVertexBuffer.clear();
	mTopology = topology;
	mApplyTransform = applyTransform;
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
// Send all the stored vertices to the rasterizer as specified topology
bool PrimitivesManager::EndDraw()
{
	if (!mDrawBegin)
	{
		return false;
	}
	// to start, the triangle vertices are in local space

	// this matrix transforms the local vertices to world space
	Matrix4 matWorld = MatrixStack::Get()->GetTransform();
	// this matrix transforms the world vertices to local space of the camera
	Matrix4 matView = Camera::Get()->GetViewMatrix();
	// this matrix transforms the camera local space vertices to NDC space
	Matrix4 matProj = Camera::Get()->GetProjectionMatrix();
	// this matrix transforms the NDC space vertices to screen space
	Matrix4 matScreen = GetScreenTransform();
	// Get the calculation to NDC space
	Matrix4 matNDC = matWorld * matView * matProj;

	Rasterizer* rasterizer = Rasterizer::Get();
	LightManager* lm = LightManager::Get();

	ShadeMode shadeMode = rasterizer->GetShadeMode();
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
			std::vector<Vertex> triangle = { mVertexBuffer[i - 2], mVertexBuffer[i - 1], mVertexBuffer[i] };
			if (mApplyTransform)
			{
				// if the vertex does not have a normal, give it the face normal
				if (MathHelper::CheckEqual(MathHelper::MagnitudeSquared(triangle[0].norm), 0.0f))
				{
					// calculate the normal in world space
					Vector3 faceNormal = CreateFaceNormal(triangle);
					for (uint32_t v = 0; v < triangle.size(); ++v)
					{
						triangle[v].norm = faceNormal;
					}
				}

				// convert triangle position to world space
				for (uint32_t v = 0; v < triangle.size(); ++v)
				{
					triangle[v].pos = MathHelper::TransformCoord(triangle[v].pos, matWorld);
					triangle[v].posWorld = triangle[v].pos;
					triangle[v].norm = MathHelper::TransformNormal(triangle[v].norm, matWorld);
				}

				// if color.z >= 0, then it is a colored shape, otherwise its a texture
				if (triangle[0].color.z >= 0.0f)
				{
					if (shadeMode == ShadeMode::Flat)
					{
						triangle[0].color *= lm->ComputeLightColor(triangle[0].pos, triangle[0].norm);
						triangle[1].color = triangle[0].color;
						triangle[2].color = triangle[0].color;
					}
					else if (shadeMode == ShadeMode::Gouraud)
					{
						// apply lighting in world space (Gourand Shading)
						for (uint32_t v = 0; v < triangle.size(); ++v)
						{
							triangle[v].color *= lm->ComputeLightColor(triangle[v].pos, triangle[v].norm);
						}
					}
				}
				else if (mCorrectUV)
				{
					// apply the corrective uv in view space (VIEW SPACE)
					// at this point, we are in world space, so next step is
					// multiply by matView
					for (uint32_t v = 0; v < triangle.size(); ++v)
					{
						Vector3 viewSpacePos = MathHelper::TransformCoord(triangle[v].pos, matView);
						triangle[v].color.x /= viewSpacePos.z;
						triangle[v].color.y /= viewSpacePos.z;
						triangle[v].color.w = 1.0 / viewSpacePos.z;
					}
				}

				// convert triangle position to NDC space
				for (uint32_t v = 0; v < triangle.size(); ++v)
				{
					triangle[v].pos = MathHelper::TransformCoord(triangle[v].pos, matNDC);
				}

				// while in NDC space, we can see if the face is facing the camera or away
				if (CullTriangle(mCullMode, triangle))
				{
					continue;
				}

				// convert ndc space triangles to Screen space
				for (uint32_t v = 0; v < triangle.size(); ++v)
				{
					triangle[v].pos = MathHelper::TransformCoord(triangle[v].pos, matScreen);
					// Flatten only on screen space and only pixel x, y values
					MathHelper::FlattenVectorScreenCoord(triangle[v].pos);
				}
			}
			if (!Clipper::Get()->ClipTriangle(triangle))
			{
				for (uint32_t v = 2; v < triangle.size(); ++v)
				{
					rasterizer->DrawTriangle(triangle[0], triangle[v - 1], triangle[v]);
				}
			}
		}
	}
	break;
	default:
		return false;
	}
}