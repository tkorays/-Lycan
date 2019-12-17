#pragma once 
#include "Crack.hpp"

namespace Lycan {
    class IScopeTarget {
    public:
        virtual void DoStart() = 0;
        virtual void DoEnd() = 0;
    };

    template<class TScopeTarget> 
    class ScopeWrapper : private DisableCopy {
    public: 
        ScopeWrapper(TScopeTarget* t) {
            if(!t) return;
            scopeObj = t;
            scopeObj->DoStart();
        }
        ~ScopeWrapper() {
            scopeObj->DoEnd();
        }
    protected:
        TScopeTarget* scopeObj;
    private: 
        ScopeWrapper() { }
    };
};
