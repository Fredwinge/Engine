#pragma once
#include "Renderer.h"
#include "RenderMesh.h"
#include "VertexData.h"
#include <assert.h>

template<uint32 LatDiv = 10, uint32 LongDiv = 10>
class CSphere
{
public:

	static void Create(CRenderer* pRenderer, CRenderMesh** pRenderMesh)
	{
		if (!s_pRenderMesh)
		{
			CreateMesh(pRenderer);
		}

		*pRenderMesh = s_pRenderMesh;
	}

private:

	static void CreateMesh(CRenderer* pRenderer)
	{
		constexpr uint32 numLatDiv = LatDiv;
		constexpr uint32 numLongDiv = LongDiv;
		assert(LatDiv >= 3 && LongDiv >= 3);

		constexpr float PI = 3.14159265f;

		constexpr float radius = 1.0f;
		const Vector3 base = Vec3(0.0f, radius, 0.0f);
		const float latitudeAngle = PI / numLatDiv;
		const float longitudeAngle = 2.0f * PI / numLongDiv;

		std::vector<SVertexData> vertices;
		for (uint32 iLat = 1; iLat < numLatDiv; ++iLat)
		{
			const Vec3 latBase = base * Matrix::CreateRotation(Vec3(latitudeAngle * iLat, 0.0f, 0.0f));

			for (uint32 iLong = 0; iLong < numLongDiv; ++iLong)
			{

				SVertexData vertex;
				vertex.Position = latBase * Matrix::CreateRotation(Vec3(0.0f, longitudeAngle * iLong, 0.0f));
				vertex.Normal = vertex.Position.GetNormalized();
				vertices.push_back(vertex);
			}
		}


		//Add the cap vertices
		const tIndex iNorthPole = (tIndex)vertices.size();

		SVertexData vNorthPole;
		vNorthPole.Position = base;
		vNorthPole.Normal = vNorthPole.Position.GetNormalized();
		vertices.push_back(vNorthPole);

		const tIndex iSouthPole = (tIndex)vertices.size();

		SVertexData vSouthPole;
		vSouthPole.Position = -base;
		vSouthPole.Normal = vSouthPole.Position.GetNormalized();
		vertices.push_back(vSouthPole);

		//Calculate indices
		const auto calcIdx = [numLatDiv, numLongDiv](tIndex iLat, tIndex iLong)
		{
			return iLat * numLongDiv + iLong;
		};

		std::vector<tIndex> indices;
		for (tIndex iLat = 0; iLat < numLatDiv - 2; ++iLat)
		{
			for (tIndex iLong = 0; iLong < numLongDiv; ++iLong)
			{
				indices.push_back(calcIdx(iLat, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong + 1));
			}

			//wrap band
			indices.push_back(calcIdx(iLat, numLongDiv - 1));
			indices.push_back(calcIdx(iLat + 1, numLongDiv - 1));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat + 1, numLongDiv - 1));
			indices.push_back(calcIdx(iLat + 1, 0));
		}

		//cap fans
		for (tIndex iLong = 0; iLong < numLongDiv - 1; ++iLong)
		{
			//north
			indices.push_back(iNorthPole);
			indices.push_back(calcIdx(0, iLong));
			indices.push_back(calcIdx(0, iLong + 1));
			//south
			indices.push_back(calcIdx(numLatDiv - 2, iLong + 1));
			indices.push_back(calcIdx(numLatDiv - 2, iLong));
			indices.push_back(iSouthPole);
		}

		//wrap triangles
		//north
		indices.push_back(iNorthPole);
		indices.push_back(calcIdx(0, numLongDiv - 1));
		indices.push_back(calcIdx(0, 0));
		//south
		indices.push_back(calcIdx(numLatDiv - 2, 0));
		indices.push_back(calcIdx(numLatDiv - 2, numLongDiv - 1));
		indices.push_back(iSouthPole);

		s_pRenderMesh = new CRenderMesh(CVertexBuffer(pRenderer, vertices), CIndexBuffer(pRenderer, indices));
	}

	static CRenderMesh* s_pRenderMesh;
};

template<uint32 LatDiv, uint32 LongDiv>
CRenderMesh* CSphere<LatDiv, LongDiv>::s_pRenderMesh = nullptr;