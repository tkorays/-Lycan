#include <iostream>
#include <vector>
#include <memory>
#include "../include/Lycan/FlagSet.hpp"
using namespace std;

using Lycan::FlagSet;

enum class EA {
    a = 0,b,c,d,e
};

// 定义一个最多支持10个类型的标志集合，最大id为10
typedef FlagSet<EA, 10> EAFlags;

void Func1(EAFlags& abf) {
    for(auto i = 0; i < FlagSet<EA>::Count; i++) {
        if(abf.Check(static_cast<EA>(i))) {
            cout<<"support:"<<i<<endl;
        }
    }

}

int main() {
    FlagSet<EA> bf;
    bf  << EA::a
        << EA::c
        << EA::d
        << EA::e;
    bf.Clear(EA::d);


    //Func1(bf);
    Func1(EAFlags()
        <<EA::b
        <<EA::e
        );

    cout<<FlagSet<int, 1024>::Bytes<<endl;
    cout<<FlagSet<int, 8>::Bytes<<endl;
    return 0;
}
