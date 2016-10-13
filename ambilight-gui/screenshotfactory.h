#ifndef SCREENSHOTFACTORY_H
#define SCREENSHOTFACTORY_H

#ifdef __linux__
    #include <xlibscreenshotprovider.h>
#elif _WIN32
    #include <ddapiscreenshotprovider.h>
#else
    #error Platform not recognized
#endif

/**
 * @brief Helper factory class to simplify getting a win/linux screenshot
 */
struct ScreenshotFactory {
    /**
     * @brief Super ugly function to get win/xlibscreenshotprovider in one line of code
     */
    static std::shared_ptr<ScreenshotProvider> getPlatformAppropriateScreenshotProvider(){
        #ifdef __linux__
            return std::shared_ptr<ScreenshotProvider>(new XlibScreenshotProvider());
        #elif _WIN32
            return std::shared_ptr<ScreenshotProvider>(new DdApiScreenshotProvider());
        #else
            #error Platform not recognized
        #endif
    }
};



#endif // SCREENSHOTFACTORY_H
