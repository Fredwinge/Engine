#pragma once
#include <vector>
#include "Renderable/EssentialRenderData.h"
#include "Renderer.h"

class CModelLoader
{

	friend class CModel;

private:

	//TODO: Figure out a good way to rework this, if necessary
	//Add rework so LoadModel becomes a base function which splits into different ones depending on file type
	static void LoadModel(const char* path, std::vector<VertexData>* pVertexBuffer, std::vector<uint16>* pIndexBuffer);
	
};