#ifndef __LYCAN_GRAPHIC_H__
#define __LYCAN_GRAPHIC_H__

#include <cstdint>
#include <string>
#include "exception.h"

namespace lycan {

class Rgba {
public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    Rgba(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) : r(_r), g(_g), b(_b), a(_a) { }
    template <typename T> T rgba() const { return static_cast<T>(r << 24 | g << 16 | b << 8 | a); }
    template <typename T> T bgra() const { return static_cast<T>(b << 24 | g << 16 | r << 8 | a); }
    template <typename T> T argb() const { return static_cast<T>(a << 24 | r << 16 | g << 8 | b); }
    template <typename T> T abgr() const { return static_cast<T>(a << 24 | b << 16 | g << 8 | r); }
};




template <typename T, int N>
class Vec {
public:
    typedef T ValueType;

    ValueType data[N];

    size_t sizeOf() const { return sizeof(data); }
    size_t channels() const { return N; }
    template <typename Q> Q ConvertTo() { return static_cast<Q>(data); }
};

typedef Vec<unsigned int, 1> Vec1U;
typedef Vec<int, 1> Vec1S;
typedef Vec<unsigned char, 4> Vec4UB;
typedef Vec<char, 4> Vec4B;


template <typename T>
class MatrixIterator {
public:
    MatrixIterator(int rows, int cols, void* data, int start)
        : m_rows(rows), m_cols(cols), m_data(static_cast<T*>(data)), m_index(start) {}

    T& operator *() { return m_data[m_index]; }
    T& operator [](ptrdiff_t i) { return m_data[i]; }
    MatrixIterator& operator +=(ptrdiff_t offset) {
        m_index = std::max(m_index + offset, m_rows*m_cols - 1);
        return *this;
    }
    MatrixIterator& operator ++() {
        m_index = std::min(m_index + 1, m_rows*m_cols);
        return *this;
    }
    MatrixIterator& operator ++(int) {
        m_index = std::min(m_index + 1, m_rows*m_cols);
        return *this;
    }
    MatrixIterator& operator -=(ptrdiff_t offset) {
        m_index = std::max(m_index - offset, 0);
        return *this;
    }
    MatrixIterator& operator --() {
        m_index = std::max(m_index - 1, 0);
        return *this;
    }

    bool operator ==(MatrixIterator& it) const {
        return it->m_index == m_index && it->m_data == m_data;
    }
    bool operator !=(MatrixIterator& it) const {
        return it.m_index != m_index || it.m_data != m_data;
    }
private:
    int m_rows;
    int m_cols;
    T* m_data;
    int m_index;
};


template <typename T>
class Vector {
public:
    typedef T ValueType;
    typedef MatrixIterator<T> Iterator;

    //Vector();
    Vector(size_t count) : m_count(count), m_managed(true) { m_data = new ValueType[count]; }
    Vector(ValueType* p, size_t size) : Vector(p, size, false) {  }
    Vector(ValueType* p, size_t count, bool copy) : m_count(count), m_managed(copy) {
        if (copy) {
            m_data = new ValueType[count];
            memcpy(m_data, p, sizeof(ValueType)*m_count);
        }
        else {
            m_data = p;
        }
    }
    ~Vector() { if (m_managed && m_data) delete m_data; }

    Vector clone() const { return Vector(m_data, m_count, true); }
    size_t count() const { return m_count; }

    const ValueType& at(int i) const { return m_data[i]; }
    ValueType& at(int i) { return m_data[i]; }
    ValueType& operator[](int i) { return m_data[i]; }
    const ValueType& operator[](int i) const { return m_data[i]; }

    Iterator begin() { return MatrixIterator<T>(1, m_count, m_data, 0); }
    Iterator begin() const { return MatrixIterator<T>(1, m_count, m_data, 0); }
    Iterator end() { return MatrixIterator<T>(1, m_count, m_data, 1* m_count); }
    Iterator end() const { return MatrixIterator<T>(1, m_count, m_data, 1* m_count); }
private:
    ValueType* m_data;
    size_t m_count;
    bool m_managed;

};

template <typename T>
class Matrix {
public:
    typedef T ValueType;
    typedef MatrixIterator<T> Iterator;

