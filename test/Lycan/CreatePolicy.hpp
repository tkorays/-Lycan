#pragma once

namespace Lycan {

template <class T>
class ICreatePolicy {
public:
    static T* Create() {
        return 0;
    }

    static void Destroy(T* obj) {

    }
};

template <class T>
class CreateByNewPolicy : public ICreatePolicy<T> {
public:
    static T* Create() {
        return new T();
    }

    static void Destroy(T* obj) {
        if(obj) delete obj;
    }
};

template <class T>
class CreateByStaticPolicy : public ICreatePolicy<T> {
public:
    static T* Create() {
        static T obj;
        return &obj;
    }

    static void Destroy(T* obj) {

    }
};

}