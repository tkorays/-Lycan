#pragma once
#include <stddef.h>
#include <stdint.h>

namespace Lycan { 
namespace Graphic2D {

    class Shape { 
    public:

    }; 

    template <typename T> 
    class Point : public Shape {
    public:
        typedef T ValueType;

        T x;
        T y;

        Point() : x(0), y(0) { }
        Point(T _x, T _y) : x(_x), y(_y) { }
        Point operator+(const Point& p) { return Point<T>(x + p.x, y + p.y); }
        Point operator-(const Point& p) { return Point<T>(x - p.x, y - p.y); }
        template <typename Q> Point operator+(Q q) { return Point<T>(x + q, y + q); }
        template <typename Q> Point operator-(Q q) { return Point<T>(x - q, y - q); }
        template <typename Q> Point operator*(Q q) { return Point<T>(x * q, y * q); }
        template <typename Q> Point operator/(Q q) { return Point<T>(x / q, y / q); }
        inline bool operator==(const Point& p) { return p.x == x && p.y == y; }
    };

    typedef Point<int8_t> PointI8;
    typedef Point<uint8_t> PointU8;
    typedef Point<int16_t> PointI16;
    typedef Point<uint16_t> PointU16;
    typedef Point<int32_t> PointI32;
    typedef Point<uint32_t> PointU32;
    typedef Point<float> PointF;
    typedef Point<double> PointD;


    template <typename T> 
    class Size {
    public:
        typedef T ValueType;

        T cx;
        T cy;

        Size() : cx(0), cy(0) { }
        Size(T _cx, T _cy) : cx(_cx), cy(_cy) { }
        Size(const Size& sz) : cx(sz.cx), cy(sz.cy) {}
        template <typename Q> Size operator+(Q q) { return Size<T>(cx + q, cy + q); }
        template <typename Q> Size operator-(Q q) { return Size<T>(cx - q, cy - q); }
        template <typename Q> Size operator*(Q q) { return Size<T>(cx * q, cy * q); }
        template <typename Q> Size operator/(Q q) { return Size<T>(cx / q, cy / q); }
        inline bool operator==(const Size& sz) { return sz.cx == cx && sz.cy == cy; }
    };

    template <typename T> 
    class Offset : public Point<T> {
    public: 
        typedef T ValueType;
    };

    template <typename T>
    class Rect : public Shape {
    public:
        typedef T ValueType;

        T left;
        T top;
        T right;
        T bottom;

        inline T Width() const { return right - left; }
        inline T Height() const { return bottom - top; }

        Point<T> LT() const { return Point<T>(left, top); }
        Point<T> RB() const { return Point<T>(right, bottom); }
        Size<T> RectSize() const { return Size<T>(right - left, bottom - top); }

        inline void Move(Offset<T> offset) { Move(offset.x, offset.y); } 
        inline void Move(T xShift, T yShift) { left += xShift; right += xShift; top += yShift; bottom += yShift; }
        inline void MoveTo(const Point<T>& p) { Move(p.x - left, p.y - top); }
        inline bool PtInRect(T x, T y) const { return x >= left && x <= right && y >= top && y <= bottom; }
        inline bool PtInRect(const Point<T>& p) const { return PtInRect(p.x, p.y); }
        inline bool ContainsRect(const Rect<T>& rc) { return rc.top >= top && rc.bottom <= bottom && rc.left >= left && rc.right <= right; }
    };

    template <typename T>
    class Line : public Shape {
    public:
        Point<T> p1;
        Point<T> p2;

        bool IsSameLine(const Line<T>& line) {
            return (p1 == line.p1 && p2 == line.p2)
                || (p1 == line.p2 && p2 == line.p1);
        }
    };

    template <typename T> 
    class Circle : public Shape {
    public: 
        Point<T> center;
        T radius;
    };
}
}