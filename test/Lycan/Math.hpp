#pragma once
#include <stddef.h>
#include <type_traits>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cmath>

namespace Lycan {
namespace Math {
    template <typename T, int N> 
    class Vec {
    public: 
        typedef T ValueType;

        ValueType data[N];

        size_t SizeOf() const { return sizeof(data); }
        template <typename Q> Q ConvertTo() { return (Q)data; }
    };

    typedef Vec<unsigned int, 1> Vec1U;
    typedef Vec<int, 1> Vec1S;
    typedef Vec<unsigned char, 4> Vec4UB;
    typedef Vec<char, 4> Vec4B;

    template <typename T> 
    class Matrix {
    public: 
        typedef T ValueType;
        typedef Matrix<T> MatrixType;

        Matrix() : managed(false), data(0), rows(0), cols(0), size(0), index(0) {}
        Matrix(void* _data, int _rows, int _cols, bool _managed=false)
            : managed(_managed)
            , data((ValueType*)_data)
            , rows(_rows)
            , cols(_cols)
            , size(_cols*_rows*sizeof(ValueType))
            , index(0) { }
        Matrix(int _rows, int _cols) 
            : managed(true)
            , rows(_rows) 
            , cols(_cols)
            , size(_cols*_rows*sizeof(ValueType))
            , index(0) {
            data    = new ValueType[cols*rows];
        }
        Matrix(const Matrix& m) 
            : managed(true)
            , rows(m.GetRows())
            , cols(m.GetCols())
            , size(m.GetSize())
            , index(0) {
            data = new ValueType[cols*rows];
            memcpy(data, m.GetData(), m.GetSize());
        }
        Matrix& operator=(const Matrix& m) {
            if(managed && data && (rows != m.GetRows() || cols != m.GetCols())) {
                delete[] data;
            }
            managed = true;
            rows    = m.GetRows();
            cols    = m.GetCols();
            size    = m.GetSize();
            data    = new ValueType[cols*rows];
            index   = 0;
            memcpy(data, m.GetData(), size);
            return *this;
        }
        ~Matrix() {
            if(managed && data) {
                delete[] data;
            }
        }

        static Matrix Zeros(int _rows, int _cols) {
            ValueType* d = new ValueType[_cols*_rows];
            memset(d, 0, sizeof(ValueType)*_cols*_rows);
            return Matrix(d, _rows, _cols, true);
        }

        inline ValueType& At(int row, int col) const { return data[row*cols + col]; }
        template <typename Q> inline Q& At(int row, int col) const { return *(Q*)&data[row*cols + cols]; }
        inline ValueType& At(int idx) const { return data[idx]; }

        template <typename Q>
        Matrix<Q> CastTo() {
            return Matrix<Q>((void*)data, rows, cols*sizeof(ValueType)/sizeof(Q));
        }
        
        Matrix& operator<<(ValueType v) {
            At(index++) = v;
            if(index >= rows*cols) index = 0;
            return *this;
        }

        template <typename Q, class = typename std::enable_if<std::is_integral<Q>::value>::type >
        Matrix operator=(const Q& q) {
            for(int i = 0; i < cols*rows; i++) {
                At(i) = q;
            }
            return *this;
        }

        template <typename Q, class = typename std::enable_if<std::is_integral<Q>::value>::type >
        Matrix operator+(const Q& q) {
            Matrix m(*this);
            for(int i = 0; i < cols*rows; i++) {
                m.At(i) = At(i) + q;
            }
            return m;
        }

        template <typename Q, class = typename std::enable_if<std::is_integral<Q>::value>::type >
        Matrix& operator+=(const Q& q) {
            for(int i = 0; i < cols*rows; i++) {
                At(i) +=q;
            }
            return *this;
        }

        template <typename Q, class = typename std::enable_if<std::is_integral<Q>::value>::type >
        Matrix operator-(const Q& q) {
            Matrix m(*this);
            for(int i = 0; i < cols*rows; i++) {
                m.At(i) = At(i) + q;
            }
            return m;
        }

