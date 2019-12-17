#pragma once
#include "Math.hpp"

namespace Lycan { 

namespace Graphic2D {


    class Shape { 
    public:
        template <typename MatrixValueType> 
        int Draw(Lycan::Math::Matrix<MatrixValueType>& m, MatrixValueType ptValue) { return 0; }

        template <typename MatrixValueType> 
        int Draw(Lycan::Math::ROI<MatrixValueType>&m , MatrixValueType ptValue) { return 0; }
    }; 

    template <typename T> 
    class Point : public Shape {
    public:
        typedef T ValueType;

        T x;
        T y;

        inline Point() : x(0), y(0) { }
        inline Point(T _x, T _y) : x(_x), y(_y) { }
        Point& operator+(const Point& p) { x += p.x; y += p.y; return *this; }
        Point& operator-(const Point& p) { x -= p.x; y -= p.y; return *this; }
        template <typename Q> Point& operator+(Q q) { x += q; y += q; return *this; }
        template <typename Q> Point& operator-(Q q) { x -= q; y -= q; return *this; }
        template <typename Q> Point& operator*(Q q) { x *= q; y *= q; return *this; }
        template <typename Q> Point& operator/(Q q) { x /= q; y /= q; return *this; }
        inline bool operator==(const Point& p) { return p.x == x && p.y == y; }

        template <typename MatrixValueType> 
        int Draw(Lycan::Math::Matrix<MatrixValueType>& m, MatrixValueType ptValue) {
            m.At(y, x) = ptValue;
            return 0;
        }
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

        inline Rect(int _left, int _top, int _right, int _bottom)
            : left(_left)
            , top(_top)
            , right(_right)
            , bottom(_bottom) { }

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

        template <typename MatrixValueType> int Draw(Lycan::Math::Matrix<MatrixValueType>& m, MatrixValueType v);
    };

    template <typename T>
    class Line : public Shape {
    public:
        Point<T> p1;
        Point<T> p2;

        inline Line(int x0, int y0, int x1, int y1) {
            p1.x = (x0); p1.y = (y0);
            p2.x = (x1); p2.y = (y1);  
        }

        inline bool IsSameLine(const Line<T>& line) {
            return (p1 == line.p1 && p2 == line.p2)
                || (p1 == line.p2 && p2 == line.p1);
        }

        template <typename MatrixValueType> int Draw(Lycan::Math::Matrix<MatrixValueType>& m, MatrixValueType v);
    };

    template <typename T> 
    class Circle : public Shape {
    public: 
        Point<T> center;
        T radius;
    };

    /*
    template <typename ValueType>
    void DrawLineByWXL(int x0, int y0, int x1, int y1, ValueType v) {
        #define IPART(x) floor(x)
        #define ROUND(x) IPART(x+0.5)
        #define FPART(x) x - floor(x)
        #define RFPART(x) 1 - FPART(x)

        At(y0, x0) = v;
        if(x0 == x1 && y0 == y1) return;

        bool steep = abs(y1 - y0) > abs(x1 - x0);
        if(steep) {
            std::swap(x0, y0);
            std::swap(x1, y1);
        } 
        if(x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }

        int dx = (x1 - x0);
        int dy = (y1 - y0);
        float gradient;
        if(dx == 0) {
            gradient = 1.0;
        } else {
            gradient = dy / (float)dx;
        }

        float xend = ROUND(x0);
        float yend = y0 + gradient*(xend - x0);
        float xgap = RFPART(x0 + 0.5);
        int xpxl1 = xend;
        int ypxl1 = IPART(yend);
        if(steep) {
            At(xpxl1, ypxl1) = v*RFPART(yend)*xgap;
            At(xpxl1, ypxl1+1) = v*FPART(yend)*xgap;
        } else {
            At(ypxl1, xpxl1) = v*RFPART(yend)*xgap;
            At(ypxl1, xpxl1+1) = v*FPART(yend)*xgap;
        }

        float intery = yend + gradient;

        xend = ROUND(x1);
        yend = y1 + gradient*(xend - x1);
        xgap = FPART(x1 + 0.5);
        int xpxl2 = xend;
        int ypxl2 = IPART(yend);
        if(steep) {
            At(xpxl2, ypxl2) = v*RFPART(yend)*xgap;
            At(xpxl2, ypxl2+1) = v*FPART(yend)*xgap;
        } else {
            At(ypxl2, xpxl2) = v*RFPART(yend)*xgap;
            At(ypxl2, xpxl2+1) = v*FPART(yend)*xgap;
        }

        uint32_t mask = 0;
        if(steep) {
            for(int x = xpxl1 + 1; x <= xpxl2 - 1; x++) {
                At(x, IPART(intery)) = v*RFPART(intery);
                At(x, IPART(intery)+1) = v*FPART(intery);
                intery = intery + gradient;
            }
        } else {
            for(int x = xpxl1 + 1; x <= xpxl2 - 1; x++) {
                At(IPART(intery), x) = v * RFPART(intery);
                At(IPART(intery)+1, x) = v*FPART(intery);
                intery = intery + gradient;
            }
        }
    }
    */

    template <typename T>
    template <typename MatrixValueType>
    int Line<T>::Draw(Lycan::Math::Matrix<MatrixValueType>& m, MatrixValueType v) {
        int x0 = p1.x;
        int y0 = p1.y;
        int x1 = p2.x;
        int y1 = p2.y;

        m.At(y0, x0) = v;
        if(x0 == x1 && y0 == y1) return 0;
        int dx = abs(x1 - x0);
        int dy = abs(y1 - y0);

        int flag = 0;
        if(dx < dy) {
            std::swap(x0, y0);
            std::swap(x1, y1);
            std::swap(dx, dy);
            flag = 1;
        }

        int x = x0, y = y0;
        int tx = x1 > x0 ? 1 : -1;
        int ty = y1 > y0 ? 1 : -1;

        int e = -dx;
        while(x != x1) {
            x += tx;
            e += 2*dy;
            if(e > dx) {
                e -= 2*dx;
                y += ty;
            } 
            if(flag) m.At(x, y) = v;
            else m.At(y, x) = v;
        }
        return 0;
    }

    template <typename T>
    template <typename MatrixValueType> int Rect<T>::Draw(Lycan::Math::Matrix<MatrixValueType>& m, MatrixValueType v) {
        Line<T> l1(left, top, right, top);
        Line<T> l2(left, top, left, bottom);
        Line<T> l3(right, top, right, bottom);
        Line<T> l4(left, bottom, right, bottom);
        l1.Draw(m, v);
        l2.Draw(m, v);
        l3.Draw(m, v);
        l4.Draw(m, v);
        return 0;
    }
}
}