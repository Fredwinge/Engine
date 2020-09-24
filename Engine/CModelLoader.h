#pragma once
#include <vector>
#include "Drawable\CModel.h"
#include "CGraphics.h"

class CModelLoader
{

public:

	static CModel* LoadModel(CGraphics& rGfx, const char* path);
	
};