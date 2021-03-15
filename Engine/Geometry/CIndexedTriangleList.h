#pragma once
#include <vector>
#include <DirectXMath.h>

//TODO: Remove / Rework classes that inherit from this one
//Remove this class

template<class T>
class CIndexedTriangleList
{
public:

	CIndexedTriangleList() = default;
	CIndexedTriangleList(std::vector<T> verts_in, std::vector<unsigned short> indices_in)
		:
		vertices(std::move(verts_in)),
		indices(std::move(indices_in))
	{
		assert(vertices.size() > 2);		//There needs to be more than 2 vertices for a triangle to form dum dum
		assert(indices.size() % 3 == 0);
	}

	void Transform(DirectX::FXMMATRIX matrix)
	{
		for (auto& v : vertices)
		{
			const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&v.pos);
			DirectX::XMStoreFloat3(&v.pos, DirectX::XMVector3Transform(pos, matrix));
		}
	}

public:

	std::vector<T> vertices;
	std::vector<unsigned short> indices;
};