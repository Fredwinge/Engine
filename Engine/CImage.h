#pragma once
#include <DirectXTex.h>

class CImage
{

	//Error function
	static void CIMAGE_ERROR(int line, const char* file, const char* errorString);

public:

	enum Result
	{
		LOAD_FAILED,
		LOAD_SUCCESSFUL
	};

	static Result MakeFromFile(const wchar_t* filePath, CImage** ppImage);

	size_t GetWidth() { return m_Scratch.GetMetadata().width; }
	size_t GetHeight() { return m_Scratch.GetMetadata().height; }

	uint8_t* GetBuffer() { return m_Scratch.GetImage(0, 0, 0)->pixels; }
	size_t GetBufferSize() { return m_Scratch.GetPixelsSize(); }

	DirectX::ScratchImage* GetScratch() { return &m_Scratch; }

private:

	CImage(DirectX::ScratchImage scratch);

	DirectX::ScratchImage m_Scratch;
};