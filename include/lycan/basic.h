#ifndef __LYCAN_BASIC_H__
#define __LYCAN_BASIC_H__

#include <cstdint>
#include <cstddef>
#include <algorithm>

namespace lycan {

template <typename T> inline T max(T&& a, T&& b) { return a > b ? a : b; }
template <typename T> inline T min(T&& a, T&& b) { return a < b ? a : b; }

template <typename T>
class PositionT {
public:
    typedef T ValueType;

    T x;
    T y;

    PositionT() : x(0), y(0) { }
    PositionT(T _x, T _y) : x(_x), y(_y) { }
    PositionT operator+(const PositionT& p) { return PositionT<T>(x + p.x, y + p.y); }
    PositionT operator-(const PositionT& p) { return PositionT<T>(x - p.x, y - p.y); }
    PositionT& operator+=(const PositionT& p) { x += p.x; y += p.y; return *this; }
    PositionT& operator-=(const PositionT& p) { x -= p.x; y -= p.y; return *this; }
    template <typename Q> PositionT operator+(Q q) { return PositionT<T>(x + q, y + q); }
    template <typename Q> PositionT operator-(Q q) { return PositionT<T>(x - q, y - q); }
    template <typename Q> PositionT operator*(Q q) { return PositionT<T>(x * q, y * q); }
    template <typename Q> PositionT operator/(Q q) { return PositionT<T>(x / q, y / q); }
    template <typename Q> PositionT& operator+=(Q q) { x += q; y += q; return *this; }
    template <typename Q> PositionT& operator-=(Q q) { x -= q; y -= q; return *this; }
    template <typename Q> PositionT& operator*=(Q q) { x *= q; y *= q; return *this; }
    template <typename Q> PositionT& operator/=(Q q) { x /= q; y /= q; return *this; }
    bool operator==(const PositionT& p) { return p.x == x && p.y == y; }
    bool operator!=(const PositionT& p) { return p.x != x || p.y != y; }
};

typedef PositionT<int8_t> Position8;
typedef PositionT<uint8_t> PositionU8;
typedef PositionT<int16_t> Position16;
typedef PositionT<uint16_t> PositionU16;
typedef PositionT<int32_t> Position32;
typedef PositionT<uint32_t> PositionU32;
typedef PositionT<float> PositionF;
typedef PositionT<double> PositionD;


template <typename T>
class SizeT {
public:
    typedef T ValueType;

    T cx;
    T cy;

    SizeT() : cx(0), cy(0) { }
    SizeT(T _cx, T _cy) : cx(_cx), cy(_cy) { }
    SizeT(const SizeT& sz) : cx(sz.cx), cy(sz.cy) {}
    template <typename Q> SizeT operator+(Q q) { return SizeT<T>(cx + q, cy + q); }
    template <typename Q> SizeT operator-(Q q) { return SizeT<T>(cx - q, cy - q); }
    template <typename Q> SizeT operator*(Q q) { return SizeT<T>(cx * q, cy * q); }
    template <typename Q> SizeT operator/(Q q) { return SizeT<T>(cx / q, cy / q); }
    template <typename Q> SizeT& operator+=(Q q) { cx += q; cy += q; return *this; }
    template <typename Q> SizeT& operator-=(Q q) { cx -= q; cy -= q; return *this; }
    template <typename Q> SizeT& operator*=(Q q) { cx *= q; cy *= q; return *this; }
    template <typename Q> SizeT& operator/=(Q q) { cx /= q; cy /= q; return *this; }
    inline bool operator==(const SizeT& sz) { return sz.cx == cx && sz.cy == cy; }
    inline bool operator!=(const SizeT& sz) { return sz.cx != cx || sz.cy != cy; }
};

typedef SizeT<int8_t> Size8;
typedef SizeT<uint8_t> SizeU8;
typedef SizeT<int16_t> Size16;
typedef SizeT<uint16_t> SizeU16;
typedef SizeT<int32_t> Size32;
typedef SizeT<uint32_t> SizeU32;
typedef SizeT<float> SizeF;
typedef SizeT<double> SizeD;

template <typename T>
class RectT {
public:
    typedef T ValueType;

