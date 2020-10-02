#pragma once
#include "../CGraphics.h"
#include "../Renderable/EssentialRenderData.h"
#include "../Bindable/BindableBase.h"
#include "../Renderable/IRenderable.h"

class IRenderCallback
{

public:

	IRenderCallback(CGraphics& rGfx);
	~IRenderCallback();

	//TODO: Change from IRenderable to something else
	virtual void RenderCallback(CGraphics& rGfx, IRenderable* pRenderable);


private:

	CVertexShader* m_pVertexShader;

	CInputLayout* m_pInputLayout;

	CPixelShader* m_pPixelShader;

	struct vrtCBuf
	{
		DirectX::XMMATRIX transformMatrix;
	};

};