#pragma once
#include <stddef.h>
#include <type_traits>

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
        Matrix(char* _data, size_t _width, size_t _height)
            : data((ValueType*)_data), width(_width), height(_height) { }

        inline ValueType At(int x, int y) { return data[y*width + x]; }

        template <typename Q> 
        inline Q At(int x, int y) { 
            return *(Q*)&data[y*width + x]; 
        }
        inline ValueType* Line(int line) { return (data + width*line); }
        inline size_t Size() { return width*height*sizeof(ValueType); }

    private:
        ValueType* data;
        size_t width;
        size_t height;
        size_t size;
    };
} // Math
} // Lycan
