#ifndef __LYCAN_MEMORY_H__
#define __LYCAN_MEMORY_H__

#include <cstddef>
#include <memory>

namespace lycan {

template <typename T>
void* MemoryAlign(T* p, size_t align) {
    return (void*)(((uintptr_t)(p)+(align-1)) & ~(align-1));
}

template <typename T, int TAlign>
class Allocator {
public: 
    Allocator() {}
    ~Allocator() {}
    T* malloc(size_t count) {
        return allocate(count);
    }
    void free(T* p) {
        return deallocate(p);
    }
    static T* allocate(size_t count) {
        void* p = malloc(sizeof(T)*count + sizeof(uintptr_t) + TAlign);
        void* t = MemoryAlign(p, TAlign);
        *((uintptr_t*)t - 1) = (uintptr_t)p;
        return (T*)t;
    }
    static void deallocate(T* p) {
        if(p) free((void*)*((uintptr_t*)p - 1));
    }
    static int align() { return TAlign; }
};

template <typename T>
class Allocator<T, 0> {
public: 
    Allocator() { }
    ~Allocator() { }
    T* malloc(size_t count) {
        return allocate(count);
    }
    void free(T* p) {
        return deallocate(p);
    }
    static T* allocate(size_t count) {
        return (T*)malloc(count*sizeof(T));
    }
    static void deallocate(T* p) {
        if(!p) return;
        free(p);
    }
    static int align() { return 0; }
};


}

#endif /* __LYCAN_MEMORY_H__ */
