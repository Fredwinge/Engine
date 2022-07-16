#include "CCube.h"

std::vector<SVertexData> CCube::s_VertexBuffer;
std::vector<unsigned short> CCube::s_IndexBuffer;

bool CCube::s_bCubeCreated = false;

void CCube::CreateBuffers()
{
	const float side = 0.5f;
	//RIGHT FACE
	SVertexData face[4];
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

	//Push
	for (int i = 0; i < 4; ++i)
	{
		s_VertexBuffer.push_back(face[i]);
	}

	//LEFT FACE
	faceNormal = Vector3(-side, 0.0f, 0.0f);

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

	//Push
	for (int i = 0; i < 4; ++i)
	{
		s_VertexBuffer.push_back(face[i]);
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

	//Push
	for (int i = 0; i < 4; ++i)
	{
		s_VertexBuffer.push_back(face[i]);
	}

	//DOWN FACE
	faceNormal = Vector3(0.0f, -side, 0.0f);

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

	//Push
	for (int i = 0; i < 4; ++i)
	{
		s_VertexBuffer.push_back(face[i]);
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

	//Push
	for (int i = 0; i < 4; ++i)
	{
		s_VertexBuffer.push_back(face[i]);
	}

	//BACKWARD FACE
	faceNormal = Vector3(0.0f, 0.0f, -side);

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

	//Push
	for (int i = 0; i < 4; ++i)
	{
		s_VertexBuffer.push_back(face[i]);
	}

	uint16 Indices[36] =
	{
		0, 1, 2, 3, 2, 1,
		6, 5, 4, 5, 6, 7,
		8, 9, 10, 11, 10, 9,
		14, 13, 12, 13, 14, 15,
		18, 17, 16, 17, 18, 19,
		20, 21, 22, 23, 22, 21
	};

	for (int i = 0; i < 36; ++i)
	{
		s_IndexBuffer.push_back(Indices[i]);
	}
}