#ifndef __LYCAN_IMGCODEC_H__
#define __LYCAN_IMGCODEC_H__
#include <lycan/lycan.h>
#include <lycan/basic.h>

namespace lycan {

class ImageDecoder {
public:
    ImageDecoder();
    virtual ~ImageDecoder() { }
    virtual Result setSource(const std::string& filename);
    virtual Result readHeader() = 0;
    virtual Result readData() = 0;

    int width() const { return m_width; }
    int height() const { return m_height; }

protected:
    int     m_width;
    int     m_height;
    int     m_type;
    std::string m_filename;
};

}

#endif /* __LYCAN_IMGCODEC_H__ */
