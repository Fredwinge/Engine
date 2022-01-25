#include "CPixelShader.h"
#include "../GraphicsAssertMacros.h"

//TODO: Ugly, fix this
inline std::wstring convert( const std::string& as )
{
	// deal with trivial case of empty string
	if( as.empty() )    return std::wstring();

	// determine required length of new string
	size_t reqLength = ::MultiByteToWideChar( CP_UTF8, 0, as.c_str(), (int)as.length(), 0, 0 );

	// construct new string of required length
	std::wstring ret( reqLength, L'\0' );

	// convert old string to new string
	::MultiByteToWideChar( CP_UTF8, 0, as.c_str(), (int)as.length(), &ret[0], (int)ret.length() );

	// return new string ( compiler should optimize this away )
	return ret;
}

CPixelShader::CPixelShader(CRenderer* pRenderer, const char* shaderName)
{
	GET_INFOMANAGER(pRenderer);

	std::string path = OUTPUT_DIR;
	path.append("Shaders\\");
	path.append(shaderName);
	std::wstring wPath = convert(path);

	ID3DBlob* pBlob;

	//D3DReadFileToBlob reads CSO files
	GFX_ASSERT_INFO(D3DReadFileToBlob(wPath.c_str(), &pBlob));

	GFX_ASSERT_INFO(pRenderer->GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPixelShader));

	pBlob->Release();
}

CPixelShader::CPixelShader(ID3D11PixelShader* pPixelShader)
	:
	m_pPixelShader(pPixelShader)
{

}

void CPixelShader::Bind(CRenderer* pRenderer) noexcept
{
	pRenderer->GetDeviceContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
}