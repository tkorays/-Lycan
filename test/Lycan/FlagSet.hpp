#pragma once

namespace Lycan {
    template <typename T, int MaxId=32>
    class FlagSet {
    public:
        enum { Count = MaxId, Bytes = (MaxId >> 3) + 1 };

        bool Check(T flag) {
            int pos = static_cast<int>(flag);
            int index = pos >> 3;
            pos = pos - (index<<3);
            return static_cast<bool>(bits[index] & (1<<pos));
        }

        void Set(T flag) {
            int pos = static_cast<int>(flag);
            int index = pos >> 3;
            pos = pos - (index<<3);
            bits[index] = bits[index] | (1<<pos);
        }

        void Clear(T flag) {
            int pos = static_cast<int>(flag);
            int index = pos >> 3;
            pos = pos - (index<<3);
            bits[index] = bits[index] & ~(1<<pos);
        }

        FlagSet& operator<<(T flag) {
            Set(flag);
            return *this;
        }
    private:
        char bits[Bytes] = {0};
    };
}
