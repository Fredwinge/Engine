#pragma once
#include "Math/Matrix.h"
#include "Renderer.h"
#include "../Renderable/EssentialRenderData.h"
#include "../Bindable/BindableBase.h"
#include "../Renderable/IRenderable.h"

class IRenderCallback
{

public:

	IRenderCallback(CRenderer* pRenderer);
	~IRenderCallback();

	//TODO: Change from IRenderable to something else
	//TODO: Bad callback, change render system again
	virtual void RenderCallback(CRenderer* pRenderer, RenderData* pRenderData);


private:

	CVertexShader* m_pVertexShader;

	CInputLayout* m_pInputLayout;

	CPixelShader* m_pPixelShader;

	struct vrtCBuf
	{
		Matrix worldViewProjection;
	};

};