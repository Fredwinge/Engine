#include "IBindable.h"

ID3D11DeviceContext* IBindable::GetContext(CGraphics& gfx) noexcept
{
	return gfx.m_pDeviceContext;
}

ID3D11Device* IBindable::GetDevice(CGraphics& gfx) noexcept
{
	return gfx.m_pDevice;
}

CDXGIInfoManager& IBindable::GetInfoManager(CGraphics& gfx) noexcept
{
#ifndef NDEBUG
	return gfx.m_InfoManager;
#else
	throw std::logic_error("You did dun fucked up [tried to access gfx.m_InfoManager in release]");
#endif
}