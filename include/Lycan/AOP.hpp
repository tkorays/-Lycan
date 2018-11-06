#pragma once

namespace Lycan {
    #define __LYCAN_AOP__ AOPClass aop(__FUNCTION__);

    typedef const char* NullAOPClass;

    class SimpleAOPClass {
    public:
        SimpleAOPClass(const char* s) { }
        ~SimpleAOPClass() { }
    };

    template <class TAOPClass = NullAOPClass>
    class EnableAOP {
    public:
        typedef TAOPClass AOPClass;
    };
    
};
