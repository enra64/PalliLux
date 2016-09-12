#include "winscreenshotprovider.h"

#include <ctime>

#include <Wincodec.h>             // we use WIC for saving images
#include <d3d9.h>                 // DirectX 9 header
#pragma comment(lib, "d3d9.lib")  // link to DirectX 9 library


using namespace Magick;

WinScreenshotProvider::WinScreenshotProvider()
{
}

WinScreenshotProvider::~WinScreenshotProvider()
{
}
/*
void dump_buffer()
{
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
}*/

void  SaveBitmap(char *filename, HBITMAP hBitmap)
{
	HDC        hdc = NULL;
	unsigned char *pBuf = NULL;
	BITMAPINFO bmp_inf;

	do {
		hdc = GetDC(NULL);
		ZeroMemory(&bmp_inf, sizeof(BITMAPINFO));
		bmp_inf.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		// Passing a NULL gets just the header info containing 
		// the width and height which are used to compute the 
		// size required for a malloc
		GetDIBits(hdc, hBitmap, 0, 0, NULL, &bmp_inf, DIB_RGB_COLORS);

		if (bmp_inf.bmiHeader.biSizeImage <= 0)
			bmp_inf.bmiHeader.biSizeImage = bmp_inf.bmiHeader.biWidth*abs(bmp_inf.bmiHeader.biHeight)*(bmp_inf.bmiHeader.biBitCount + 7) / 8;

		if ((pBuf = (unsigned char*) malloc(bmp_inf.bmiHeader.biSizeImage + sizeof(BITMAPINFOHEADER))) == NULL)
		{
			MessageBox(NULL, "Unable to Allocate Bitmap Memory", "Error", MB_OK | MB_ICONERROR);
			break;
		}
		bmp_inf.bmiHeader.biCompression = BI_RGB;
		// Now get the whole image
		GetDIBits(hdc, hBitmap, 0, bmp_inf.bmiHeader.biHeight, pBuf + sizeof(BITMAPINFOHEADER), &bmp_inf, DIB_RGB_COLORS);
		{
			int i;
			unsigned char *p, *q;
			MagickWand *wand;

			// Copy the separate bmp_inf into the DIB info header which is 
			// now contiguous with the image data in pBuf
			p = (unsigned char *)&bmp_inf;
			q = pBuf;
			for (i = 0; i<sizeof(BITMAPINFOHEADER); i++)*q++ = *p++;

			Image img;
			Blob b(pBuf, bmp_inf.bmiHeader.biSizeImage + sizeof(BITMAPINFOHEADER));
			img.read(b);
			img.write("zsz.png");
		}
	} while (0);
	if (hdc)     ReleaseDC(NULL, hdc);
	if (pBuf)    free(pBuf);

}


float WinScreenshotProvider::getScreenCrop(Image& result, const Geometry& d)
{
	// benchmarking start
	clock_t start = clock();
	/*
	// get the device context of the screen
	HDC hScreenDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	// and a device context to put it in
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

	int width = GetDeviceCaps(hScreenDC, HORZRES);
	int height = GetDeviceCaps(hScreenDC, VERTRES);

	// maybe worth checking these are positive values
	HGDIOBJ hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);

	// get a new bitmap
	HGDIOBJ hOldBitmap = SelectObject(hMemoryDC, hBitmap);

	BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);
	hBitmap = SelectObject(hMemoryDC, hOldBitmap);

	// work with the bitmap
	BITMAPINFOHEADER* header = (BITMAPINFOHEADER*) GlobalLock(hBitmap);

	// note that if we are working with uncompressed bitmaps (32 bits), header->biSizeImage might be 0
	int length = header->biSize + (00000001 << header->biBitCount) * sizeof(RGBQUAD) + header->biSizeImage; 

	result.read(Blob(header, length));

	// clean up
	DeleteDC(hMemoryDC);
	DeleteDC(hScreenDC);

	return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;*/


	
	HDC hScreen = GetDC(GetDesktopWindow());
	int ScreenX = GetDeviceCaps(hScreen, HORZRES);
	int ScreenY = GetDeviceCaps(hScreen, VERTRES);

	HDC hdcMem = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, ScreenX, ScreenY);
	HGDIOBJ hOld = SelectObject(hdcMem, hBitmap);
	BitBlt(hdcMem, 0, 0, ScreenX, ScreenY, hScreen, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hOld);

	BITMAPINFOHEADER bmi = { 0 };
	bmi.biSize = sizeof(BITMAPINFOHEADER);
	bmi.biPlanes = 1;
	bmi.biBitCount = 32;
	bmi.biWidth = ScreenX;
	bmi.biHeight = -ScreenY;
	bmi.biCompression = BI_RGB;
	bmi.biSizeImage = 0;// 3 * ScreenX * ScreenY;


	//if (ScreenData)
	//	free(ScreenData);
	uint8_t* ScreenData = (uint8_t*)malloc(4 * ScreenX * ScreenY);

	GetDIBits(hdcMem, hBitmap, 0, ScreenY, ScreenData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);

	// wörk
	//result.modifyImage();
	//Pixels pixelCache(result);

	// actually Quantum
	//unsigned short* pixels = pixelCache.get(0, 0, ScreenX, ScreenY);

	std::string size = std::to_string(ScreenX) + "x" + std::to_string(ScreenY);

	//Image b;

	//b.read(ScreenX, ScreenY, "BGRA", Magick::LongPixel, ScreenData);

	for (int x = 0; x < ScreenX; x++) {
		for (int y = 0; y < ScreenY; y++) {
			result.pixelColor(x, y, Color(
				double(ScreenData[4 * ((y*ScreenX) + x) + 0]) / 255.0,
				double(ScreenData[4 * ((y*ScreenX) + x) + 1]) / 255.0,
				double(ScreenData[4 * ((y*ScreenX) + x) + 2]) / 255.0));
		}
	}

	// finish changes
	//pixelCache.sync();

	ReleaseDC(GetDesktopWindow(), hScreen);
	DeleteDC(hdcMem);
	DeleteObject(hBitmap);

	// return benchmarking value
	return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}