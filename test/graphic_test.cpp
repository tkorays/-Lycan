#include <Lycan/Graphic.hpp>
#include <Lycan/Math.hpp>
#include <iostream>

using namespace std;
using namespace Lycan::Graphic2D;
using namespace Lycan::Math;

#if 1
#include "Lycan/SDLRender.hpp"
#include <time.h>
#endif

class A {
    public:
    int a;

    A(int b) : a(b) {}
};

int main() {
    int b = 6;
    A* pa = (A*)&b;
    ::new (pa) A(9);
    cout<<pa->a<<endl;

    unsigned char s[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    Matrix<char> m((char*)s, 4, 4);
#if 0
    cout<<(int)m.At(0, 0)<<endl;
    cout<<hex<<m.At<int>(0, 0)<<endl;

    Matrix<Vec4UB> img((char*)s, 4, 3);
    cout<<img.GetSize()<<endl;

    cout<<img.At<Vec1U>(0, 0).data[0]<<endl;

    Matrix<int> a = img.CastTo<int>();
    cout<<a<<endl;
    cout<<m<<endl;

    m *= 2;
    cout<<m<<endl;

    Matrix<char> n = m*2;
    cout<<(m*(int)3)<<endl;
    cout<<m*n<<endl;
#endif
    Matrix<int> m1 = Matrix<int>::Zeros(4, 3);
    Matrix<int> m2 = Matrix<int>::Zeros(3, 4);
    
    m1 << 1 << 2 << 3 << 4;
    m1 << 5 << 6 << 7 << 8;
    m1 << 9 << 10 << 11 << 12;

    m2 << 1 << 5 << 9;
    m2 << 2 << 6 << 10;
    m2 << 3 << 7 << 11;
    m2 << 4 << 8 << 12;

    cout<<m1*m2<<endl;

    Lycan::Ext::SDLRender::Init();
    Lycan::Ext::SDLRender render("image");

    Matrix<uint32_t> img = Matrix<uint32_t>::Zeros(480, 640);
    img = 0xffffffff;

    Lycan::Math::ROI<uint32_t> roi0(&img, 0, 0, 240, 320);
    roi0 = 0xffff00ff;

    roi0.Move(20, 20);
    roi0 = 0xff0000ff;

    roi0.Move(20, 20);
    roi0 = 0x00ff00ff;

    roi0.Move(20, 20);
    roi0 = 0x0000ffff;

    roi0.Move(20, 20);
    roi0 = 0x00ffff00;

    //img.DrawLine(640, 480, 10, 10, 0x000000FF);
    //mg.DrawRect(70, 80, 300, 500, 0xff0000ff);

    Point<int> p1(420, 630);
    p1.Draw(img, (uint32_t)0x00ff00ff);

    Line<int> line(640, 480, 10, 10);
    line.Draw(img, (uint32_t)0x000000FF);

    Rect<int> rect(70, 80, 500, 300);
    rect.Draw(img, (uint32_t)0xff0000ff);

    while(render.Wait()) {
        render.ShowImage(img.GetData(), img.GetCols(), img.GetRows(), img.GetCols()*4, Lycan::Ext::SDLRender::FMT_RGBA32);
        if(render.LoopEnd()) break;
    }

    return 0;
}