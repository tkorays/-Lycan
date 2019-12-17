#pragma once

#include "CreatePolicy.hpp"

namespace Lycan {
    #define __LYCAN_SINGLETON__(_className, _createPolicy) public: \
        friend class _createPolicy; \
        static _className* GetInstance() { \
            return _createPolicy::Create(); \
        } \
        static void DestroyInstance(_className* t) { \
            _createPolicy::Destroy(t); \
        } \
     private: \
        _className() { } \
        ~_className() { } \
        _className& operator=(const _className&); \
        _className(const _className&); 
}
