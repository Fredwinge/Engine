#pragma once
#include "Bindable/IndexBuffer.h"
#include "Bindable/VertexBuffer.h"

//The purpose of this class is to hold mesh data
//TODO: InputLayout aswell?
class CRenderMesh
{
	//TODO: Binormal / Tangent vectors
	//https://www.rastertek.com/dx10tut20.html have a look
	struct VertexData
	{
		Vector3 Position = Vector3(0.0f, 0.0f, 0.0f);
		Vector2 TexCoord = Vector2(0.0f, 0.0f);
		Vector3 Normal = Vector3(0.0f, 0.0f, 0.0f);
	};

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