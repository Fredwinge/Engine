#include "Box.h"
#include "Bindable/BindableBase.h"
#include "Primitives/CCube.h"
#include "Utils\Camera.h"

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
	phi(adist(rng)),
	m_WorldMatrix(Matrix::Identity)
{
	CCube::Create(pRenderer, &m_pRenderMesh);

	m_pMaterial = new CMaterial(pRenderer, "BaseVertexShader.cso", "BasePixelShader.cso");
}

void CBox::Update(float deltaTime)
{
	roll += droll * deltaTime;
	pitch += dpitch * deltaTime;
	yaw += dyaw * deltaTime;
	theta += dtheta * deltaTime;
	phi += dphi * deltaTime;
	chi += dchi * deltaTime;

	Matrix posMatrix = Matrix::Identity;
	posMatrix.Pos.x = r;
	m_WorldMatrix = Matrix::CreateRotation(Vec3(roll, pitch, yaw)) * posMatrix * Matrix::CreateRotation(Vec3(theta, phi, chi));
}

//TODO: Do better than this
struct vrtCBuf
{
	Matrix worldViewProjection;
};

void CBox::RenderInternal(CRenderer* pRenderer)
{
	vrtCBuf cbuf;

	cbuf.worldViewProjection = m_WorldMatrix * pRenderer->GetCamera()->GetViewProjection();
	cbuf.worldViewProjection.Transpose();


	CConstantBuffer* vertexCBuffer = new CConstantBuffer(pRenderer, CConstantBuffer::ETYPE_VERTEX, &cbuf, sizeof(vrtCBuf));
	vertexCBuffer->Bind(pRenderer);

	delete vertexCBuffer;
}