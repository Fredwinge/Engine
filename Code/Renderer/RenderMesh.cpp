#include "RenderMesh.h"

CRenderMesh::CRenderMesh(CVertexBuffer vBuffer, CIndexBuffer iBuffer)
{
	m_pIndexBuffer = new CIndexBuffer(iBuffer);
	m_pVertexBuffer = new CVertexBuffer(vBuffer);
}

CRenderMesh::~CRenderMesh()
{
	if(m_pIndexBuffer)
		delete m_pIndexBuffer;

	if(m_pVertexBuffer)
		delete m_pVertexBuffer;
}

void CRenderMesh::BindBuffers(CRenderer* pRenderer)
{
	m_pIndexBuffer->Bind(pRenderer);
	m_pVertexBuffer->Bind(pRenderer);
}