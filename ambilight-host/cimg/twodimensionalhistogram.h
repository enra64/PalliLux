#ifndef cimg_plugin_TWO_DIMENSIONAL_HISTOGRAM
#define cimg_plugin_TWO_DIMENSIONAL_HISTOGRAM



//! Compute the histogram of pixel values \newinstance.
CImg<unsigned char> get_2D_histogram(const unsigned int nb_levels) const {
    const char channel_count = 3;
    const unsigned int height = nb_levels;
    const ulongT size = size();
    const ulongT bytesPerChannel = size / 3;


    CImg<unsigned char> result(nb_levels * channel_count, height, 1, channel_count, 0);

    // space to count all the pixel values
    unsigned int bins[nb_levels][3] = {0};

    // color values we need
    const unsigned char red[] = {255, 0, 0};
    const unsigned char green[] = {0, 255, 0};
    const unsigned char blue[] = {0, 0, 255};

    // iterate over all pixels & channels to count which channel intensity is how often
    for(unsigned int channel = 0; channel < _spectrum; channel++){
        for(unsigned int val = 0; val < bytesPerChannel; val++){
            bins[_data[val + channel * bytesPerChannel]][channel]++;
        }
    }


    for(unsigned int channel = 0; channel < _spectrum; channel++){
        for(unsigned int bin = 0; bin < nb_levels; bin += 3){
            result.draw_line(bin + channel, 0, bin + channel, bins[bin][channel] / nb_levels, red);
            result.draw_line(bin + channel, 0, bin + channel, bins[bin][channel] / nb_levels, green);
            result.draw_line(bin + channel, 0, bin + channel, bins[bin][channel] / nb_levels, blue);
        }
    }

    return result;
}

#endif // cimg_plugin_TWO_DIMENSIONAL_HISTOGRAM
