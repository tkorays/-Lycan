#ifndef __LYCAN_IMGCODEC_JPEG_H__
#define __LYCAN_IMGCODEC_JPEG_H__

#include "imgcodec.hpp"

namespace lycan {

class ImageDecoderJpeg : public ImageDecoder {
public:
    ImageDecoderJpeg();
    virtual ~ImageDecoderJpeg();
    virtual Result readHeader() override;
    virtual Result readData() override;
    void close();
protected:
    FILE* m_fp;
    void* m_jpegSession;
};

}

#endif /* __LYCAN_IMGCODEC_JPEG_H__ */
