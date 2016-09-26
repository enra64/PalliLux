#ifndef WINSCREENSHOT_H
#define WINSCREENSHOT_H

#include "screenshotprovider.h"

#include <minmax.h>
#include <atlimage.h>


// Author:  Ovidiu Cucu
// Website: http://www.codexpert.ro/
// Modified to supply a DIB section to allow for direct pixel access
class CScreenImage : public CImage {
public:
	// initialise the screen dc 
	void createScreenDeviceContext() {
		hDCScreen = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	}

	// release the screen dc 
	void releaseScreenDeviceContext() {
		::DeleteDC(hDCScreen);
	}

	bool CaptureRect(const CRect& rect) throw() {
		// detach and destroy the old bitmap if any attached
		CImage::Destroy();

		// create a screen and a memory device context
		HDC hDCMem = ::CreateCompatibleDC(hDCScreen);

		// configure bitmapinfo
		BITMAPINFO bmi;
		memset(&bmi, 0, sizeof(BITMAPINFO));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = rect.Width();
		bmi.bmiHeader.biHeight = rect.Height(); // top-down
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 24;
		bmi.bmiHeader.biCompression = BI_RGB;

		void* dibData;
		HBITMAP hBitmap = ::CreateDIBSection(hDCScreen, &bmi, DIB_RGB_COLORS, &dibData, NULL, NULL);

		HBITMAP hBmpOld = (HBITMAP)::SelectObject(hDCMem, hBitmap);

		// bit-blit from screen to memory device context
		// note: CAPTUREBLT flag is required to capture layered windows
		DWORD dwRop = SRCCOPY | CAPTUREBLT;
		bool bRet = ::BitBlt(hDCMem, 0, 0, rect.Width(), rect.Height(),
			hDCScreen,
			rect.left, rect.top, dwRop);

		// attach bitmap handle to this object
		Attach(hBitmap);

		// restore the memory DC and perform cleanup
		::SelectObject(hDCMem, hBmpOld);
		::DeleteDC(hDCMem);

		// added after testing, but since i reuse the dc, this may not be too wise
		//::DeleteDC(hDCScreen);

		return bRet;
	}

	bool CaptureScreen() throw() {
		CRect rect(0, 0, ::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
		return CaptureRect(rect);
	}
private:
	HDC hDCScreen;
};

/**
* @brief An implementation of the Screenshot interface for windows
*/
class WinScreenshotProvider : public ScreenshotProvider {
public:
	WinScreenshotProvider();
	~WinScreenshotProvider();

	void takeScreenshot();

	/**
	* @brief takeScreenshot Take a screenshot, converting it to a Magick++ image
	* @param result the resulting image
	* @param d the dimensions the image will be; these are the dimensions requested from X
	* @return the time in seconds required
	*/
	float getScreenCrop(Image& result, const Geometry& d) override;
private:
	// storage space for the image
	Image mImage;
	CScreenImage mScreenshotImage;
};

#endif // WINSCREENSHOT_H
