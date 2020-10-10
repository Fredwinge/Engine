#include "IRenderCallback.h"

IRenderCallback::IRenderCallback(CGraphics& rGfx)
{
	m_pVertexShader = new CVertexShader(rGfx, L"../Debug/BaseVertexShader.cso");
	m_pPixelShader = new CPixelShader(rGfx, L"../Debug/BasePixelShader.cso");

	const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	m_pInputLayout = new CInputLayout(rGfx, inputElementDesc, m_pVertexShader->GetBytecode());
}

IRenderCallback::~IRenderCallback()
{

	if (m_pVertexShader != nullptr)
		delete m_pVertexShader;

	if (m_pPixelShader != nullptr)
		delete m_pPixelShader;

	if (m_pInputLayout != nullptr)
		delete m_pInputLayout;
}

void IRenderCallback::RenderCallback(CGraphics& rGfx, RenderData* pRenderData)
{

	ID3D11DeviceContext* pContext = rGfx.GetDeviceContext();

	pRenderData->pVertexBuffer->Bind(rGfx);

	m_pVertexShader->Bind(rGfx);

	m_pInputLayout->Bind(rGfx);

	rGfx.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	vrtCBuf cbuf;

	cbuf.transformMatrix = DirectX::XMMatrixTranspose(pRenderData->mWorldMatrix * rGfx.GetViewProjection());

	CVertexConstantBuffer<vrtCBuf>* vertexCBuffer = new CVertexConstantBuffer<vrtCBuf>(rGfx, cbuf);
	vertexCBuffer->Bind(rGfx);

	pRenderData->pIndexBuffer->Bind(rGfx);

	m_pPixelShader->Bind(rGfx);

	rGfx.DrawIndexed(pRenderData->pIndexBuffer->GetCount());


	delete vertexCBuffer;
	
}