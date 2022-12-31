#include "PointLight.h"
#include "Bindable/BindableBase.h"
#include "Utils\Camera.h"
#include "Primitives\Sphere.h"

struct PointLightBufferVertex
{
	Matrix worldViewProjection;
	Matrix worldMatrix;
	Vec4 LightColor;
};

struct PointLightBufferPixel
{
	Vec4 camDirToCenter;
};

CPointLight::CPointLight(CRenderer* pRenderer, Vec4 vColor, Vec3 vPos, float fRadius)
	:
	m_vColor(vColor),
	m_fRadius(fRadius)
{

	//TODO: Something is wrong with the CSphere, there are weird artifacts on one side when the sphere is transparent. Check it out
	CSphere<20, 20>::Create(pRenderer, &m_pRenderMesh);

	m_pMaterial = new CMaterial(pRenderer, "DebugLightVertex.cso", "DebugLightPixel.cso");

	m_WorldMatrix = Matrix::Identity;
	m_WorldMatrix.Pos.SetXYZ(vPos);


	PointLightBufferVertex VCBuf;

	VCBuf.worldViewProjection = m_WorldMatrix * pRenderer->GetCamera()->GetViewProjection();
	VCBuf.worldViewProjection.Transpose();
	VCBuf.worldMatrix = m_WorldMatrix.GetTransposed();
	VCBuf.LightColor = vColor;

	m_pVertexCBuffer = new CConstantBuffer(pRenderer, CConstantBuffer::ETYPE_VERTEX, &VCBuf, sizeof(PointLightBufferVertex));

	PointLightBufferPixel PCBuf;
	PCBuf.camDirToCenter = Vec4::Zero;
	m_pPixelCBuffer = new CConstantBuffer(pRenderer, CConstantBuffer::ETYPE_PIXEL, &PCBuf, sizeof(PointLightBufferPixel));
}

void CPointLight::Update(float deltaTime)
{
}

void CPointLight::RenderInternal(CRenderer* pRenderer)
{
	PointLightBufferVertex VCBuf;

	VCBuf.worldViewProjection = m_WorldMatrix * pRenderer->GetCamera()->GetViewProjection();
	VCBuf.worldViewProjection.Transpose();
	VCBuf.worldMatrix = m_WorldMatrix.GetTransposed();
	VCBuf.LightColor = m_vColor;
	m_pVertexCBuffer->Update(pRenderer, &VCBuf);
	m_pVertexCBuffer->Bind(pRenderer);

	PointLightBufferPixel PCBuf;
	PCBuf.camDirToCenter = Vec4((pRenderer->GetCamera()->GetPosition() - m_WorldMatrix.Pos.GetXYZ()).GetNormalized(), 1.0f);
	m_pPixelCBuffer->Update(pRenderer, &PCBuf);
	m_pPixelCBuffer->Bind(pRenderer);
}