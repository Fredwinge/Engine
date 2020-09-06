#include "CDXGIInfoManager.h"
#include "CWindow.h"
#include "CGraphics.h"
#include <dxgidebug.h>
#include <memory>
#include "GraphicsAssertMacros.h"

#pragma comment(lib, "dxguid.lib")

//#define GFX_ASSERT_NOINFO(hrcall) if(FAILED(hr = (hrcall))) throw CGraphics::HrException(__LINE__,__FILE__,hr)

CDXGIInfoManager::CDXGIInfoManager()
{
	//Define function signature of DXGIGetDebugInterface
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	//Load the dll that contains the function DXGIGetDebugInterface
	const auto hModDxgiDebug = LoadLibraryExA("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hModDxgiDebug == nullptr)
	{
		CHWND_LAST_EXCEPT();
	}

	//Get address of DXGIGetDebugInterface in dll
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
		);
	if (DxgiGetDebugInterface == nullptr)
	{
		CHWND_LAST_EXCEPT();
	}

	HRESULT hr;
	GFX_ASSERT_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&m_pDXGIInfoQueue)));
}

CDXGIInfoManager::~CDXGIInfoManager()
{
	if (m_pDXGIInfoQueue != nullptr)
		m_pDXGIInfoQueue->Release();
}

void CDXGIInfoManager::Set() noexcept
{
	//Set the index (m_NextLine) so that the next call to GetMessages()
	//will only get errors generated after this function was last called
	m_NextLine = m_pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> CDXGIInfoManager::GetMessages() const
{
	std::vector<std::string> messages;
	const auto end = m_pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = m_NextLine; i < end; ++i)
	{
		HRESULT hr;
		SIZE_T messageLength;
		//Get the size of message i in bytes
		GFX_ASSERT_NOINFO(m_pDXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
		//Allocate memory for message
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		//Get the message and push its description into the vector
		GFX_ASSERT_NOINFO(m_pDXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
		messages.emplace_back(pMessage->pDescription);
	}

	return messages;
}