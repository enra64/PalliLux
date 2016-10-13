#ifndef cimg_plugin_TWO_DIMENSIONAL_HISTOGRAM
#define cimg_plugin_TWO_DIMENSIONAL_HISTOGRAM

//! Compute the histogram of pixel values \newinstance.
CImg get_2D_histogram(const unsigned int nb_levels) const {
    const char channel_count = 3;
    const unsigned int height = nb_levels;
    const ulongT bytesPerChannel = size() / 3;
    const float nb_levels_float = nb_levels;


    CImg<unsigned char> result(nb_levels * channel_count, height, 1, channel_count, 0);

    // space to count all the pixel values
    unsigned int bins[nb_levels][3] = {0};

    const float binValueLimitizer = nb_levels_float / 255.f;
    unsigned int maxBinValue = 0;

    // iterate over all pixels to count which channel intensity is how often
    for(ulongT i = 0; i < size(); i++){
        // at(i) returns the intensity of the current pixel, which we scale to between 0 and nb_levels
        // i/bytesPerChannel returns the channel: either 0, 1 or 2
        const unsigned int bin = at(i) * binValueLimitizer;
        bins[bin][i / bytesPerChannel]++;

        // save maximum bin value to enable scaling
        maxBinValue = bin > maxBinValue ? bin : maxBinValue;
    }

    // color values we need
    const unsigned char tdh_colors[3][3] = {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255}};

    for(unsigned int bin = 0; bin < nb_levels; bin++){
        for(int channel = 0; channel < 3; channel++){
            unsigned int scaledHeight = (float) bins[bin][channel] / (float) maxBinValue;// / maxvalue of bins * (float) height;
            result.draw_line(
                        3 * bin + channel,
                        height - scaledHeight,
                        3 * bin + channel,
                        height,
                        tdh_colors[channel]);
        }
    }

    return result;
}

#endif // cimg_plugin_TWO_DIMENSIONAL_HISTOGRAM
