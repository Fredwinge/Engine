#include "CModel.h"
#include "../Bindable/BindableBase.h"
#include "../CModelLoader.h"

CModel::CModel(CGraphics& rGfx, const char* path)
	:
	r(0.0f),
	roll(0.0f),
	pitch(0.0f),
	yaw(0.0f),
	theta(0.0f),
	phi(0.0f),
	chi(0.0f)
{
	namespace dx = DirectX;

	std::vector<VertexData> VertexBuffer;
	std::vector<unsigned short> IndexBuffer;

	CModelLoader::LoadModel(path, &VertexBuffer, &IndexBuffer);

	//m_pRenderData->pIndexBuffer = new CIndexBuffer(rGfx, IndexBuffer);
	//m_pRenderData->pVertexBuffer = new CVertexBuffer(rGfx, VertexBuffer);
	m_pRenderData = new RenderData(CIndexBuffer(rGfx, IndexBuffer), CVertexBuffer(rGfx, VertexBuffer));
	//BINDS
	//Bind vertex buffer
	/*AddBind(std::make_unique<CVertexBuffer>(rGfx, VertexBuffer));

	//Bind vertex shader
	auto pVertexShader = std::make_unique<CVertexShader>(rGfx, L"../Debug/BaseVertexShader.cso");
	auto pVertexShaderByteCode = pVertexShader->GetBytecode();
	AddBind(std::move(pVertexShader));

	//Bind pixel shader
	AddBind(std::make_unique<CPixelShader>(rGfx, L"../Debug/BasePixelShader.cso"));

	//Bind index buffer
	AddIndexBuffer(std::make_unique<CIndexBuffer>(rGfx, IndexBuffer));

	//Bind input layout
	const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	AddBind(std::make_unique<CInputLayout>(rGfx, inputElementDesc, pVertexShaderByteCode));

	//Bind topology
	AddBind(std::make_unique<CTopology>(rGfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	
	//Bind vertex constant buffer
	AddBind(std::make_unique<CTransformCBuf>(rGfx, *this));*/

	dx::XMStoreFloat3x3(&mt, dx::XMMatrixScaling(1.0f, 1.0f, 1.0f));
}

void CModel::Update(float deltaTime)
{
	yaw += 4.0f * deltaTime;
}

DirectX::XMMATRIX CModel::GetWorldMatrix() const
{
	return DirectX::XMLoadFloat3x3(&mt) * 
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, -3.0f, -10.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}