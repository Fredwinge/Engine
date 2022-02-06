#pragma once
#include "Bindable/IndexBuffer.h"
#include "Bindable/VertexBuffer.h"
#include "Math/Matrix.h"

//TEMPORARY FILE
//TODO: FIND A BETTER SETUP FOR THIS

struct RenderData
{
	RenderData(CVertexBuffer vBuffer, CIndexBuffer iBuffer)
	{
		pIndexBuffer = new CIndexBuffer(iBuffer);
		pVertexBuffer = new CVertexBuffer(vBuffer);

		m_WorldMatrix = Matrix::Identity;
	}

	CIndexBuffer* pIndexBuffer;
	CVertexBuffer* pVertexBuffer;

	//TODO: Actually model * world, rename?
	Matrix m_WorldMatrix;
};

//TODO: put somewhere better?
//TODO: Binormal / Tangent vectors
//https://www.rastertek.com/dx10tut20.html have a look
struct VertexData
{
	Vector3 Position = Vector3(0.0f, 0.0f, 0.0f);
	Vector2 TexCoord = Vector2(0.0f, 0.0f);
	Vector3 Normal = Vector3(0.0f, 0.0f, 0.0f);
};