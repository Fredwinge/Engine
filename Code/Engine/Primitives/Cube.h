#pragma once
#include "VertexData.h"
#include "Math/Vectors.h"
#include "RenderMesh.h"

//	C		U		B		E

//TODO: Clean up this mess of a class

class CCube
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

	static void CreateMesh(CRenderer* pRenderer);

	static CRenderMesh* s_pRenderMesh;
};