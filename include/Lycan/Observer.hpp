#pragma once
#include <vector>
#include <algorithm>
#include <map>
#include <set>

namespace Lycan {
    typedef int ObservableId;;
    class IObservable;
    class IObserver;
    class IObservableProxy;
    

    class IObserver {
    public:
        virtual void SetProxy(IObservableProxy* p) { proxy = p; }
        virtual void Notified(IObservable* observable) = 0;
        virtual void Subscribe(ObservableId id);
        virtual void Unsubscribe(ObservableId id);
    protected: 
        IObservableProxy* proxy;
    };

    class IObservableProxy {
    public: 
        virtual void Publish(IObservable* from) = 0;
        virtual void Subscribe(IObserver* observer, ObservableId id);
        virtual void Unsubscribe(IObserver* observer, ObservableId id);
        virtual void Register(ObservableId id, IObservable* observable);
        virtual void Unregister(ObservableId id, IObservable* observable);
    protected: 
        std::map< 
            ObservableId,
            std::pair<
                IObservable*,
                std::set<IObserver*>
            >
        > observers;
    };

    class IObservable {
    public:
        virtual void SetProxy(IObservableProxy* p) { proxy = p; }
    protected:
        virtual void Publish();
        IObservableProxy* proxy;
    };

    template <typename T>
    class Observable : public IObservable {
    public:
        Observable() : is_first_assign(true) { }
        void Update(T v) {
            
            if(v != _observed_value || is_first_assign) {
                _observed_value = v;
                Publish();
            }
        }
    protected:
        T _observed_value;
        bool is_first_assign;
    };


    void IObserver::Subscribe(ObservableId id) {
        if(proxy) {
            proxy->Subscribe(this, id);
        }
    }

    void IObserver::Unsubscribe(ObservableId id) {
        if(proxy) {
            proxy->Unsubscribe(this, id);
        }
    }

    void IObservableProxy::Subscribe(IObserver* observer, ObservableId id) {
        if(!observer) return;
        if(observers.find(id) != observers.end()) {
            observers[id].second.insert(observer);
        }
    }
    void IObservableProxy::Unsubscribe(IObserver* observer, ObservableId id) {
        if(!observer) return;
        if(observers.find(id) != observers.end()) {
            observers[id].second.erase(observer);
        }
    }

    void IObservableProxy::Register(ObservableId id, IObservable* observable) {
        if(!observable) return;
        auto it = observers.find(id);
        if(it == observers.end()) {
            observers[id].first = observable;
            observers[id].second = std::set<IObserver*>();
            observable->SetProxy(this);
        }
    }

    void IObservableProxy::Unregister(ObservableId id, IObservable* observable) {
        if(!observable) return;
        auto it = observers.find(id);
        if(it != observers.end()) {
            observers.erase(it);
        }
    }

    void IObservable::Publish() {
        if(proxy) {
                proxy->Publish(this);
            }
    }
}