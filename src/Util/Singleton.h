#ifndef __SINGLETON_H
#define __SINGLETON_H

#include <mutex>
#include <memory>
#include <cassert>

namespace Sun {

    
    //for teaching
    template<class _T>
    class TSingleton{
    public:
        static _T* getInstance();
    private:
        TSingleton();
        TSingleton(const TSingleton&) = delete;
        TSingleton& operator=(const TSingleton&) = delete;
        ~TSingleton();
        static _T data_;
        static TSingleton<_T>* instance_;
        static std::mutex mutex_;
    };

    template<class _T>
    _T* TSingleton<_T>::getInstance() {
        if (instance_ == nullptr) {
            mutex_.lock();
            if (instance_ == nullptr) {
                instance_ = new TSingleton<_T>();
            }
            mutex_.unlock();
        }
        return &instance_->data_;
    }

    template<class _T>
    std::mutex TSingleton<_T>::mutex_;

    template<typename _T>
    TSingleton<_T>* TSingleton<_T>::instance_ = 0;

    template<typename _T>
    _T TSingleton<_T>::data_;
   
    template<class _T>
    TSingleton<_T>::TSingleton() {
    }

    template<class _T>
    TSingleton<_T>::~TSingleton() {
        assert(false);
    }
    

    ////for use 
    template<class _T>
    class Singleton {
    public:
        static _T* get();
        ~Singleton();
    private:
        Singleton() = default;
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;
        _T* data_;
        static std::unique_ptr<Singleton<_T>> instance_;
    };

    template<class _T>
    std::unique_ptr<Singleton<_T>> Singleton<_T>::instance_;

    template<class _T>
    Singleton<_T>::~Singleton() {
        delete data_;
    }

    template<class _T>
    _T* Singleton<_T>::get() {
        static std::once_flag s_flag;
        std::call_once(s_flag, []() {
            instance_.reset(new Singleton<_T>);
            instance_->data_ = new _T;
            });
        return instance_->data_;
    }
}

#endif