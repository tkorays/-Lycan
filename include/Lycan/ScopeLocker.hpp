#pragma once
#include "ScopeWrapper.hpp"
#include <mutex>

namespace Lycan {
    class ILocker : public IScopeTarget {
    public:
        virtual ~ILocker() {}
        virtual void Lock() = 0;
        virtual void Unlock() = 0;
        virtual void DoStart() { Lock(); }
        virtual void DoEnd() { Unlock(); } 
    };

    class SimpleLocker : public ILocker {
    public: 
        virtual ~SimpleLocker() { }
        virtual void Lock() { mtx.lock(); };
        virtual void Unlock() { mtx.unlock(); };
    protected:
        std::mutex mtx;
    };

    typedef ScopeWrapper<ILocker> AutoScopeLocker;
}