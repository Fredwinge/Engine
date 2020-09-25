#include "CModel.h"
#include "../Bindable/BindableBase.h"

CModel::CModel(CGraphics& rGfx, std::vector<VertexData>* pVertexBuffer, std::vector<unsigned short>* pIndices)
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

	if (IsStaticInitialized() == false)
	{

		AddStaticBind(std::make_unique<CVertexBuffer>(rGfx, *pVertexBuffer));

		auto pVertexShader = std::make_unique<CVertexShader>(rGfx, L"../Debug/BaseVertexShader.cso");
		auto pVertexShaderByteCode = pVertexShader->GetBytecode();
		AddStaticBind(std::move(pVertexShader));

		AddStaticBind(std::make_unique<CPixelShader>(rGfx, L"../Debug/BasePixelShader.cso"));

		AddStaticIndexBuffer(std::make_unique<CIndexBuffer>(rGfx, *pIndices));

		//struct PixelCBuffer
		//{
		//	struct
		//	{
		//		float r;
		//		float g;
		//		float b;
		//		float a;
		//	} face_colors[8];
		//};
		//const PixelCBuffer PxCBuffer =
		//{
		//	{
		//		{1.0f, 0.0f, 1.0f, 1.0f},
		//	{1.0f, 0.0f, 0.0f, 1.0f},
		//	{0.0f, 1.0f, 0.0f, 1.0f},
		//	{0.0f, 1.0f, 1.0f, 1.0f},
		//	{1.0f, 1.0f, 0.0f, 1.0f},
		//	{0.0f, 0.0f, 1.0f, 1.0f},
		//	{1.0f, 1.0f, 1.0f, 1.0f},
		//	{1.0f, 0.0f, 1.0f, 1.0f},
		//	}
		//};
		//AddStaticBind(std::make_unique<CPixelConstantBuffer<PixelCBuffer>>(rGfx, PxCBuffer));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		AddStaticBind(std::make_unique<CInputLayout>(rGfx, inputElementDesc, pVertexShaderByteCode));

		AddStaticBind(std::make_unique<CTopology>(rGfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<CTransformCBuf>(rGfx, *this));

	dx::XMStoreFloat3x3(&mt, dx::XMMatrixScaling(1.0f, 1.0f, 1.0f));
}

void CModel::Update(float deltaTime)
{

}

DirectX::XMMATRIX CModel::GetTransformXM() const
{
	return DirectX::XMLoadFloat3x3(&mt) * 
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, -10.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}