
#include <iostream>
using namespace std;
#include "../include/lycan/basic.h"
#include "../include/lycan/memory.h"

int main() {
    lycan::VectorT<int> vec1(5);
    lycan::VectorT<int> vec2(vec1);
    lycan::VectorT<char> vec3(6);
    vec3.clone();
    for(auto it = vec1.begin(); it != vec1.end(); it++) {
        *it = 1234;
    }
    cout<<vec1.at(1)<<endl;

    lycan::MatrixT<int> mat(2,2);
    for(auto it = mat.begin(); it != mat.end(); it++) {
        *it = 3456;
    }
    cout<<mat.at(1,1)<<endl;

    lycan::VectorT<int> v = mat[1];
    mat[1][0] = 2;
    cout<<"::"<<mat[1][0]<<endl;
    v[0] = 7890;
    v[1] = 7654;
    cout<<mat.at(1,0)<<","<<mat.at(1,1)<<endl;
    for(auto it = mat.begin(); it != mat.end(); it++) {
        cout<<*it<<endl;
    }

    typedef lycan::Allocator<int, 8> Alloc;
    int* p = Alloc::allocate(7);
    cout<<p<<endl;
    Alloc::deallocate(p);
    cout<<Alloc::align() <<endl;
    return 0;
}