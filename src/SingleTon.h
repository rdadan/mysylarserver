#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <memory>

namespace rdadan {
// 饿汉模式，线程安全，类创建的同时就会创建单例
template <class T>
class SingleTon {
public:
    static T *getInstance() {
        static T v;
        return &v;
    }
};
template <class T, class X = void, int N = 0>
class SingletonPtr {
public:
    static std::shared_ptr<T> getInstancePtr() {
        static std::shared_ptr<T> ptr(new T);
        return ptr;
    }
};

} // namespace rdadan
#endif