        template <typename Q, class = typename std::enable_if<std::is_integral<Q>::value>::type >
        Matrix& operator-=(const Q& q) {
            for(int i = 0; i < cols*rows; i++) {
                At(i) -=q;
            }
            return *this;
        }

        template <typename Q, class = typename std::enable_if<std::is_integral<Q>::value>::type >
        Matrix operator*(const Q& q) {
            Matrix m(*this);
            for(int i = 0; i < cols*rows; i++) {
                m.At(i) = At(i)*q;
            }
            return m;
        }

        template <typename Q, class = typename std::enable_if<std::is_integral<Q>::value>::type >
        Matrix& operator*=(const Q& q) {
            for(int i = 0; i < cols*rows; i++) {
                At(i) *=q;
            }
            return *this;
        }

        template <typename Q, class = typename std::enable_if<std::is_integral<Q>::value>::type >
        Matrix operator/(const Q& q) {
            Matrix m(*this);
            for(int i = 0; i < cols*rows; i++) {
                m.At(i) = At(i) + q;
            }
            return m;
        }

        template <typename Q, class = typename std::enable_if<std::is_integral<Q>::value>::type >
        Matrix& operator/=(const Q& q) {
            for(int i = 0; i < cols*rows; i++) {
                At(i) /=q;
            }
            return *this;
        }

        template <class M, class = typename M::MatrixType, class = typename M::MatrixType> 
        Matrix operator*(const M& m) {
            if(m.GetRows() != cols) {
                return Matrix();
            }
            Matrix out = Matrix::Zeros(rows, m.GetCols());
            for(int i = 0; i < out.GetRows(); i++) {
                for(int j = 0; j < out.GetCols(); j++) {
                    for(int k = 0; k < cols; k++) {
                        out.At(i, j) += At(i, k)*m.At(k, j);
                    }
                }
            }
            return out;
        }

        void Resize(int _rows, int _cols) {
            if(_rows == rows && _cols == cols) return ;
            if(data && managed) {
                delete[] data;
            }

            rows    = _rows;
            cols    = _cols;
            size    = _rows*_cols*sizeof(ValueType);
            data    = new ValueType[_rows*_cols];
            managed = true;
            index   = 0;
        }

        inline ValueType* Line(int line) { return (data + cols*line); }
        inline ValueType* GetData() const { return data; }
        inline int GetCols() const { return cols; }
        inline int GetRows() const { return rows; }
        inline size_t GetSize() const { return size; }
        inline void Manage(bool _managed) { managed = _managed; }
        inline bool IsNull() const { return !data || cols <= 0 || rows <= 0 || !size; }

        template <typename Q>
        friend std::ostream& operator<<(std::ostream& out, const Matrix<Q>& m);

    private:
        bool        managed;
        ValueType*  data;
        int         cols;
        int         rows;
        size_t      size;
        size_t      index;
    };

    template <typename Q>
    std::ostream& operator<<(std::ostream& o, const Matrix<Q>& m) {
        for(int i = 0; i < m.GetRows(); i++) {
            for(int j = 0; j < m.GetCols(); j++) {
                o << m.At(i, j) << ",";
            }
            o << std::endl;
        }
        return o;
    }

    std::ostream& operator<<(std::ostream& o, const Matrix<char>& m) {
        for(int i = 0; i < m.GetRows(); i++) {
            for(int j = 0; j < m.GetCols(); j++) {
                o << (int)m.At(i, j) << ",";
            }
            o << std::endl;
        }
        return o;
    }

    std::ostream& operator<<(std::ostream& o, const Matrix<unsigned char>& m) {
        for(int i = 0; i < m.GetRows(); i++) {
            for(int j = 0; j < m.GetCols(); j++) {
                o << (int)m.At(i, j) << ",";
            }
            o << std::endl;
        }
        return o;
    }