    T left;
    T top;
    T right;
    T bottom;

    RectT() : left(0), top(0), right(0), bottom(0) {}
    RectT(int l, int t, int r, int b) : left(l), top(t), right(r), bottom(b) { }
    T width() const { return (right - left + static_cast<T>(1)); }
    T height() const { return (bottom - top + static_cast<T>(1)); }

    PositionT<T> leftTop() const { return PositionT<T>(left, top); }
    PositionT<T> rightBottom() const { return PositionT<T>(right, bottom); }
    SizeT<T> rectSize() const { 
        return SizeT<T>(right - left + static_cast<T>(1), bottom - top + static_cast<T>(1)); 
    }

    inline void move(SizeT<T> offset) { move(offset.x, offset.y); }
    inline void move(T xShift, T yShift) { left += xShift; right += xShift; top += yShift; bottom += yShift; }
    inline void moveTo(const PositionT<T>& p) { move(p.x - left, p.y - top); }
    inline bool ptInRect(T x, T y) const { return x >= left && x <= right && y >= top && y <= bottom; }
    inline bool ptInRect(const PositionT<T>& p) const { return ptInRect(p.x, p.y); }
    inline bool containsRect(const RectT<T>& rc) { return rc.top >= top && rc.bottom <= bottom && rc.left >= left && rc.right <= right; }
};

typedef RectT<int8_t> Rect8;
typedef RectT<uint8_t> RectU8;
typedef RectT<int16_t> Rect16;
typedef RectT<uint16_t> RectU16;
typedef RectT<int32_t> Rect32;
typedef RectT<uint32_t> RectU32;
typedef RectT<float> RectF;
typedef RectT<double> RectD;

template <typename T>
class ReferenceT {
public:
    typedef T ValueType;

    ReferenceT() : count(0), data(nullptr) { }
    ReferenceT(ValueType* p) : count(1), data(p) {  }
    ~ReferenceT() { if(data) { delete data; } }
    int addRef() { if (count >= 0) ++count; return count; }
    int release() { if (count > 0) --count; return count; }
    bool norefs() { return count == 0; }

    int             count;
    ValueType*      data;
};

template <typename T>
class VectorT {
public:
    typedef T ValueType;

    class Iterator {
    public:
        Iterator(ValueType* data, size_t count, int index)
            : m_data(data), m_count(count), m_index(index) {  }
        Iterator(const Iterator& it) 
            : m_data(it.m_data), m_count(it.m_count), m_index(it.m_index)  { }
        ValueType& operator *() { return m_data[m_index]; }
        ValueType& operator [](ptrdiff_t i) { return m_data[i]; }

        Iterator operator++() { m_index = min<int>(m_index+1, m_count); return *this; }
        Iterator operator--() { m_index = max<int>(m_index-1, 0); return *this; }
        Iterator operator++(int) { Iterator it(*this); ++*this; return it; }
        Iterator operator--(int) { Iterator it(*this); --*this; return it; }
        Iterator& operator+=(ptrdiff_t i) { m_index = min(m_index + i, m_count); return *this; }
        Iterator& operator-=(ptrdiff_t i) { m_index = max(m_index - i, 0); return *this; }
        bool operator==(const Iterator& it) { 
            return m_data == it.m_data && m_count == it.m_count && m_index == it.m_index; 
        }
        bool operator!=(const Iterator& it) {
            return m_data != it.m_data || m_count != it.m_count || m_index != it.m_index; 
        }
    private:
        ValueType*  m_data;
        size_t      m_count;
        int         m_index;
    };

