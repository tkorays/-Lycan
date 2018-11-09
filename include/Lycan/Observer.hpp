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
    template <typename T> class Observable;
    

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
        friend class IObservable;
        virtual void PublishTo(IObservable* from, IObserver* to) = 0;
        void Subscribe(IObserver* observer, ObservableId id);
        void Unsubscribe(IObserver* observer, ObservableId id);
        void Register(ObservableId id, IObservable* observable);
        void Unregister(ObservableId id, IObservable* observable);
        template <typename T> Observable<T>* GetObservable(ObservableId id);
    protected: 
        virtual void Publish(IObservable* from);
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
        void SetId(ObservableId _id) { id = _id; }
        ObservableId GetId() const { return id; }
    protected:
        virtual void Publish();
        IObservableProxy* proxy;
        ObservableId id;
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
        const T& GetValue() const { return _observed_value; }
    protected:
        T _observed_value;
        bool is_first_assign;
    };

    //////////////////////////////////////////////////////////////////
    // start of implements
    //////////////////////////////////////////////////////////////////

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

    void IObservableProxy::Publish(IObservable* from) {
        if(!from) return;
        for(auto it : observers) {
            if(it.first == from->GetId()) {
                for(auto itt : it.second.second) {
                    PublishTo(from, itt);
                }
            }
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
            observable->SetId(id);
        }
    }

    void IObservableProxy::Unregister(ObservableId id, IObservable* observable) {
        if(!observable) return;
        auto it = observers.find(id);
        if(it != observers.end()) {
            observers.erase(it);
        }
    }

    template <typename T>
    Observable<T>* IObservableProxy::GetObservable(ObservableId id) {
        IObservable* ob = observers[id].first;
        if(ob) {
            Observable<T>* ret = dynamic_cast<Observable<T>*>(ob);
            if(ret) return ret;
        }
        return 0;
    }

    void IObservable::Publish() {
        if(proxy) {
                proxy->Publish(this);
            }
    }
}