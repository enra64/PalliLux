#ifndef cimg_plugin_LOAD_CUSTOM_CHANNEL_ORDER
#define cimg_plugin_LOAD_CUSTOM_CHANNEL_ORDER

unsigned char charPosition(const unsigned char searched, const char* order, const unsigned int channels) {
    for(unsigned int i = 0; i < channels; i++) {
        if(order[i] == searched){
            return i;
        }
    } 

    return 255;
} 

//! Construct image with specified size and initialize pixel values from a memory buffer \specialization.
//! channel_order may be any order of "BGRA" with 4 channels or any order of "BGR" with 3 channels at this time.
//! source_channel_count may be not equal to size_c to allow ignoring empty alpha channels.
//! size_z is 1, as i did not want to think about handling more than two dimensions
//! if an allocated shared_buffer is given, source will be copied there in the right order.
//! if shared_buffer is nullptr, a new buffer will be allocated, and source will be copied there in the right order.
//! pitch is the byte count of a row. this is relevant because it might not be the same value as the count of rgb bytes per row (see windows DIB...)
//! if pitch is zero, it will default to the calculated amount of bytes per row (eg 3*width)
void read(const char *const source,
          const char* channel_order,
          const unsigned int source_channel_count,
          const unsigned int size_x,
          const unsigned int size_y=1,
          const unsigned int size_c=1,
          unsigned char* const shared_buffer = nullptr,
          int pitch = 0){

    // check channel count validity
    if(size_c != 3 && size_c != 4 && source_channel_count != 3 && source_channel_count != 4)
        throw std::invalid_argument("only channel counts of 3 or four are valid");

    if(source_channel_count < size_c)
        throw std::invalid_argument("the source must not have less channels than the target!");

    // size_z must be 1, i dont wanna write code to handle that
    const unsigned int size_z = 1;

    // calculate overage image size
    const size_t siz = (size_t)size_x*size_y*size_z*size_c;

    if (source && siz) {
        // save member variables
        _width = size_x;
        _height = size_y;
        _depth = size_z;
        _spectrum = size_c;
        // if shared_buffer is nullptr, is_shared is false
        _is_shared = shared_buffer;

        // we want to use shared_buffer as _data
        if(shared_buffer){
            _data = shared_buffer;
        }
        // _data should be our own buffer
        else {
            try {
                // _data may have been previously initialized, so we just delete whatever was there
                delete[] _data;

                // allocate new _data space
                _data = new T[siz];
            } catch (...) {
                _width = _height = _depth = _spectrum = 0;
                _data = 0;
                throw std::bad_alloc();
            }
        }

        // check whether we have to calculate our own pitch
        if(pitch == 0)
            pitch = source_channel_count * _width;


        // determine the relative offset of each channel
        unsigned char rOff, gOff, bOff, aOff;
        rOff = charPosition('R', channel_order, size_c);
        gOff = charPosition('G', channel_order, size_c);
        bOff = charPosition('B', channel_order, size_c);

        if(source_channel_count == 4)
            aOff = charPosition('A', channel_order, size_c);

        // now, _data is set (hopefully) to the correct buffer location, and we can start copying the data:

        uint8_t *redStart, *greenStart, *blueStart, *alphaStart;

        // for each row
        for (unsigned int row = 0; row < _height; row++) {
                // where does the data for this row start in the source buffer?
                const unsigned char *rowStart = reinterpret_cast<const unsigned char*>(source + pitch * row);

                // where does this row start in _data?
                redStart = this->data(0, row, 0, 0);
                greenStart = this->data(0, row, 0, 1);
                blueStart = this->data(0, row, 0, 2);

                // this is only valid and relevant if the target channel count has an alpha
                if(_spectrum == 4)
                    alphaStart = this->data(0, row, 0, 3);

                // copy R, then G, then B into the result images buffer
                for (size_t pixelIndex = 0; pixelIndex < _width; pixelIndex++) {
                        blueStart[pixelIndex] = rowStart[source_channel_count * pixelIndex + bOff];
                        greenStart[pixelIndex] = rowStart[source_channel_count * pixelIndex + gOff];
                        redStart[pixelIndex] = rowStart[source_channel_count * pixelIndex + rOff];

                        // if an alpha channel exists, copy it
                        if(source_channel_count == 4 && _spectrum == 4)
                            alphaStart[pixelIndex] = rowStart[source_channel_count * pixelIndex + aOff];
                }
        }


    } else {
        _width = _height = _depth = _spectrum = 0;
        _is_shared = false;
        _data = 0;
    }
}

//! Construct image with specified size and initialize pixel values from a memory buffer \specialization.
//! channel_order may be any order of "BGRA" with 4 channels or any order of "BGR" with 3 channels at this time.
//! size_z is 1, as i did not want to think about handling more than two dimensions
//! if shared_buffer is true, and an allocated shared_buffer is given, source will be copied there in the right order.
//! if shared_buffer is false, and shared_buffer is nullptr, a new buffer will be allocated, and source will be copied there in the right order.
//! pitch is the byte count of a row. this is relevant because it might not be the same value as the count of rgb bytes per row (see windows DIB...)
//! if pitch is zero, it will default to the calculated amount of bytes per row (eg 3*width)
CImg (
        unsigned char *const source,
        const char* channel_order,
        const unsigned int size_x,
        const unsigned int size_y=1,
        const unsigned int size_c=1,
        unsigned char* const shared_buffer = nullptr,
        const int pitch = 0) {
    read(source, channel_order, size_x, size_y, size_c, shared_buffer, pitch);
}

#endif // cimg_plugin_LOAD_CUSTOM_CHANNEL_ORDER
