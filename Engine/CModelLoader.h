#pragma once
#include <vector>
#include "Drawable\CModel.h"
#include "CGraphics.h"

class CModelLoader
{

	friend class CModel;

private:

	//TODO: Figure out a good way to rework this, if necessary
	static void LoadModel(const char* path, std::vector<CModel::VertexData>* pVertexBuffer, std::vector<unsigned short>* pIndexBuffer);
	
};