#include "d3dscreenshotprovider.h"

#include <ctime>
#include <iostream>
#include <assert.h>

using namespace cimg_library;
using namespace std;

D3DScreenshotProvider::D3DScreenshotProvider()
{
	HWND hwnd = GetDesktopWindow();
	HWND dummyHWND = CreateWindowA("STATIC", "dummy", NULL, 0, 0, 100, 100, NULL, NULL, NULL, NULL);

	// create Direct3D
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	
	// 
	D3DDISPLAYMODE	ddm;
	g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &ddm);


	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.Windowed = true;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	d3dpp.BackBufferFormat = ddm.Format;
	d3dpp.BackBufferHeight = ddm.Height;
	d3dpp.BackBufferWidth = ddm.Width;
	//d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = dummyHWND;
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	//d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	// 
	g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, dummyHWND, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice);

	// 
	g_pd3dDevice->CreateOffscreenPlainSurface(ddm.Width, ddm.Height, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &g_pSurface, NULL);
}

D3DScreenshotProvider::~D3DScreenshotProvider()
{
	delete[] mImageBuffer;
}

void D3DScreenshotProvider::takeScreenshot()
{
	int width = ::GetSystemMetrics(SM_CXSCREEN); 
	int height = ::GetSystemMetrics(SM_CYSCREEN);

	// if we have never allocated memory for the image, do it now, but otherwise, we just keep reusing the memory to avoid allocation
	if (mImageBuffer == nullptr)
		mImageBuffer = new uint8_t[CIMG_CHANNEL_COUNT * height * width];

	// create result image with own bufferspace
	mImage = Image(mImageBuffer, width, height, CIMG_2D_Z_LEVEL_COUNT, CIMG_CHANNEL_COUNT, CIMG_SHARED_MEMORY);

	g_pd3dDevice->CreateOffscreenPlainSurface(width, height, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &g_pSurface, NULL);
	g_pd3dDevice->GetFrontBufferData(0, g_pSurface);
	D3DLOCKED_RECT lockedRect;
	g_pSurface->LockRect(&lockedRect, NULL, D3DLOCK_NO_DIRTY_UPDATE | D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY);
		
	// get image data from surface
	uint8_t* bits = static_cast<uint8_t*>(lockedRect.pBits);

	for (int row = 0; row < height; row++) {
		// get a pointer to the DIB section row start
		// pitch -> how many bytes are used per line (not necessarily equal to the amount of RGB bytes due to padding)
		uint8_t* rowStart = bits + lockedRect.Pitch * row;

		// get pointers to the CImg color locations
		uint8_t* redStart = mImage.data(0, row, 0, CIMG_RED_CHANNEL);
		uint8_t* greenStart = mImage.data(0, row, 0, CIMG_GREEN_CHANNEL);
		uint8_t* blueStart = mImage.data(0, row, 0, CIMG_BLUE_CHANNEL);

		// copy R, then G, then B into the result images buffer
		for (int pixelIndex = 0; pixelIndex < width; pixelIndex++) {
			redStart[pixelIndex] = rowStart[4 * pixelIndex + 2];
			greenStart[pixelIndex] = rowStart[4 * pixelIndex + 1];
			blueStart[pixelIndex] = rowStart[4 * pixelIndex + 0];
		}
	}

	g_pSurface->UnlockRect();
	g_pSurface->Release();

	//mImage.save("test.bmp");
}

float D3DScreenshotProvider::getScreenCrop(Image & result, const Geometry & d)
{
	// benchmarking
	clock_t start = clock();

	// TODO: this is somewhat fucked up if you have more than one monitor, as currently only the center one will be captured
	result = mImage.get_crop(d.left(), d.top(), d.right(), d.bottom());

	// benchmarking end
	return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}
