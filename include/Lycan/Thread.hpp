#pragma once
#include <thread>

namespace Lycan {
    class IThreadPool {
    public:
        void* Malloc(size_t n) {
            return 0;
        }
        void Free(void*) {
            
        }
    };

    class IHasMemoryPoolClass {
    public: 
        IHasMemoryPoolClass() {

        }
        ~IHasMemoryPoolClass() {

        }

        void* Malloc(size_t n) {
            return m_pool->Malloc(n);
        }
        void Free(void* p) {
            m_pool->Free(p);
        }

        //template <typename T>
        void* operator new(size_t n) {
            return 0;
        }
    protected:
        Pool* m_pool;
    };

    class IThreadClass : public IHasMemoryPoolClass {
    public: 
        void StartThread() {
            std::thread t(TreadFunc);
        }
        virtual void RunThread() = 0;
    protected: 
        static void TreadFunc(IThreadClass* _thisClass) {
            if(!_thisClass) return;
            _thisClass->RunThread();
        }
    protected: 
        
    };

    class A : public IHasMemoryPoolClass {
    public:
        int a;
    };

    class B : public IThreadClass {
    public:
        void StartUp() {
            a = new A;
        }
        virtual void RunThread() {

        }
    private:
        A* a;
    };
}