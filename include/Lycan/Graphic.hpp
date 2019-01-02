#pragma once

namespace Lycan {
    template <typename T> 
    class Point2D {
    public:
        T x;
        T y;

        Point2D() : x(0), y(0) { }
        Point2D(T _x, T _y) : x(_x), y(_y) { }
        Point2D& operator+(const Point2D& p) {
            x += p.x;
            y += p.y;
            return *this;
        }

        Point2D& operator-(const Point2D& p) {
            x -= p.x;
            y -= p.y;
            return *this;
        }

        template <typename Q>
        Point2D& operator*(Q q) {
            x *= q;
            y *= q;
        }

        template <typename Q>
        Point2D& operator/(Q q) {
            x /= q;
            y /= q;
        }

        inline bool operator==(const Point2D& p) {
            return p.x == x && p.y == y;
        }
    };


    template <typename T>
    class Rect {
    public:
        T top;
        T bottom;
        T left;
        T right;

        inline T Width() const { return right - left; }
        inline T Height() const { return bottom - top; }

        Point2D<T> TopLeft() const { return Point2D<T>(left, top); }
        Point2D<T> RightBottom() const { return Point2D<T>(right, bottom); }

        inline void Move(T xShift, T yShift) const {
            left += xShift;
            right += xShift;
            top += yShift;
            bottom += yShift;
        }

        void MoveTo(const Point2D<T>& p) {
            Move(p.x - left, p.y - top);
        }

        inline bool PtInRect(T x, T y) const {
            return x >= left && x <= right && y >= top && y <= bottom;
        }

        bool PtInRect(const Point2D<T>& p) const {
            return PtInRect(p.x, p.y);
        }

        inline bool ContainsRect(const Rect<T>& rc) {
            return rc.top >= top && rc.bottom <= bottom && rc.left >= left && rc.right <= right;
        }
    };

    template <typename T>
    class Line {
    public:
        Point2D<T> p1;
        Point2D<T> p2;

        bool IsSameLine(const Line<T>& line) {
            return (p1 == line.p1 && p2 == line.p2)
                || (p1 == line.p2 && p2 == line.p1);
        }
    };
}