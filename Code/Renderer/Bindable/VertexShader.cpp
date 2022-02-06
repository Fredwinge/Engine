#include "VertexShader.h"
#include "GraphicsAssertMacros.h"

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

CVertexShader::CVertexShader(CRenderer* pRenderer, const char* shaderName)
{
	GET_INFOMANAGER(pRenderer);

	std::string path = OUTPUT_DIR;
	path.append("Shaders\\");
	path.append(shaderName);
	std::wstring wPath = convert(path);

	//D3DReadFileToBlob reads CSO files
	GFX_ASSERT_INFO(D3DReadFileToBlob(wPath.c_str(), &m_pBytecodeBlob));

	GFX_ASSERT_INFO(pRenderer->GetDevice()->CreateVertexShader(m_pBytecodeBlob->GetBufferPointer(), m_pBytecodeBlob->GetBufferSize(), nullptr, &m_pVertexShader));
}

CVertexShader::CVertexShader(ID3D11VertexShader* pVertexShader)
	:
	m_pVertexShader(pVertexShader)
{

}

void CVertexShader::Bind(CRenderer* pRenderer) noexcept
{
	pRenderer->GetDeviceContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
}