#include "../include/Lycan/Observer.hpp"
#include <iostream>
using namespace std;

class MyProxy : public Lycan::IObservableProxy {
public:
    void PublishTo(Lycan::IObservable* from, Lycan::IObserver* to) {
        cout<<"published"<<endl;
        to->Notified(from);
    }
    
    void UpdateA(int a) {
        Lycan::Observable<int>* ob = GetObservable<int>(a);
        if(ob) {
            ob->Update(1024);
        }
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
    Lycan::Observable<double> c;
    void RegisterObservers() {
        proxy->Register(3, &c);
    }

    void Notified(Lycan::IObservable* observable) {
        cout<<"notified, id:"<< observable->GetId() <<endl;
        if(observable->GetId() == 0) {
            Lycan::Observable<int> *p = dynamic_cast< Lycan::Observable<int>* >(observable);
            cout<<"a:"<<p->GetValue()<<endl;
        } else if(observable->GetId() == 1) {
            Lycan::Observable<string> *p = dynamic_cast< Lycan::Observable<string>* >(observable);
            cout<<"b:"<<p->GetValue()<<endl;
        }
    }
};

int main() {
    ServiceA sa;

    MyProxy proxy;
    sa.RegisterObservers(&proxy);

    WinB winb;
    winb.SetProxy(&proxy);
    winb.RegisterObservers();
    
    winb.Subscribe(0);
    winb.Subscribe(1);

    sa.a.Update(4);
    sa.b.Update("1234");

    proxy.UpdateA(5);
    return 0;
}
