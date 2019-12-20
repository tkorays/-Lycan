#include "imgcodec.hpp"

namespace lycan {

ImageDecoder::ImageDecoder() 
    : m_width(0)
    , m_height(0)
    , m_filename("") {

}

Result ImageDecoder::setSource(const std::string& filename) {
    m_filename = filename;
    return Result(0);
}

}