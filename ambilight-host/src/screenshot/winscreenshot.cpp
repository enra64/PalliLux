#include "winscreenshot.h"

#include <ctime>

#include <Windows.Graphics.DirectX.Direct3D11.interop.h>
#include <dxgi.h>
using namespace Windows::Graphics::DirectX::Direct3D11;
using namespace Microsoft::WRL;


using namespace Magick;

WinScreenshot::WinScreenshot()
{
}

WinScreenshot::~WinScreenshot()
{
}

float WinScreenshot::getScreenCrop(Image& result, const Geometry& d)
{
	// benchmarking start
	clock_t start = clock();

	IDirect3DSurface9* pRenderTarget = NULL;
	IDirect3DSurface9* pDestTarget = NULL;
	const char file[] = "Pickture.bmp";
	// sanity checks.
	if (Device == NULL)
		return;

	// get the render target surface.
	HRESULT hr = Device->GetRenderTarget(0, &pRenderTarget);
	// get the current adapter display mode.
	//hr = pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&d3ddisplaymode);

	// create a destination surface.
	hr = Device->CreateOffscreenPlainSurface(DisplayMde.Width,
	                                         DisplayMde.Height,
	                                         DisplayMde.Format,
	                                         D3DPOOL_SYSTEMMEM,
	                                         &pDestTarget,
	                                         NULL);
	//copy the render target to the destination surface.
	hr = Device->GetRenderTargetData(pRenderTarget, pDestTarget);
	//save its contents to a bitmap file.
	hr = D3DXSaveSurfaceToFile(file,
	                           D3DXIFF_BMP,
	                           pDestTarget,
	                           NULL,
	                           NULL);

	// clean up.
	pRenderTarget->Release();
	pDestTarget->Release();

	// return benchmarking value
	return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}
