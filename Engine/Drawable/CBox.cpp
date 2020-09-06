#include "CBox.h"
#include "../Bindable/BindableBase.h"
#include "../GraphicsAssertMacros.h"
#include "../Geometry/CCube.h"

CBox::CBox(CGraphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>&adist,
	std::uniform_real_distribution<float>&ddist,
	std::uniform_real_distribution<float>&odist,
	std::uniform_real_distribution<float>&rdist,
	std::uniform_real_distribution<float>&bdist)
	:
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
{

	namespace dx = DirectX;

	if (IsStaticInitialized() == false)
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
		};

		/*//Box UV Coords
		model.vertices[0].tex = { 0.0f, 0.0f }; //1, 2, 4
		model.vertices[1].tex = { 1.0f, 0.0f }; //0, 3, 5
		model.vertices[2].tex = { 0.0f, 1.0f }; //0, 3, 6
		model.vertices[3].tex = { 1.0f, 1.0f }; //1, 2, 7
		model.vertices[4].tex = { 1.0f, 1.0f }; //0, 5, 6
		model.vertices[5].tex = { 0.0f, 1.0f }; //1, 4, 7
		model.vertices[6].tex = { 1.0f, 0.0f }; //2, 4, 7
		model.vertices[7].tex = { 0.0f, 0.0f }; //3, 5, 6
		*/

		auto model = CCube::Make<Vertex>();
		AddStaticBind(std::make_unique<CVertexBuffer>(gfx, model.vertices));

		auto pVertexShader = std::make_unique<CVertexShader>(gfx, L"../Debug/BaseVertexShader.cso");
		auto pVertexShaderByteCode = pVertexShader->GetBytecode();
		AddStaticBind(std::move(pVertexShader));

		AddStaticBind(std::make_unique<CPixelShader>(gfx, L"../Debug/BasePixelShader.cso"));

		AddStaticIndexBuffer(std::make_unique<CIndexBuffer>(gfx, model.indices));

		struct PixelCBuffer
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[8];
		};
		const PixelCBuffer PxCBuffer =
		{
			{
				{1.0f, 0.0f, 1.0f, 1.0f},
				{1.0f, 0.0f, 0.0f, 1.0f},
				{0.0f, 1.0f, 0.0f, 1.0f},
				{0.0f, 1.0f, 1.0f, 1.0f},
				{1.0f, 1.0f, 0.0f, 1.0f},
				{0.0f, 0.0f, 1.0f, 1.0f},
				{1.0f, 1.0f, 1.0f, 1.0f},
				{1.0f, 0.0f, 1.0f, 1.0f},
			}
		};
		AddStaticBind(std::make_unique<CPixelConstantBuffer<PixelCBuffer>>(gfx, PxCBuffer));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		AddStaticBind(std::make_unique<CInputLayout>(gfx, inputElementDesc, pVertexShaderByteCode));

		AddStaticBind(std::make_unique<CTopology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<CTransformCBuf>(gfx, *this));

	dx::XMStoreFloat3x3(&mt, dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng)));
}

void CBox::Update(float deltaTime) noexcept
{
	roll += droll * deltaTime;
	pitch += dpitch * deltaTime;
	yaw += dyaw * deltaTime;
	theta += dtheta * deltaTime;
	phi += dphi * deltaTime;
	chi += dchi * deltaTime;
}

DirectX::XMMATRIX CBox::GetTransformXM() const noexcept
{
	return DirectX::XMLoadFloat3x3(&mt) * 
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}