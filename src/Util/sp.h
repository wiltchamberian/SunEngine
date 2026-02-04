/*****************************************************************************
* @brief : sp
* 引擎内存管理智能指针,该智能指针与引擎的内存管理配套
* 有了该指针，使用者无需纠结是定义成员变量还是成员指针，
* 统一使用该指针包的对象
* 也无需担心成员指针的管理问题，这部分完全交给了引擎的
* 内存管理组件
* 同时该智能指针实际只是存储区域的一块弱引用，因此可以随意创建，释放
* 但用户不能操作实际指向的空间，这部分由引擎统一管理
* 
* 简单理解，所有内存交给引擎管理，用户无法通过该指针真正释放或者直接申请，
* 如果确实需要，通过相应的工厂创建或者释放
* @author : acedtang
* @date : 2021/3/29
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/

#ifndef __SP_H
#define __SP_H

#include <cstdint>

namespace Sun {

    struct SId{
        inline void setUse(bool use) {
            inUse_ = use;
        }
        inline bool inUse() {
            return inUse_;
        }
        inline void setType(std::uint32_t type) {
            id_ = id_ &(0xffffffff)& ((std::uint64_t)type << 32);
        }
        inline std::uint32_t getType() {
            return id_ >> 32;
        }
        inline void setIndex(std::int64_t index) {
            id_ = id_ & index;
        }
        inline std::int64_t getIndex() {
            return id_ & (0xffffffff);
        }
        //高4个字节存type,低4个字节存index
        std::int64_t id_ ;
        bool inUse_;
    };

    template<typename _T>
    class sp
    {
        //friend class ComponentFactory<_T>;
        template<class _T> friend class MemoryPool;
    public:
        //create a empty
        sp();
        sp(_T* t);
        sp(const sp<_T>& ptr) {
            ptr_ = ptr.ptr_;
            type_ = ptr.type();
        }
        _T*& ptr();
        _T* data() const;
        int type() const;
    public:
        template<class A, class B>
        friend sp<A> cast(sp<B> b);

        operator bool() const {
            return ptr_ != nullptr;
        }
        bool operator == (sp<_T> other) {
            return ptr_ == other.ptr_;
        }
        bool operator != (void* pt) {
            return ptr_ != pt;
        }
        bool operator != (const sp<_T>& other) {
            return ptr_ != other.ptr_;
        }
        template<class _U>
        sp<_T> operator = (sp<_U> other) {
            ptr_ = other.data();
            type_ = other->type_;
        }
        void setEmpty();
        bool isValid();
        bool isEmpty();
        //_T& operator*();		//解引用运算符重载
        _T* operator->();	//成员运算符重载
    private:     
        _T* ptr_;
        int type_;
    };

    template<typename _T>
    _T*& Sun::sp<_T>::ptr() {
        return ptr_;
    }

    template<typename _T>
    int Sun::sp<_T>::type() const
    {
        return type_;
    }

    template<typename _T>
    _T* Sun::sp<_T>::data() const
    {
        return ptr_;
    }

    template<typename _T>
    void Sun::sp<_T>::setEmpty() {
        ptr_ = nullptr;
    }

    template<typename _T>
    bool Sun::sp<_T>::isValid()
    {
        return ptr_ != nullptr;
    }

    template<typename _T>
    bool Sun::sp<_T>::isEmpty()
    {
        return ptr_ == nullptr;
    }

    template<typename _T>
    Sun::sp<_T>::sp(_T* t)
    {
        ptr_ = t;
    }

    template<typename _T>
    Sun::sp<_T>::sp()
        :ptr_(nullptr)
    {
        
    }

    //template<typename _T>
    //_T& sp<_T>::operator*()		//解引用运算符重载
    //{
    //    return *ptr_;
    //}

    template<typename _T>
    _T* sp<_T>::operator->()		//成员运算符重载
    {
        return ptr_;
    }

    template <class T, class U>
    sp<T> smart_pointer_cast(const sp<U>& pp) {
        sp<T> res;
        res.ptr_ = pp.data();
        res.type_ = pp.type();
        return res;
    }

    template<class A, class B>
    sp<A> cast(sp<B> b) {
        sp<A> ptr;
        ptr.type_ = b.type_;
        ptr.ptr_ = (b.ptr_);
        return ptr;
    }
}

#endif


