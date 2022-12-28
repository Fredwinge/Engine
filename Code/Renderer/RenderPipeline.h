#pragma once
#include "Renderable\IRenderable.h"
#include "Utils\Timer.h"
#include "Bindable/RenderTarget.h"

//TODO: Make sure it matches shaders
#define MAX_DIRECTIONAL_LIGHTS 8
#define MAX_POINT_LIGHTS 8

//TODO: Move these structs somewhere else
struct SDirectionalLight
{
	Vec4 m_vLightColor;
	Vec4 m_vDirection;
};

struct SPointLight
{
	Vec4 m_vLightColor;
	Vec3 m_vPos;
	float m_fRadius;
};

class CRenderPipeline
{
public:

	enum ERenderPass
	{
		OPAQUE_PASS = 0,

		NUM_PASSES
	};

	CRenderPipeline(CRenderer* pRenderer);

	void AddToQueue(IRenderable* pRenderable, ERenderPass ePass);
	void ClearQueue();
	
	void RenderScene();

	//TODO: rework, this is bad
	void AddDirectionalLight(SDirectionalLight dirLight);

	void AddPointLight(SPointLight pointLight);

private:

	std::vector<IRenderable*> m_RenderQueue[NUM_PASSES];

	CRenderer* m_pSceneRenderer;

	CTimer m_Timer;

	//Post process tests
	CSampler* m_pSampler;
	CRenderTarget* m_pRenderTarget;
	CMaterial* m_pPostProcessMaterial;
	CRenderMesh* m_pPostProcessMesh;

	CRenderTarget* m_pZTarget;
	CRenderTarget* m_pPosTarget;

	CDepthStencil* m_PostProcessDepthStencil;
	

	//TODO: Move somewhere else
	SDirectionalLight m_vDirectionalLights[MAX_DIRECTIONAL_LIGHTS];
	uint32 m_NumDirectionalLights = 0;

	SPointLight m_vPointLights[MAX_POINT_LIGHTS];
	uint32 m_NumPointLights = 0;

	CMaterial* m_pDeferredLightingMaterial;
	struct DeferredLightsBuffer
	{
		SDirectionalLight m_pDirectonalLights[MAX_DIRECTIONAL_LIGHTS];
		SPointLight m_pPointLights[MAX_POINT_LIGHTS];

		Vec4 m_CamPos;
	};
	CConstantBuffer* m_pLightingCBuffer;

	//TODO: This is meant to copy m_pRenderTarget and be used as a shader resource, figure out some better way to handle this.
	CRenderTarget* m_pCopyTarget;

};