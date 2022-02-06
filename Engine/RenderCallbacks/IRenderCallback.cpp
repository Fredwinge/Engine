#include "IRenderCallback.h"
#include "../CCamera.h"

IRenderCallback::IRenderCallback(CRenderer* pRenderer)
{
	m_pVertexShader = new CVertexShader(pRenderer, "BaseVertexShader.cso");
	m_pPixelShader = new CPixelShader(pRenderer, "BasePixelShader.cso");

	const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	m_pInputLayout = new CInputLayout(pRenderer, inputElementDesc, m_pVertexShader->GetBytecode());
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

static float rotBonus = 0.0f;

void IRenderCallback::RenderCallback(CRenderer* pRenderer, RenderData* pRenderData)
{

	ID3D11DeviceContext* pContext = pRenderer->GetDeviceContext();

	pRenderData->pVertexBuffer->Bind(pRenderer);

	m_pVertexShader->Bind(pRenderer);

	m_pInputLayout->Bind(pRenderer);

	pRenderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	vrtCBuf cbuf;

	cbuf.worldViewProjection = pRenderData->m_WorldMatrix * pRenderer->GetCamera()->GetViewProjection();
	cbuf.worldViewProjection.Transpose();


	CVertexConstantBuffer<vrtCBuf>* vertexCBuffer = new CVertexConstantBuffer<vrtCBuf>(pRenderer, cbuf);
	vertexCBuffer->Bind(pRenderer);

	pRenderData->pIndexBuffer->Bind(pRenderer);

	m_pPixelShader->Bind(pRenderer);

	pRenderer->DrawIndexed(pRenderData->pIndexBuffer->GetCount());


	delete vertexCBuffer;
	
}