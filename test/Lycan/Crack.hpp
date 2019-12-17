#pragma once

namespace Lycan {
    class DisableCopy {
    public:
        DisableCopy() { }
        ~DisableCopy() { }
    private: 
        DisableCopy(const DisableCopy&);
        DisableCopy& operator=(const DisableCopy&);
    };
};