    VectorT(size_t count) : m_count(count), m_managed(true), m_dataRef(nullptr) { 
        m_data = new ValueType[count];
    }
    VectorT(ValueType* p, size_t count, bool copy) 
        : m_count(count), m_managed(copy), m_dataRef(nullptr) {
        if (copy) {
            m_data = new ValueType[count];
            memcpy(m_data, p, sizeof(ValueType)*m_count);
        } else {
            m_data = p;
        }
    }
    VectorT(ValueType* p, size_t size) : VectorT(p, size, true) {  }
    VectorT(VectorT<ValueType>& v) 
        : m_data(v.m_data), m_count(v.m_count), m_managed(false), m_dataRef(v.m_dataRef) { 
        if(v.m_dataRef == nullptr) {
            v.m_dataRef = new ReferenceT<ValueType>(m_data);
        }
        m_dataRef = v.m_dataRef;
        m_dataRef->addRef();
    }
    VectorT(const VectorT<ValueType>& v) 
        : m_data(v.m_data), m_count(v.m_count), m_managed(false), m_dataRef(v.m_dataRef) { 
        if(m_dataRef) m_dataRef->addRef();
    }
    ~VectorT() { 
        // data is managed by RefCounter
        if(m_dataRef) {
            m_dataRef->release();
            if (m_dataRef->norefs()) delete m_dataRef; 
        } 
        // data is managed by self
        else if(m_managed) {
            if(m_data) delete m_data;
        }
    }

    VectorT& operator=(VectorT<ValueType>& v) {
        m_data = v.m_data; 
        m_count = v.m_count; 
        m_managed = false;
        if(v.m_dataRef == nullptr) {
            v.m_dataRef = new ReferenceT<ValueType>(m_data);
        }
        m_dataRef = v.m_dataRef;
        m_dataRef->addRef();
    }

    VectorT& operator=(const VectorT<ValueType>& v) {
        m_data = v.m_data; 
        m_count = v.m_count; 
        m_managed = false;
        m_dataRef = v.m_dataRef;
        if(m_dataRef) m_dataRef->addRef();
    }

    VectorT clone() const { return VectorT(m_data, m_count, true); }
    size_t count() const { return m_count; }

    const ValueType& at(int i) const { return m_data[i]; }
    ValueType& at(int i) { return m_data[i]; }
    ValueType& operator[](int i) { return m_data[i]; }
    const ValueType& operator[](int i) const { return m_data[i]; }

    Iterator begin() { return Iterator(m_data, m_count, 0); }
    Iterator begin() const { return begin(); }
    Iterator end() { return Iterator(m_data, m_count, m_count); }
    Iterator end() const { return end(); }
private:
    size_t                  m_count;
    ValueType*              m_data;
    bool                    m_managed;
    ReferenceT<ValueType>*  m_dataRef; 
};

template <typename T>
class MatrixT {
public:
    typedef T ValueType;
    
    class Iterator {
    public:
        Iterator(ValueType* data, int rows, int cols, int index) 
            : m_data(data), m_rows(rows), m_cols(cols), m_index(index) {  }
        Iterator(const Iterator& it) : Iterator(it.m_data, it.m_rows, it.m_cols, it.m_index) { }

        ValueType& operator *() { return m_data[m_index]; }
        ValueType& operator [](ptrdiff_t i) { return m_data[i]; }

        Iterator operator++() { m_index = min<int>(m_index+1, m_rows*m_cols); return *this; }
        Iterator operator--() { m_index = max<int>(m_index-1, 0); return *this; }
        Iterator operator++(int) { Iterator it(*this); ++*this; return it; }
        Iterator operator--(int) { Iterator it(*this); --*this; return it; }
        Iterator& operator+=(ptrdiff_t i) { m_index = min(m_index + i, m_rows*m_cols); return *this; }
        Iterator& operator-=(ptrdiff_t i) { m_index = max(m_index - i, 0); return *this; }
        bool operator==(const Iterator& it) { 
            return m_data == it.m_data && m_rows*m_cols == (it.m_rows*it.m_cols) && m_index == it.m_index; 
        }
        bool operator!=(const Iterator& it) {
            return m_data != it.m_data || m_rows*m_cols != (it.m_rows*it.m_cols) || m_index != it.m_index; 
        }
    private:
        ValueType*  m_data;
        int         m_rows;
        int         m_cols;
        int         m_index;
    };

