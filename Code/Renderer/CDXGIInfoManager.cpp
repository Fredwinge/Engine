#include "CDXGIInfoManager.h"
#include "CGraphics.h"
#include <dxgidebug.h>
#include <memory>
#include "GraphicsAssertMacros.h"
#include "WindowsAssertMacros.h"

#pragma comment(lib, "dxguid.lib")

CDXGIInfoManager::CDXGIInfoManager()
{
	//Define function signature of DXGIGetDebugInterface
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	//Load the dll that contains the function DXGIGetDebugInterface
	const auto hModDxgiDebug = LoadLibraryExA("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hModDxgiDebug == nullptr)
	{
		WND_LAST_ERROR();
	}

	//Get address of DXGIGetDebugInterface in dll
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
		);
	if (DxgiGetDebugInterface == nullptr)
	{
		WND_LAST_ERROR();
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
	const UINT64 endLine = m_pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (UINT64 i = m_NextLine; i < endLine; ++i)
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