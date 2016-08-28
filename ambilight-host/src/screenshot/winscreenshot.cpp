#include "winscreenshot.h"

#include <ctime>

#include <dxgi.h>

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

	HDC hScreen = GetDC(GetDesktopWindow());
	int ScreenX = GetDeviceCaps(hScreen, HORZRES);
	int ScreenY = GetDeviceCaps(hScreen, VERTRES);
	uint8_t* ScreenData = 0;

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

	if (ScreenData)
		free(ScreenData);

	size_t screenDataLength = 4 * ScreenX * ScreenY;

	ScreenData = new uint8_t[screenDataLength];

	GetDIBits(hdcMem, hBitmap, 0, ScreenY, ScreenData, reinterpret_cast<BITMAPINFO*>(&bmi), DIB_RGB_COLORS);

	result.read(Blob(ScreenData, screenDataLength));

	ReleaseDC(GetDesktopWindow(), hScreen);
	DeleteDC(hdcMem);
	DeleteObject(hBitmap);

	// return benchmarking value
	return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}