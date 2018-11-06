#include "../include/Lycan/AOP.hpp"
#include "../include/Lycan/CreatePolicy.hpp"
#include "../include/Lycan/ScopeWrapper.hpp"
#include "../include/Lycan/ScopeLocker.hpp"
#include "../include/Lycan/Singleton.hpp"
#include <iostream>
using namespace std;

class MyAOP {
public:
    MyAOP(const char* s) {
        cout<<s<<" enter!"<<endl;
        f = s;
    }
    ~MyAOP() {
        cout<<f<<" leave!"<<endl;
    }
private:
    const char* f;
};

class MySimpleLockerSingleton : 
    public Lycan::ILocker,
    public Lycan::EnableAOP<MyAOP> {
        __LYCAN_SINGLETON__(
            MySimpleLockerSingleton,
            Lycan::CreateByNewPolicy<MySimpleLockerSingleton>
        )
    public: 
        virtual void Lock() { 
            __LYCAN_AOP__
        };
        virtual void Unlock() {
            __LYCAN_AOP__
        };
};

MySimpleLockerSingleton* lck = 0;

void SetupInstance() {
    lck = MySimpleLockerSingleton::GetInstance();
}

void func() {
    Lycan::ScopeWrapper<MySimpleLockerSingleton> p1(lck);
}

int main() {
    SetupInstance();
    func();
    return 0;
}
