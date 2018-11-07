#include "../include/Lycan/Observer.hpp"
#include <iostream>
using namespace std;

class MyProxy : public Lycan::IObservableProxy {
public:
    void Publish(Lycan::IObservable* from) {
        cout<<"publish"<<endl;
    }
};

class ServiceA {
public:
    Lycan::Observable<int> a;
    Lycan::Observable<string> b;

    void RegisterObservers(Lycan::IObservableProxy* proxy) {
        proxy->Register(0, &a);
        proxy->Register(1, &b);
    }
};

class WinB : public Lycan::IObserver {
public:
    void Notified(Lycan::IObservable* observable) {
        cout<<"notified"<<endl;
    }
};

int main() {
    ServiceA sa;

    MyProxy proxy;
    sa.RegisterObservers(&proxy);

    WinB winb;
    winb.SetProxy(&proxy);
    winb.Subscribe(0);
    winb.Subscribe(1);

    sa.a.Update(4);
    sa.b.Update("1234");

    return 0;
}
