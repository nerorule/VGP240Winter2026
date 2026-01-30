#include "Clipper.h"
#include "Viewport.h"

const short BIT_INSIDE = 0;      // 0000
const short BIT_LEFT = 1 << 1;   // 0001
const short BIT_RIGHT = 1 << 2;  // 0010
const short BIT_BOTTOM = 1 << 3; // 0100
const short BIT_TOP = 1 << 4;    // 1000

short GetOutputCode(float x, float y)
{
	Viewport* vp = Viewport::Get();

	short code = BIT_INSIDE;
	if (x < vp->GetMinX())
	{
		code |= BIT_LEFT;
	}
	else if (x > vp->GetMaxY())
	{
		code |= BIT_RIGHT;
	}
	if (y < vp->GetMinY())
	{
		code |= BIT_TOP;
	}
	else if (y > vp->GetMaxY())
	{
		code |= BIT_BOTTOM;
	}
	return code;
}

enum ClipEdge
{
	CE_LEFT,
	CE_TOP,
	CE_RIGHT,
	CE_BOTTOM,
	CE_COUNT  // can be used to index trough the enum
};

bool IsInFront(ClipEdge edge, const Vector3& pos)
{
	Viewport* vp = Viewport::Get();
	switch (edge)
	{
	case CE_LEFT: return pos.x > vp->GetMinX();
	case CE_TOP: return pos.y > vp->GetMinY();
	case CE_RIGHT: return pos.x < vp->GetMaxX();
	case CE_BOTTOM: return pos.y < vp->GetMaxY();
	default:
		break;
	}
	return false;
}

Vertex ComputeIntersection(ClipEdge edge, const Vertex& v, const Vertex& vPO)
{
	Viewport* vp = Viewport::Get();
	float t = 0.0f;
	switch (edge)
	{
	case CE_LEFT: t = (vp->GetMinX() - v.pos.x) / (vPO.pos.x - v.pos.x); break;
	case CE_TOP: t = (vp->GetMinY() - v.pos.y) / (vPO.pos.y - v.pos.y); break;
	case CE_RIGHT: t = (vp->GetMaxX() - v.pos.x) / (vPO.pos.x - v.pos.x); break;
	case CE_BOTTOM: t = (vp->GetMaxX() - v.pos.y) / (vPO.pos.y - v.pos.y); break;
	default:
		break;
	}
	return LerpVertex(v, vPO, t);
}

Clipper::Clipper()
{

}

Clipper* Clipper::Get()
{
	static Clipper sInstance;
	return &sInstance;
}

void Clipper::OnNewFrame()
{
	mIsClipping = false;
}

bool Clipper::ClipPoint(const Vertex& v)
{
	if (!mIsClipping)
	{
		return false;
	}
	Viewport* vp = Viewport::Get();
	float minX = vp->GetMinX();
	float minY = vp->GetMinY();
	float maxX = vp->GetMaxX();
	float maxY = vp->GetMaxY();

	return v.pos.x < minX || v.pos.x > maxX || v.pos.y < minY || v.pos.y > maxY;;
}
bool Clipper::ClipLine(Vertex& a, Vertex& b)
{
	if (!mIsClipping)
	{
		return false;
	}

	Viewport* vp = Viewport::Get();
	float minX = vp->GetMinX();
	float minY = vp->GetMinY();
	float maxX = vp->GetMaxX();
	float maxY = vp->GetMaxY();

	short codeA = GetOutputCode(a.pos.x, a.pos.y);
	short codeB = GetOutputCode(b.pos.x, b.pos.y);
	while (true)
	{
		if (!(codeA || codeB))
		{
			// if both a and b are 0000, then draw the line
			break;
		}
		else if (codeA & codeB)
		{
			// both lines are on the same side, so not crossing the screen
			break;
		}

		float t = 0.0f;
		short outCode = codeB > codeA ? codeB : codeA;
		if (outCode & BIT_TOP) { t = (minY - a.pos.y) / (b.pos.y - a.pos.y); }
		else if (outCode & BIT_BOTTOM) { t = (maxY - a.pos.y) / (b.pos.y - a.pos.y);}
		else if (outCode & BIT_LEFT) { t = (minX - a.pos.x) / (b.pos.x - a.pos.x); }
		else if (outCode & BIT_RIGHT) { t = (maxY - a.pos.x) / (b.pos.x - a.pos.x); }

		if (outCode == codeA)
		{
			a = LerpVertex(a, b, t);
			codeA = GetOutputCode(a.pos.x, a.pos.y);
		}
		else
		{
			b = LerpVertex(a, b, t);
			codeB = GetOutputCode(b.pos.x, b.pos.y);
		}
	}

	// if codeA or codeB are not 0000, it not on the screen
	return (codeA | codeB);
}
bool Clipper::ClipTriangle(std::vector<Vertex>& vertices)
{
	// detect if triangle is at least partially outside and enable clipping for the frame
	Viewport* vp = Viewport::Get();
	float minX = vp->GetMinX();
	float minY = vp->GetMinY();
	float maxX = vp->GetMaxX();
	float maxY = vp->GetMaxY();

	// if any vertex lies outside the viewport, enable clipping
	for (const Vertex& v : vertices)
	{
		if (v.pos.x < minX || v.pos.x > maxX || v.pos.y < minY || v.pos.y > maxY)
		{
			mIsClipping = true;
			break;
		}
	}

	// If no clipping was detected we still run the algorithm to produce the correct return value.
	std::vector<Vertex> newVertices;
	for (int i = 0; i < CE_COUNT; ++i)
	{
		newVertices.clear();
		ClipEdge edge = (ClipEdge)i;
		for (size_t n = 0; n < vertices.size(); ++n)
		{
			size_t nPO = (n + 1) % vertices.size();
			//original vertex
			const Vertex& v = vertices[n];
			//veretex plus one
			const Vertex& vPO = vertices[nPO];

			bool nIsInFront = IsInFront(edge, v.pos);
			bool nPoIsInFront = IsInFront(edge, vPO.pos);

			// case 1 both are in front
			if (nIsInFront && nPoIsInFront)
			{
				newVertices.push_back(vPO);
			}
			// case 2 both are behind
			else if (!nIsInFront && !nPoIsInFront)
			{
				//we dont save anything as they are off screen
			}
			// case 3 v is in front vPO is behind
			else if (nIsInFront && !nPoIsInFront)
			{
				//only save the intersaction
				newVertices.push_back(ComputeIntersection(edge, v, vPO));
			}
			// case 4 v is behind and VPO is in front
			else if (!nIsInFront && nPoIsInFront)
			{
				// we save intersaction and save vPO
				newVertices.push_back(ComputeIntersection(edge, v, vPO));
				newVertices.push_back(vPO);
			}
		}
		vertices = newVertices;
	}
	return newVertices.empty();
}

bool Clipper::IsClipping() const
{
	return mIsClipping;
}
void Clipper::SetClipping(bool enable)
{
	mIsClipping = enable;
}