#ifndef __LYCAN_VFRAME_H__
#define __LYCAN_VFRAME_H__

#include <cstdint>
#include "memory.h"


namespace lycan {

class Vframe {
public:
    enum class DataFormat {
        Raw,
        GlTexture,
        DxSurface
    };
    enum class PixelColorSpace {
        NotSpecified,
        ARGB32,
        RGBA32,
        YUV_I420,
        YUV_NV12,
        YUV_NV21,
        YUV_YUY2
    };

    Vframe(int width, int height);
    Vframe(int width, int height, void* data, size_t stride, size_t size);
    Vframe(int width, int height, uintptr_t glTexId, uintptr_t glCtx);
    Vframe(int width, int height, void* dxSurface);
    ~Vframe();

    Vframe(Vframe& v);
    Vframe(const Vframe& v);
    Vframe(Vframe&& v);

    Vframe& operator=(Vframe& v);
    Vframe& operator=(const Vframe& v);
    Vframe& operator=(Vframe&& v);

    int32_t width() const { return m_width; }
    int32_t height() const { return m_height; }
    uint64_t timestamp() const { return m_timestamp; }
    DataFormat dataFormat() const { return m_dataFormat; }
    PixelColorSpace pixelColorSpace() const { return m_pixelColorSpace; }
    void* data() { return m_rawData; }
    size_t dataStride() const { return m_rawDataStride; }
    size_t dataSize() const { return m_rawDataSize; }
    uintptr_t glTexture() const { return m_glContext; }
    uintptr_t glContext() const { return m_glContext; }

    void swap(Vframe& v);
private:
    int32_t             m_width;
    int32_t             m_height;
    uint64_t            m_timestamp;
    DataFormat          m_dataFormat;
    PixelColorSpace     m_pixelColorSpace;
    void*               m_rawData;
    size_t              m_rawDataStride;
    size_t              m_rawDataSize;
    Allocator<void*>*   m_allocator;
    uintptr_t           m_glTexture;
    uintptr_t           m_glContext;
};

}

#endif /* __LYCAN_VFRAME_H__ */
