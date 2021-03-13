#include "CCube.h"

std::vector<VertexData> CCube::s_VertexBuffer;
std::vector<unsigned short> CCube::s_IndexBuffer;

bool CCube::s_bCubeCreated = false;

void CCube::CreateBuffers()
{
	const float side = 0.5f;
	//RIGHT FACE
	VertexData face[4];
	Vector3 faceNormal = Vector3(side, 0.0f, 0.0f);

	face[0].Position = Vector3(side, -side, -side);
	face[0].TexCoord = Vector2(0.0f, 0.0f);
	face[0].Normal = faceNormal;

	face[1].Position = Vector3(side, side, -side);
	face[1].TexCoord = Vector2(1.0f, 0.0f);
	face[1].Normal = faceNormal;

	face[2].Position = Vector3(side, -side, side);
	face[2].TexCoord = Vector2(0.0f, 1.0f);
	face[2].Normal = faceNormal;

	face[3].Position = Vector3(side, side, side);
	face[3].TexCoord = Vector2(1.0f, 1.0f);
	face[3].Normal = faceNormal;

	uint16 faceIndices[6] =
	{ 0, 1, 2,
		3, 2, 1 };

	//Push
	for (int i = 0; i < 4; ++i)
	{
		s_VertexBuffer.push_back(face[i]);
	}
	for (int i = 0; i < 6; ++i)
	{
		s_IndexBuffer.push_back(faceIndices[i]);
	}

	//LEFT FACE
	faceNormal = Vector3(-side, 1.0f, 1.0f);

	face[0].Position = Vector3(-side, -side, -side);
	face[0].TexCoord = Vector2(0.0f, 0.0f);
	face[0].Normal = faceNormal;

	face[1].Position = Vector3(-side, side, -side);
	face[1].TexCoord = Vector2(1.0f, 0.0f);
	face[1].Normal = faceNormal;

	face[2].Position = Vector3(-side, -side, side);
	face[2].TexCoord = Vector2(0.0f, 1.0f);
	face[2].Normal = faceNormal;

	face[3].Position = Vector3(-side, side, side);
	face[3].TexCoord = Vector2(1.0f, 1.0f);
	face[3].Normal = faceNormal;

	uint16 faceIndices2[6] =
	{ 6, 5, 4,
		5, 6, 7 };

	//Push
	for (int i = 0; i < 4; ++i)
	{
		s_VertexBuffer.push_back(face[i]);
	}
	for (int i = 0; i < 6; ++i)
	{
		s_IndexBuffer.push_back(faceIndices2[i]);
	}

	//TOP FACE
	faceNormal = Vector3(0.0f, side, 0.0f);

	face[0].Position = Vector3(-side, side, -side);
	face[0].TexCoord = Vector2(0.0f, 0.0f);
	face[0].Normal = faceNormal;

	face[1].Position = Vector3(-side, side, side);
	face[1].TexCoord = Vector2(1.0f, 0.0f);
	face[1].Normal = faceNormal;

	face[2].Position = Vector3(side, side, -side);
	face[2].TexCoord = Vector2(0.0f, 1.0f);
	face[2].Normal = faceNormal;

	face[3].Position = Vector3(side, side, side);
	face[3].TexCoord = Vector2(1.0f, 1.0f);
	face[3].Normal = faceNormal;

	uint16 faceIndices3[6] =
	{ 8, 9, 10,
		11, 10, 9 };

	//Push
	for (int i = 0; i < 4; ++i)
	{
		s_VertexBuffer.push_back(face[i]);
	}
	for (int i = 0; i < 6; ++i)
	{
		s_IndexBuffer.push_back(faceIndices3[i]);
	}

	//DOWN FACE
	faceNormal = Vector3(1.0f, -side, 1.0f);

	face[0].Position = Vector3(-side, -side, -side);
	face[0].TexCoord = Vector2(0.0f, 0.0f);
	face[0].Normal = faceNormal;

	face[1].Position = Vector3(-side, -side, side);
	face[1].TexCoord = Vector2(1.0f, 0.0f);
	face[1].Normal = faceNormal;

	face[2].Position = Vector3(side, -side, -side);
	face[2].TexCoord = Vector2(0.0f, 1.0f);
	face[2].Normal = faceNormal;

	face[3].Position = Vector3(side, -side, side);
	face[3].TexCoord = Vector2(1.0f, 1.0f);
	face[3].Normal = faceNormal;

	uint16 faceIndices4[6] =
	{ 14, 13, 12,
		13, 14, 15 };

	//Push
	for (int i = 0; i < 4; ++i)
	{
		s_VertexBuffer.push_back(face[i]);
	}
	for (int i = 0; i < 6; ++i)
	{
		s_IndexBuffer.push_back(faceIndices4[i]);
	}

	//FORWARD FACE
	faceNormal = Vector3(0.0f, 0.0f, side);

	face[0].Position = Vector3(-side, -side, side);
	face[0].TexCoord = Vector2(0.0f, 0.0f);
	face[0].Normal = faceNormal;

	face[1].Position = Vector3(-side, side, side);
	face[1].TexCoord = Vector2(1.0f, 0.0f);
	face[1].Normal = faceNormal;

	face[2].Position = Vector3(side, -side, side);
	face[2].TexCoord = Vector2(0.0f, 1.0f);
	face[2].Normal = faceNormal;

	face[3].Position = Vector3(side, side, side);
	face[3].TexCoord = Vector2(1.0f, 1.0f);
	face[3].Normal = faceNormal;

	uint16 faceIndices5[6] =
	{ 18, 17, 16,
		17, 18, 19 };

	//Push
	for (int i = 0; i < 4; ++i)
	{
		s_VertexBuffer.push_back(face[i]);
	}
	for (int i = 0; i < 6; ++i)
	{
		s_IndexBuffer.push_back(faceIndices5[i]);
	}

	//BACKWARD FACE
	faceNormal = Vector3(1.0f, 1.0f, -side);

	face[0].Position = Vector3(-side, -side, -side);
	face[0].TexCoord = Vector2(0.0f, 0.0f);
	face[0].Normal = faceNormal;

	face[1].Position = Vector3(-side, side, -side);
	face[1].TexCoord = Vector2(1.0f, 0.0f);
	face[1].Normal = faceNormal;

	face[2].Position = Vector3(side, -side, -side);
	face[2].TexCoord = Vector2(0.0f, 1.0f);
	face[2].Normal = faceNormal;

	face[3].Position = Vector3(side, side, -side);
	face[3].TexCoord = Vector2(1.0f, 1.0f);
	face[3].Normal = faceNormal;

	uint16 faceIndices6[6] =
	{ 20, 21, 22,
		23, 22, 21 };

	//Push
	for (int i = 0; i < 4; ++i)
	{
		s_VertexBuffer.push_back(face[i]);
	}
	for (int i = 0; i < 6; ++i)
	{
		s_IndexBuffer.push_back(faceIndices6[i]);
	}
}