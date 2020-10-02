#pragma once
#include "CConstantBuffer.h"
#include "../Renderable/IRenderable.h"
#include <DirectXMath.h>

//TODO: Remove this?
//The purpose of CTransformCBuf is to automatically set the perspective matrix in the vertex constant buffer

class CTransformCBuf : public IBindable
{
public:

	CTransformCBuf(CGraphics& gfx, const IRenderable& parent);
	void Bind(CGraphics& gfx) noexcept override;

private:
	
	//m_pVCBuf is a unique ptr so that it can be dynamically allocated and reused
	//incase we initialize multiple versions of one object, for performance reasons
	static std::unique_ptr<CVertexConstantBuffer<DirectX::XMMATRIX>> m_pVCBuf;
	const IRenderable& m_rParent;

};