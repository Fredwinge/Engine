#pragma once
#include <vector>
#include "Renderable/EssentialRenderData.h"
#include "Renderer.h"

class CModelLoader
{

	friend class CModel;

private:

	//TODO: Having all this as static is not pretty, should rework this
	//perhaps we can generate qtangents + binormals here aswell?
	static void LoadModel(const char* path, std::vector<VertexData>* pVertexBuffer, std::vector<uint16>* pIndexBuffer);

	static void LoadModelOBJ(const char* path, std::vector<VertexData>* pVertexBuffer, std::vector<uint16>* pIndexBuffer);
	
};