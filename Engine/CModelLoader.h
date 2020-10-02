#pragma once
#include <vector>
#include "Renderable/EssentialRenderData.h"
#include "CGraphics.h"

class CModelLoader
{

	friend class CModel;

private:

	//TODO: Figure out a good way to rework this, if necessary
	static void LoadModel(const char* path, std::vector<VertexData>* pVertexBuffer, std::vector<unsigned short>* pIndexBuffer);
	
};