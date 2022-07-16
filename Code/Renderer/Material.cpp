#include "Material.h"

CMaterial::CMaterial(CRenderer* pRenderer, const char* sVertexShader, const char* sPixelShader)
{
	m_pVertexShader = new CVertexShader(pRenderer, sVertexShader);
	m_pPixelShader = new CPixelShader(pRenderer, sPixelShader);

	//TODO: This should be read from the shader data eventually // maybe based on vertex buffer data aswell?
	const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	m_pInputLayout = new CInputLayout(pRenderer, inputElementDesc, m_pVertexShader->GetBytecode());
}

CMaterial::~CMaterial()
{
	if (m_pVertexShader)
		delete m_pVertexShader;

	if (m_pPixelShader)
		delete m_pPixelShader;

	if (m_pInputLayout)
		delete m_pInputLayout;
}

void CMaterial::BindMaterial(CRenderer* pRenderer)
{
	m_pVertexShader->Bind(pRenderer);
	m_pPixelShader->Bind(pRenderer);
	m_pInputLayout->Bind(pRenderer);
}