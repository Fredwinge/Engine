#pragma once
#include "../Bindable/CIndexBuffer.h"
#include "../Bindable/CVertexBuffer.h"
#include <DirectXMath.h>

//TEMPORARY FILE
//TODO: FIND A BETTER SETUP FOR THIS

struct RenderData
{
	RenderData(CVertexBuffer vBuffer, CIndexBuffer iBuffer)
	{
		pIndexBuffer = new CIndexBuffer(iBuffer);
		pVertexBuffer = new CVertexBuffer(vBuffer);

		mWorldMatrix = DirectX::XMMATRIX::XMMATRIX();
	}

	CIndexBuffer* pIndexBuffer;
	CVertexBuffer* pVertexBuffer;

	//TODO: Actually model * world, rename?
	DirectX::XMMATRIX mWorldMatrix;
};

//TODO: put somewhere better?
struct VertexData
{
	Vector3 Position = Vector3(0.0f, 0.0f, 0.0f);
	Vector2 TexCoord = Vector2(0.0f, 0.0f);
	Vector3 Normal = Vector3(0.0f, 0.0f, 0.0f);
};