    template <typename T> 
    class ROI {
    public: 
        typedef T ValueType;

        ROI(Matrix<ValueType>* m, int _top, int _left, int _bottom, int _right)
            : matrix(m)
            , left(_left)
            , top(_top)
            , right(_right)
            , bottom(_bottom) {

        }

        template <typename Q, class = typename std::enable_if<std::is_integral<Q>::value>::type >
        ROI& operator+(const Q& q) {
            for(int i = top; i <= bottom; i++) {
                for(int j = left; j <= right; j++) {
                    matrix->At(i, j) += q;
                }
            }
            return *this;
        }

        template <typename Q, class = typename std::enable_if<std::is_integral<Q>::value>::type >
        ROI& operator+=(const Q& q) {
            for(int i = top; i <= bottom; i++) {
                for(int j = left; j <= right; j++) {
                    matrix->At(i, j) += q;
                }
            }
            return *this;
        }

        template <typename Q, class = typename std::enable_if<std::is_integral<Q>::value>::type >
        ROI& operator-(const Q& q) {
            for(int i = top; i <= bottom; i++) {
                for(int j = left; j <= right; j++) {
                    matrix->At(i, j) -= q;
                }
            }
            return *this;
        }

        template <typename Q, class = typename std::enable_if<std::is_integral<Q>::value>::type >
        ROI& operator-=(const Q& q) {
            for(int i = top; i <= bottom; i++) {
                for(int j = left; j <= right; j++) {
                    matrix->At(i, j) -= q;
                }
            }
            return *this;
        }

        template <typename Q, class = typename std::enable_if<std::is_integral<Q>::value>::type >
        ROI& operator*(const Q& q) {
            for(int i = top; i <= bottom; i++) {
                for(int j = left; j <= right; j++) {
                    matrix->At(i, j) *= q;
                }
            }
            return *this;
        }

        template <typename Q, class = typename std::enable_if<std::is_integral<Q>::value>::type >
        ROI& operator*=(const Q& q) {
            for(int i = top; i <= bottom; i++) {
                for(int j = left; j <= right; j++) {
                    matrix->At(i, j) *= q;
                }
            }
            return *this;
        }

        template <typename Q, class = typename std::enable_if<std::is_integral<Q>::value>::type >
        ROI& operator/(const Q& q) {
            for(int i = top; i <= bottom; i++) {
                for(int j = left; j <= right; j++) {
                    matrix->At(i, j) /= q;
                }
            }
            return *this;
        }

        template <typename Q, class = typename std::enable_if<std::is_integral<Q>::value>::type >
        ROI& operator/=(const Q& q) {
            for(int i = top; i <= bottom; i++) {
                for(int j = left; j <= right; j++) {
                    matrix->At(i, j) /= q;
                }
            }
            return *this;
        }

        template <typename Q, class = typename std::enable_if<std::is_integral<Q>::value>::type >
        ROI& operator=(const Q& q) {
            for(int i = top; i <= bottom; i++) {
                for(int j = left; j <= right; j++) {
                    matrix->At(i, j) = q;
                }
            }
            return *this;
        }

        void Move(int offsetY, int offsetX) {
            top += offsetY;
            bottom += offsetY;
            left += offsetX;
            right += offsetX;
            if(top < 0) top = 0;
            if(left < 0) left = 0;
            if(right >= matrix->GetCols()) right = matrix->GetCols() - 1;
            if(bottom >= matrix->GetRows()) bottom = matrix->GetRows() - 1;
        }

        void Reset(int _top, int _left, int _bottom, int _right) {
            top = _top;
            left = _left;
            bottom = _bottom;
            right = _right;
        }
    private:
        Matrix<T>*          matrix;
        int                 left; 
        int                 top;
        int                 right;
        int                 bottom;
    };
} // Math
} // Lycan
