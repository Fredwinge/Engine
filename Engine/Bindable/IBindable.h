#pragma once
#include "../CGraphics.h"

class IBindable
{
public:

	//Bind is used to bind all the neccessary objects to the pipeline, such as vertex buffer etc.
	virtual void Bind(CGraphics& gfx) noexcept = 0;

protected:

	//The bindable objects will need access to private variables in CGraphics
	//since IBindable has access to these objects we can use these functions to grant them access
	static ID3D11DeviceContext* GetContext(CGraphics& gfx) noexcept;
	static ID3D11Device* GetDevice(CGraphics& gfx) noexcept;
	static CDXGIInfoManager& GetInfoManager(CGraphics& gfx) noexcept;
};