#pragma once
#include <vector>
#include "VertexData.h"
#include "Renderer.h"
#include "RenderMesh.h"

class CModelLoader
{
public:

	//TODO: Having all this as static is not pretty, should rework this
	//perhaps we can generate qtangents + binormals here aswell?
	static void LoadModel(const char* path, std::vector<SVertexData>* pVertexBuffer, std::vector<uint16>* pIndexBuffer);

private:

	static void LoadModelOBJ(const char* path, std::vector<SVertexData>* pVertexBuffer, std::vector<uint16>* pIndexBuffer);
	
};