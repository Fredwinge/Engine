#include "PointLight.h"
#include "Bindable/BindableBase.h"
#include "Utils\Camera.h"
#include "Primitives\Sphere.h"

struct DebugPointLightBuffer
{
	Matrix worldViewProjection;
	Vec4 LightColor;
};

CPointLight::CPointLight(CRenderer* pRenderer, Vec4 vColor, Vec3 vPos, float fRadius)
	:
	m_vColor(vColor),
	m_fRadius(fRadius)
{

	CSphere<10, 10>::Create(pRenderer, &m_pRenderMesh);

	m_pMaterial = new CMaterial(pRenderer, "DebugLightVertex.cso", "DebugLightPixel.cso");

	m_WorldMatrix = Matrix::Identity;
	m_WorldMatrix.Pos.SetXYZ(vPos);


	DebugPointLightBuffer cbuf;

	cbuf.worldViewProjection = m_WorldMatrix * pRenderer->GetCamera()->GetViewProjection();
	cbuf.worldViewProjection.Transpose();
	cbuf.LightColor = vColor;

	vertexCBuffer = new CConstantBuffer(pRenderer, CConstantBuffer::ETYPE_VERTEX, &cbuf, sizeof(DebugPointLightBuffer));
}

void CPointLight::Update(float deltaTime)
{
}

void CPointLight::RenderInternal(CRenderer* pRenderer)
{
	DebugPointLightBuffer cbuf;

	cbuf.worldViewProjection = m_WorldMatrix * pRenderer->GetCamera()->GetViewProjection();
	cbuf.worldViewProjection.Transpose();
	cbuf.LightColor = m_vColor;
	vertexCBuffer->Update(pRenderer, &cbuf);
	vertexCBuffer->Bind(pRenderer);
}