    MatrixT() : m_data(0), m_rows(0), m_cols(0), m_managed(false), m_dataRef(nullptr) { }
    MatrixT(int rows, int cols) : m_rows(rows), m_cols(cols), m_dataRef(nullptr) {
        m_data = new ValueType[sizeof(ValueType)*m_rows*m_cols];
        m_managed = true;
    }
    MatrixT(ValueType* data, int rows, int cols, bool copy) 
        : m_rows(rows), m_cols(cols), m_managed(copy), m_dataRef(nullptr) {
        if(copy) {
            m_data = new ValueType[m_rows*m_cols];
            memcpy(m_data, data, sizeof(ValueType)*m_rows*m_cols);
        } else {
            m_data = data;
        }
    }
    MatrixT(ValueType* data, int rows, int cols) : MatrixT(data, rows, cols, true) {  }
    ~MatrixT() { 
        if(m_dataRef) {
            m_dataRef->release();
            if (m_dataRef->norefs()) delete m_dataRef; 
        } 
        else if(m_managed) {
            if(m_data) delete m_data;
        }
    }
    MatrixT(MatrixT& m) : MatrixT(m.m_data, m.m_rows, m.m_cols, false), m_dataRef(m.m_dataRef) { 
        if(m_dataRef == nullptr) { 
            m.m_dataRef = new ReferenceT<ValueType>(m_data);
        }
        m_dataRef = m.m_dataRef;
        if(m_dataRef) m_dataRef->addRef();
    }

    MatrixT(const MatrixT& m) : MatrixT(m.m_data, m.m_rows, m.m_cols, false), m_dataRef(m.m_dataRef) { 
        if(m_dataRef) m_dataRef->addRef();
    }

    MatrixT& operator=(MatrixT& m) {
        return MatrixT(*this); 
    }
    MatrixT& operator=(const MatrixT& m) { 
        return MatrixT((const MatrixT&)*this); 
    }

    VectorT<T> operator[](int index) { return VectorT<T>(m_data + m_cols*index, m_cols, false); }

    ValueType at(int x, int y) { return m_data[y*m_cols + x]; }
    template <typename Q> Q at(int x, int y) { return *(Q*)&m_data[y*m_cols + x]; }
    VectorT<T> row(int line) { return VectorT<T>(m_data + m_cols*line, m_cols, false); }
    size_t sizeOfData() const { return m_rows * m_cols * sizeof(ValueType); }

    bool create(int cols, int rows) { 
        if (m_managed && m_data && m_cols == cols && m_rows == rows) return true; 
        if (m_managed && m_data) { 
            if(m_dataRef) {
                if(m_dataRef->norefs()) { delete m_dataRef; }
                else { m_dataRef->release(); }
            } else {
                delete m_data; 
            }
            m_dataRef = nullptr;
        }
        m_rows = rows;
        m_cols = cols;
        m_data = new ValueType[cols*rows];
        m_managed = true;
    }
    MatrixT clone() { return MatrixT(m_data, m_rows, m_cols, true); }
    void release() { 
        m_rows = 0; 
        m_cols = 0; 
        if (m_managed && m_data) { 
            if(m_dataRef) {
                if(m_dataRef->norefs()) { delete m_dataRef; }
                else { m_dataRef->release(); }
            } else {
                delete m_data;
            }
        } 
        m_data = nullptr;
        m_managed = false; 
        m_dataRef = nullptr;
    }
    ValueType* data() { return m_data; }
    int rows() const { return m_rows; }
    int cols() const { return m_cols; }

    Iterator begin() { return Iterator(m_data, m_rows, m_cols, 0); }
    Iterator begin() const { return Iterator(m_data, m_rows, m_cols, 0); }
    Iterator end() { return Iterator(m_data, m_rows, m_cols, m_rows*m_cols); }
    Iterator end() const { return Iterator(m_data, m_rows, m_cols, m_rows*m_cols); }

private:
    ValueType*              m_data;
    int                     m_rows;
    int                     m_cols;
    bool                    m_managed;
    ReferenceT<ValueType>*  m_dataRef;
};

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

} 

#endif /* __LYCAN_BASIC_H__ */
