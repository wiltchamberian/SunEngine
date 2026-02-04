#ifndef __STACK_H
#define __STACK_H

#include "Common/Define.h"

namespace Sun {

    template<class _T ,int32 N>
    class Stack{
    public:
        Stack() {
            stack_ = arr_;
        }
        __forceinline void push(const _T& val) {
            arr_[count_] = val;
            ++count_;
        }
        __forceinline void push_back(const _T& val) {
            arr_[count_] = val;
            ++count_;
        }
        __forceinline _T& top() {
            return arr_[count_-1];
        }
        __forceinline _T& back() {
            return arr_[count_-1];
        }
        __forceinline void pop() {
            count_--;
            count_ = count_ < 0 ? 0 : count_;
        }
        __forceinline void pop_back() {
            count_--;
            count_ = count_ < 0 ? 0 : count_;
        }
        __forceinline bool empty() const {
            return count_ <= 0;
        }

        _T* stack_;
        _T arr_[N];
        int32 count_ = 0;
        int32 capacity_ = N;
    };

}

#endif