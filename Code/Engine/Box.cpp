#include "Box.h"
#include "Bindable/BindableBase.h"
#include "Primitives/CCube.h"

CBox::CBox(CRenderer* pRenderer,
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

	//auto model = CCube::Make<Vector3>();

	////Wow, nice fix
	//std::vector<VertexData> vertexBuffer;
	//for (int i = 0; i < model.vertices.size(); ++i)
	//{
	//	VertexData vData;
	//	vData.Position = model.vertices[i];
	//	vertexBuffer.push_back(vData);
	//}
	std::vector<uint16> pIndexBuffer;
	std::vector<VertexData> pVertexBuffer;

	CCube::Create(&pVertexBuffer, &pIndexBuffer);

	m_pRenderData = new RenderData(CVertexBuffer(pRenderer, pVertexBuffer), CIndexBuffer(pRenderer, pIndexBuffer));
	//m_pRenderData->pVertexBuffer = new CVertexBuffer(rGfx, vertexBuffer);
	//m_pRenderData->pIndexBuffer = new CIndexBuffer(rGfx, model.indices);

	/*AddBind(std::make_unique<CVertexBuffer>(rGfx, model.vertices));

	//Bind vertex shader
	auto pVertexShader = std::make_unique<CVertexShader>(rGfx, L"../Debug/PrimitiveVertexShader.cso");
	auto pVertexShaderByteCode = pVertexShader->GetBytecode();
	AddBind(std::move(pVertexShader));

	//Bind pixel shader
	AddBind(std::make_unique<CPixelShader>(rGfx, L"../Debug/PrimitivePixelShader.cso"));

	//Bind index buffer
	AddIndexBuffer(std::make_unique<CIndexBuffer>(rGfx, model.indices));

	//Bind pixel constant buffer
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
	AddBind(std::make_unique<CPixelConstantBuffer<PixelCBuffer>>(rGfx, PxCBuffer));

	//Bind input layout
	const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	AddBind(std::make_unique<CInputLayout>(rGfx, inputElementDesc, pVertexShaderByteCode));

	//Bind topology
	AddBind(std::make_unique<CTopology>(rGfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	//Bind matrix constant buffer
	AddBind(std::make_unique<CTransformCBuf>(rGfx, *this));*/

	m_ModelMatrix = Matrix::Identity;
}

void CBox::Update(float deltaTime) /*noexcept*/
{
	roll += droll * deltaTime;
	pitch += dpitch * deltaTime;
	yaw += dyaw * deltaTime;
	theta += dtheta * deltaTime;
	phi += dphi * deltaTime;
	chi += dchi * deltaTime;

	Matrix posMatrix = Matrix::Identity;
	posMatrix.Pos.x = r;
	m_ModelMatrix = Matrix::CreateRotation(Vec3(roll, pitch, yaw)) * posMatrix * Matrix::CreateRotation(Vec3(theta, phi, chi));
}