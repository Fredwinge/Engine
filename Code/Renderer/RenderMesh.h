#pragma once
#include "Bindable/IndexBuffer.h"
#include "Bindable/VertexBuffer.h"

//The purpose of this class is to hold mesh data
//TODO: InputLayout aswell?
class CRenderMesh
{

public:
	CRenderMesh() = delete;
	CRenderMesh(CVertexBuffer vBuffer, CIndexBuffer iBuffer);

	~CRenderMesh();

	void BindBuffers(CRenderer* pRenderer);

	tIndex GetIdxCount() { return m_pIndexBuffer->GetCount(); }

private:

	CIndexBuffer* m_pIndexBuffer;
	CVertexBuffer* m_pVertexBuffer;

};