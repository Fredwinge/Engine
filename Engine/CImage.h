#pragma once
#include <DirectXTex.h>
#include "IException.h"

class CImage
{
public:

	class Exception : public IException
	{
	public:

		Exception(int line, const char* file, std::string note) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override { return "CImage Graphics Exception"; }
		const std::string& GetNote() const noexcept { return m_sNote; }

	private:

		std::string m_sNote;
	};

	static CImage MakeFromFile(const wchar_t* fileName);

	size_t GetWidth() { return m_Scratch.GetMetadata().width; }
	size_t GetHeight() { return m_Scratch.GetMetadata().height; }

	uint8_t* GetBuffer() { return m_Scratch.GetImage(0, 0, 0)->pixels; }
	size_t GetBufferSize() { return m_Scratch.GetPixelsSize(); }

	DirectX::ScratchImage* GetScratch() { return &m_Scratch; }

private:

	CImage(DirectX::ScratchImage scratch);

	DirectX::ScratchImage m_Scratch;
};