    Matrix() : data(0), size(0), managed(false), width(0), height(0) { }
    Matrix(int _width, int _height) : width(_width), height(_height) {
        size = sizeof(ValueType)*width*height;
        data = new ValueType[width*height];
        managed = true;
    }
    Matrix(ValueType* _data, size_t _size, bool _refer, int _width, int _height)
        : data((ValueType*)_data), size(_size), managed(!_refer), width(_width), height(_height) { }
    Matrix(ValueType* _data, size_t _size, int _width, int _height) 
        : Matrix(_width, _height) { memcpy(data, _data, _size); }
    ~Matrix() { if (managed && data) { delete data; } }

    Matrix(const Matrix& m) : Matrix(m.data, m.sizeOfData(), m.cols(), m.rows()) {  }
    Matrix& operator=(const Matrix& m) { return Matrix(m.ptrData(), m.sizeOfData(), m.cols(), m.rows()); }
    Vector<T> operator[](int index) { return Vector<T>(data + width*index, width); }

    ValueType at(int x, int y) { return data[y*width + x]; }
    template <typename Q> Q at(int x, int y) { return *(Q*)&data[y*width + x]; }
    Vector<T> row(int line) { return Vector<T>(data + width*line, width); }
    size_t sizeOfData() const { return width * height * sizeof(ValueType); }

    bool create(int cols, int rows) { 
        if (managed && data && cols == width && rows == height) return true; 
        if (managed && data) delete data;
        width = cols;
        height = rows;
        size = sizeof(ValueType)*cols*rows;
        data = new ValueType[cols*height];
        managed = true;
    }
    Matrix clone() { return Matrix(data, size, width, height); }
    void release() { width = 0; height = 0; if (managed && data) { delete data; } data = nullptr, size = 0; managed = false; }
    ValueType* ptrData() { return data; }
    int cols() const { return width; }
    int rows() const { return height; }

    Iterator begin() { return MatrixIterator<T>(height, width, data, 0); }
    Iterator begin() const { return MatrixIterator<T>(height, width, data, 0); }
    Iterator end() { return MatrixIterator<T>(height, width, data, height*width); }
    Iterator end() const { return MatrixIterator<T>(height, width, data, height*width); }

private:
    ValueType*  data;
    size_t      size;
    bool        managed;
    int         width;
    int         height;
};

class RefCount {
public:
    RefCount() : count(0) { }
    RefCount(int cnt) : count(cnt) {  }
    ~RefCount() {  }

    void set(int cnt) { count = cnt; }
    int ref() { if (count >= 0) ++count; return count; }
    int release() { if (count > 0) --count; return count; }
    bool norefs() { return count == 0; }

    // -1 indicates that the resource is not belonged to the object.
    int count;
};


class Image {
public:
    template <typename T>
    class Iterator : public MatrixIterator<T> {
    public:
        Iterator(int rows, int cols, void* data, int start) : MatrixIterator<T>(rows, cols, data, start) {}
    };

    Image() : m_rows(0), m_cols(0), m_data(nullptr), m_pixelSize(0), m_size(0), m_refs(nullptr) { }

    Image(int rows, int cols, int pixelSize) : m_rows(rows), m_cols(cols), m_pixelSize(pixelSize), 
        m_size(rows*cols*pixelSize), m_refs(new RefCount(1)) { m_data = new uint8_t[m_size]; }

    Image(lycan::SizeI size, int pixelSize) : Image(size.cy, size.cx, pixelSize) { }

    Image(int rows, int cols, int pixelSize, void* data) : m_rows(rows), m_cols(cols), 
        m_pixelSize(pixelSize), m_size(rows*cols*pixelSize), m_data(static_cast<uint8_t*>(data)), m_refs(nullptr) {  }

    //Image(int rows, int cols, int pixelSize, void* data, RefCount* ref) : Image(rows, cols, pixelSize, data) { m_refs = ref; }

