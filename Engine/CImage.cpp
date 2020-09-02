#include "CImage.h"
#include <sstream>

CImage::CImage(DirectX::ScratchImage scratch)
	:
	m_Scratch(std::move(scratch))
{ }

CImage CImage::MakeFromFile(const wchar_t* fileName)
{

	DirectX::ScratchImage scratch;

	HRESULT hr;

	hr = DirectX::LoadFromWICFile(fileName, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, nullptr, scratch);

	if (FAILED(hr))
	{
		//failure
		throw CImage::Exception(__LINE__, __FILE__, "Failed to load image");
	}

	if (scratch.GetImage(0, 0, 0)->format != DXGI_FORMAT_B8G8R8A8_UNORM)
	{
		DirectX::ScratchImage converted;

		hr = DirectX::Convert(*scratch.GetImage(0, 0, 0), 
			DXGI_FORMAT_B8G8R8A8_UNORM, 
			DirectX::TEX_FILTER_DEFAULT, 
			DirectX::TEX_THRESHOLD_DEFAULT, 
			converted);

		if (FAILED(hr))
		{
			throw CImage::Exception(__LINE__, __FILE__, "Failed to convert image");
		}

		return CImage(std::move(converted));
	}
	
	return CImage(std::move(scratch));

}

//EXCEPTION STUFF
CImage::Exception::Exception(int line, const char* file, std::string note) noexcept
	:
	IException(line, file),
	m_sNote(std::move(note))
{
}

const char* CImage::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << IException::what() << std::endl << "[Note] " << GetNote();

	m_sWhatBuffer = oss.str();

	return m_sWhatBuffer.c_str();
}