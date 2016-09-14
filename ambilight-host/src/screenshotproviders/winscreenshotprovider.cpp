#include "winscreenshotprovider.h"

#include <ctime>
#include <stdexcept>
#include <assert.h>
#include <iostream>
#include <fstream>

#include <windows.h>
#include <gdiplus.h>
#include <stdio.h>

using namespace Magick;
using namespace std;

WinScreenshotProvider::WinScreenshotProvider()
{
	mPixelBuffer = new uint8_t[8294400];
}

WinScreenshotProvider::~WinScreenshotProvider()
{
	delete[] mPixelBuffer;
}

float WinScreenshotProvider::getScreenCrop(Magick::Image& result, const Geometry& d)
{
	// benchmarking start
	clock_t start = clock();

	int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	HWND hDesktopWnd = GetDesktopWindow();
	HDC hDesktopDC = GetDC(hDesktopWnd);
	HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
	HBITMAP hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, nScreenWidth, nScreenHeight);
	SelectObject(hCaptureDC, hCaptureBitmap);
	BitBlt(hCaptureDC, 0, 0, nScreenWidth, nScreenHeight,
		hDesktopDC, 0, 0, SRCCOPY | CAPTUREBLT);

	BITMAPINFO info = { 0 };
	info.bmiHeader.biSize = sizeof(info);

	if (0 == GetDIBits(hCaptureDC, hCaptureBitmap, 0, 0, NULL, &info, DIB_RGB_COLORS))
		throw std::runtime_error("could not get bitmapinfo");


	//info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biCompression = BI_RGB;// uncompressed RGB
	//info.bmiHeader.biHeight = abs(info.bmiHeader.biHeight);


	// get the actual bitmap buffer
	if (0 == GetDIBits(
		hCaptureDC,
		hCaptureBitmap,
		0,
		info.bmiHeader.biHeight,
		mPixelBuffer,
		&info,
		DIB_RGB_COLORS))
		throw std::runtime_error("could not get bitmap buffer");

	ofstream file("terst.argb", ios::trunc | ios::binary | ios::out);
	if (!file.good())
		cout << "at" << endl;
	file.write((char*) mPixelBuffer, info.bmiHeader.biSizeImage);
	file << fflush;
	file.close();

	//result.read(info.bmiHeader.biWidth, info.bmiHeader.biHeight, "ARGB", CharPixel, mPixelBuffer);
	result.read("terst.rgba");

	//result.size(Geometry(info.bmiHeader.biWidth, info.bmiHeader.biHeight));
	//result.modifyImage();

	/*or:
	setpixels(0, 0, w, h)
	readpixels(8bit, pixelBuffer);
	*/

	//result.setPixels(0, 0, info.bmiHeader.biWidth, info.bmiHeader.biHeight);
	//result.readPixels(MagickCore::QuantumType::RGBQuantum, mPixelBuffer);

	/*Magick::Quantum* pixels = result.getPixels(0, 0, info.bmiHeader.biWidth, info.bmiHeader.biHeight);



	for (long row = 0; row < info.bmiHeader.biHeight; row++) {
		for (long col = 0; col < info.bmiHeader.biWidth; col++) {
			result.pixelColor(col, row,
				Color(double(mPixelBuffer[row * info.bmiHeader.biHeight + info.bmiHeader.biWidth + 0]) / 255.0 * QuantumRange,
					double(mPixelBuffer[row * info.bmiHeader.biHeight + info.bmiHeader.biWidth + 0]) / 255.0 * QuantumRange,
					double(mPixelBuffer[row * info.bmiHeader.biHeight + info.bmiHeader.biWidth + 0]) / 255.0 * QuantumRange));
				// convert our 0-255 channels to magick's 0-QuantumRange channels
				//pixels[row * info.bmiHeader.biWidth + col + 0] = 0;//double(mPixelBuffer[row * info.bmiHeader.biHeight + info.bmiHeader.biWidth + 0]) / 255.0 * QuantumRange;
			//pixels[row * info.bmiHeader.biWidth + col + 1] = 0;// double(mPixelBuffer[row * info.bmiHeader.biHeight + info.bmiHeader.biWidth + 1]) / 255.0 * QuantumRange;
			//pixels[row * info.bmiHeader.biWidth + col + 2] = 0;// double(mPixelBuffer[row * info.bmiHeader.biHeight + info.bmiHeader.biWidth + 2]) / 255.0 * QuantumRange;
		}
	}*/
	//result.syncPixels();

	ReleaseDC(hDesktopWnd, hDesktopDC);
	DeleteDC(hCaptureDC);
	DeleteObject(hCaptureBitmap);

	// return benchmarking value
	return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}