    Image(lycan::SizeI size, int pixelSize, void* data) : Image(size.cy, size.cx, pixelSize, data) { }
    Image(const Image& img) : Image(img.m_rows, img.m_cols, img.m_pixelSize, img.m_data) { m_refs = img.m_refs; if (m_refs) { m_refs->ref(); }}
    //Image(const Image& img, const lycan::RectI& roi);
    ~Image() {
        if (m_refs) {
            m_refs->release();
            // ���һ���ͷ���Դ�Ķ���
            if (m_refs->norefs() && m_data) {
                delete m_data;
                delete m_refs;
            }
        }
    }

    Image& operator=(const Image& img) { 
        m_rows = img.m_rows; m_cols = img.m_cols; m_pixelSize = img.m_pixelSize;
        m_data = img.m_data; m_size = img.m_size; 
        m_refs = img.m_refs;
        if (m_refs) { m_refs->ref(); }
    }
    int rows() const { return m_rows; }
    int cols() const { return m_cols; }
    size_t pixelSize() const { return m_pixelSize; }
    Image clone() const { 
        if (m_rows == 0 || m_cols == 0 || m_data == nullptr) { return Image(); }
        Image img;  
        img.m_rows = m_rows; img.m_cols = m_cols; 
        img.m_pixelSize = m_pixelSize;
        img.m_size = m_size;
        img.m_data = new uint8_t[m_size]; 
        memcpy(img.m_data, m_data, m_size);
        img.m_refs = new RefCount(1);
        return img;
    }

    void create(int rows, int cols, int pixelSize) {
        if (m_refs) { 
            m_refs->release(); 
            if (m_refs->norefs() && m_data) {
                delete m_data;
            }
            delete m_refs;
        }
        m_rows = rows; m_cols = cols; m_pixelSize = pixelSize; m_size = rows*cols*pixelSize;
        m_data = new uint8_t[m_size]; 
        m_refs = new RefCount(1);
    }
    void create(lycan::SizeI size, int pixelSize) { create(size.cy, size.cx, pixelSize); }
    void swap(Image& img) {  }
    // ref and release should appear in pair
    void ref() { if (m_refs) m_refs->ref(); }
    void release() {
        if (m_refs) {
            m_refs->release();
            if (m_refs->norefs() && m_data) {
                delete m_data;
                m_data = nullptr;
                m_size = 0;
                delete m_refs;
                m_refs = nullptr;
            }
        }
    }
    void* data() { return m_data; }

    template <typename T> const T& at(int row, int col) const {  return static_cast<T*>(m_data)[row*m_cols + col]; }
    template <typename T> T& at(int row, int col) { return static_cast<T*>(m_data)[row*m_cols + col]; }
    template <typename T> const T& at(lycan::PositionI pos) const { return at<T>(pos.y, pos.x); }
    template <typename T> T& at(lycan::PositionI pos) { return at<T>(pos.y, pos.x); }
    template <typename T> Vector<T> operator[](int x) { return Vector<T>(m_data, m_rows*m_cols); }

    template<typename T> Iterator<T> begin() { 
        if (sizeof(T) != m_pixelSize) { throw Exception(-1, "Bad cast for Image items: sizeof(T) != pixelSize."); }
        return Iterator<T>(m_rows, m_cols, m_data, 0); 
    }
    template<typename T> Iterator<T> begin() const { return begin<T>(); }
    template<typename T> Iterator<T> end() { 
        if (sizeof(T) != m_pixelSize) { throw Exception(-1, "Bad cast for Image items: sizeof(T) != pixelSize."); }
        return Iterator<T>(m_rows, m_cols, m_data, m_rows*m_cols); 
    }
    template<typename T> Iterator<T> end() const { return end<T>(); }
private:
    int         m_rows;
    int         m_cols;
    uint8_t*    m_data;
    size_t      m_pixelSize;
    size_t      m_size;
    RefCount*   m_refs;
};

}

#endif /* __LYCAN_GRAPHIC_H__ */
