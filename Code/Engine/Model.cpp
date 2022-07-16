#include "Model.h"
#include "Bindable/BindableBase.h"
#include "ModelLoader.h"
#include "Utils\Camera.h"

CModel::CModel(CRenderer* pRenderer, const char* path)
	:
	r(0.0f),
	roll(0.0f),
	pitch(0.0f),
	yaw(0.0f),
	theta(0.0f),
	phi(0.0f),
	chi(0.0f)
{

	std::vector<SVertexData> VertexBuffer;
	std::vector<uint16> IndexBuffer;

	CModelLoader::LoadModel(path, &VertexBuffer, &IndexBuffer);
	m_pRenderMesh = new CRenderMesh(CVertexBuffer(pRenderer, VertexBuffer), CIndexBuffer(pRenderer, IndexBuffer));

	//m_pRenderData->pIndexBuffer = new CIndexBuffer(rGfx, IndexBuffer);
	//m_pRenderData->pVertexBuffer = new CVertexBuffer(rGfx, VertexBuffer);
	//m_pRenderData = new RenderData(CVertexBuffer(pRenderer, VertexBuffer), CIndexBuffer(pRenderer, IndexBuffer));

	m_pMaterial = new CMaterial(pRenderer, "BaseVertexShader.cso", "BasePixelShader.cso");
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

	m_WorldMatrix = Matrix::Identity;
	m_WorldMatrix.Pos.SetXYZ(Vec3(0.0f, -3.0f, -10.0f));
}

void CModel::Update(float deltaTime)
{
	yaw += deltaTime * 0.1f;
	m_WorldMatrix.RotatePreMultiply(Vec3(0.0f, deltaTime, 0.0f));
}

//TODO: Do better than this
struct vrtCBuf
{
	Matrix worldViewProjection;
};

void CModel::RenderInternal(CRenderer* pRenderer)
{
	vrtCBuf cbuf;

	cbuf.worldViewProjection = m_WorldMatrix * pRenderer->GetCamera()->GetViewProjection();
	cbuf.worldViewProjection.Transpose();


	CConstantBuffer* vertexCBuffer = new CConstantBuffer(pRenderer, CConstantBuffer::ETYPE_VERTEX, &cbuf, sizeof(vrtCBuf));
	vertexCBuffer->Bind(pRenderer);

	delete vertexCBuffer;
}