#include "CImage.h"
#include <sstream>

//for converting wchar_t* to char* //somewhat ugly, maybe find a 'better' solution?
#include <comdef.h>
#include <string>

CImage::CImage(DirectX::ScratchImage scratch)
	:
	m_Scratch(std::move(scratch))
{ }

CImage::Result CImage::MakeFromFile(const wchar_t* filePath, CImage** ppImage)
{

	DirectX::ScratchImage scratch;

	HRESULT hr;

	hr = DirectX::LoadFromWICFile(filePath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, nullptr, scratch);

	if (FAILED(hr))
	{
		//Convert wchar* to char*
		_bstr_t bStr(filePath);
		char* cStr(bStr);
		std::string errorStr = "Failed to load image: ";

		errorStr.append(cStr);

		//failure
		CIMAGE_ERROR(__LINE__, __FILE__, errorStr.c_str());

		return LOAD_FAILED;
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
			//Convert wchar* to char*
			_bstr_t bStr(filePath);
			char* cStr(bStr);
			std::string errorStr = "Failed to convert image: ";

			errorStr.append(cStr);

			CIMAGE_ERROR(__LINE__, __FILE__, errorStr.c_str());

			return LOAD_FAILED;
		}

		*ppImage = new CImage(std::move(converted));

		return LOAD_SUCCESSFUL;
	}
	
	*ppImage = new CImage(std::move(scratch));

	return LOAD_SUCCESSFUL;

}

void CImage::CIMAGE_ERROR(int line, const char* file, const char* errorString)
{
	std::ostringstream oss;
	oss << "CImage error!" << std::endl 
		<< "[File] " << file << std::endl << "[Line] " << line << std::endl 
		<< "[Note] " << errorString;
	
	OutputDebugString(oss.str().c_str());

}