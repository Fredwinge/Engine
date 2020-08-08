#pragma once
#include <Windows.h>
#include <vector>

//The purpose of this class is to extract additional information out of DirectX errors

class CDXGIInfoManager	//Ugly name
{
public:

	CDXGIInfoManager();
	~CDXGIInfoManager();
	CDXGIInfoManager(const CDXGIInfoManager&) = delete;
	CDXGIInfoManager& operator=(const CDXGIInfoManager&) = delete;

	void Set() noexcept;
	std::vector<std::string> GetMessages() const;

private:

	unsigned long long m_NextLine = 0u;
	struct IDXGIInfoQueue* m_pDXGIInfoQueue = nullptr;
};