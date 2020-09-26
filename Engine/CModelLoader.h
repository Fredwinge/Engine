#pragma once
#include <vector>
#include "Drawable\CModel.h"
#include "CGraphics.h"

class CModelLoader
{

	friend class CModel;

private:

	//TODO: Rework to just use ptrs instead of vectors
	static void LoadModel(const char* path, std::vector<CModel::VertexData>* pVertexBuffer, std::vector<unsigned short>* pIndexBuffer);
	
};