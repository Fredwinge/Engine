#pragma once
#include "Bindable/IndexBuffer.h"
#include "Bindable/VertexBuffer.h"
#include "Math/Matrix.h"

//TODO: put somewhere better?
//TODO: Binormal / Tangent vectors
//https://www.rastertek.com/dx10tut20.html have a look
struct SVertexData
{
	Vector3 Position = Vector3(0.0f, 0.0f, 0.0f);
	Vector2 TexCoord = Vector2(0.0f, 0.0f);
	Vector3 Normal = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 Binormal = Vector3(0.0f);
	Vector3 Tangent = Vector3(0.0f);

	bool operator==(const SVertexData other)
	{
		if (Position == other.Position && TexCoord == other.TexCoord && Normal == other.Normal && Binormal == other.Binormal && Tangent == other.Tangent)
			return true;

		return false;
	}
};