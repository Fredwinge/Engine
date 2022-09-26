#pragma once
#include "Renderer.h"
#include "RenderMesh.h"

template<uint32 DivX = 1, uint32 DivZ = 1>
class CPlane
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
		constexpr uint32 numDivX = DivX;
		constexpr uint32 numDivZ = DivZ;

		constexpr float sizeX = 2.0f;
		constexpr float sizeZ = 2.0f;

		constexpr uint32 numVertsX = numDivX + 1;
		constexpr uint32 numVertsZ = numDivZ + 1;

		std::vector<SVertexData> vertices;

		{
			constexpr float halfSideX = sizeX / 2.0f;
			constexpr float halfSideZ = sizeZ / 2.0f;
			constexpr float divSizeX = sizeX / float(numDivX);
			constexpr float divSizeZ = sizeZ / float(numDivZ);

			Vec3 bottomLeft = Vec3(-halfSideX, 0.0f, -halfSideZ);

			for (uint32 z = 0, i = 0; z < numVertsZ; ++z)
			{
				const float posZ = float(z) * divSizeZ;
				for (uint32 x = 0; x < numVertsX; ++x, ++i)
				{
					const float posX = float(x) * divSizeX;
					SVertexData vertex;
					vertex.Position = bottomLeft + Vec3(posX, 0.0f, posZ);
					vertex.TexCoord = Vec2(posX / sizeX, -posZ / sizeZ);
					vertex.Normal = Vec3(0.0f, 1.0f, 0.0f);

					vertices.push_back(vertex);
				}

			}
		}

		std::vector<tIndex> indices;
		{
			const auto v2i = [numVertsX](tIndex x, tIndex z)
			{
				return (tIndex)(z * numVertsX + x);
			};

			for (uint32 z = 0; z < numDivZ; ++z)
			{
				for (uint32 x = 0; x < numDivX; ++x)
				{
					const tIndex indexArray[4] =
					{
						v2i(x,z), v2i(x + 1, z), v2i(x, z + 1), v2i(x + 1, z + 1)
					};
					indices.push_back(indexArray[0]);
					indices.push_back(indexArray[2]);
					indices.push_back(indexArray[1]);
					indices.push_back(indexArray[1]);
					indices.push_back(indexArray[2]);
					indices.push_back(indexArray[3]);
				}
			}
		}

		s_pRenderMesh = new CRenderMesh(CVertexBuffer(pRenderer, vertices), CIndexBuffer(pRenderer, indices));
	}

	static CRenderMesh* s_pRenderMesh;
};

template<uint32 DivX, uint32 DivZ>
CRenderMesh* CPlane<DivX, DivZ>::s_pRenderMesh = nullptr;