/*****************************************************************************
* @brief : brief
* 内存池，该内存池主要为引擎服务，参考了Unity的ECS架构，
* 分块链表的形式维护
* @author : acedtang
* @date : 2021/3/30
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/

#ifndef __MEMORY_POOL_H
#define __MEMORY_POOL_H

#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <vector>
#include <cstdint>
#include "exception.h"
#include "sp.h"

namespace Sun {

    inline void* zeroMalloc(size_t n) {
        void* p = malloc(n);
        memset(p, 0, n);
        return p;
    }

    struct Head {
        //指向下一个内存块
        Head* next;
        //当前块第一个未使用的索引
        int unUse;
    };

#define BLOCK_LEN  1<<13

    template<class _T>
    struct BlockInfo {
        _T* head;
        //bool isFull;
        int unUseIndex;
        //标记使用的_T块个数，为0的时候清空这块内存
        int numUsed;
    };

    class BasePool
    {

    };

    template<class _T>
    class MemoryPool:public BasePool
    {
    public:
        MemoryPool(size_t blocklen = BLOCK_LEN);
        template<class ... _Types>
        sp<_T> malloc(_Types&&... _Args);
        
        void cover(sp<_T> ptr);

        void free(sp<_T> ptr);
        void setType(int type);
    private:
        void updateUnUseIndex(int blockIndex, _T* p);
        //Head* first_ = nullptr;

        //每个独立数据块信息
        std::vector<BlockInfo<_T>> blockInfoVec_;
        //单个区块能存储的最大个数
        std::int32_t maxNumInOneBlock_;
        //块的字节数
        std::int32_t blockLen_;
        std::int32_t type_;
    };

    template<class _T>
    void Sun::MemoryPool<_T>::cover(sp<_T> ptr)
    {

    }

    template<class _T>
    void Sun::MemoryPool<_T>::setType(int type)
    {
        type_ = type;
    }

    template<class _T>
    void Sun::MemoryPool<_T>::updateUnUseIndex(int blockIndex,  _T* p)
    {
        bool found = false;
        int index = blockInfoVec_[blockIndex].unUseIndex;
        int i = index +1;
        while (i < maxNumInOneBlock_) {
            p += 1/*typeLen_*/;
            if (!p->inUse()) {
                blockInfoVec_[blockIndex].unUseIndex = i;
                found = true;
                break;
            }
        }
        if (found == true) return;
        p = blockInfoVec_[blockIndex].head;
        for (int i = 0; i < index; ++i) {
            if (!p->inUse()) {
                blockInfoVec_[blockIndex].unUseIndex = i;
                found = true;
                break;
            }
            p += 1/*typeLen_*/;
        }
        if (found) return;

        blockInfoVec_[blockIndex].unUseIndex = -1;
    }

    template<class _T>
    Sun::MemoryPool<_T>::MemoryPool(size_t blockLen)
    {
        maxNumInOneBlock_ = blockLen / sizeof(_T);
        blockLen_ = blockLen;
    }

    template<class _T>
    template<class... _Types>
    Sun::sp<_T> Sun::MemoryPool<_T>::malloc(_Types&&... _Args)
    {
        sp<_T> res;
        for (int i = 0; i < blockInfoVec_.size(); ++i) {
            //含有未用空间
            if (blockInfoVec_[i].unUseIndex != -1) {
                res.ptr_ = blockInfoVec_[i].head + /*typeLen_ * */blockInfoVec_[i].unUseIndex;
                new(res.ptr_)_T(_STD forward<_Types>(_Args)...);
                res.ptr_->setUse(true);
                res.ptr_->setType(type_);
                res.ptr_->setIndex(i * maxNumInOneBlock_ + blockInfoVec_[i].unUseIndex);
                updateUnUseIndex(i, res.ptr_);
                return res;
            }
        }
        //新分配空间
        BlockInfo<_T> info = {};
        void* vp = zeroMalloc(blockLen_);
        info.head = new(vp)_T(_STD forward<_Types>(_Args)...);
        info.unUseIndex = 1;
        blockInfoVec_.push_back(info);
        info.head->setUse(true);
        info.head->setType(type_);
        //info.head->setBlockIndex(blockInfoVec_.size() - 1);
        info.head->setIndex( (blockInfoVec_.size()-1) * maxNumInOneBlock_);
         
        return sp<_T>(info.head);
    }

    template<class _T>
    void Sun::MemoryPool<_T>::free(sp<_T> ptr)
    {
        if (!ptr) return;
        ASSERT(ptr.type_ == type_);
        
        int blockIndex = ptr->getIndex() / maxNumInOneBlock_;
        int index = ptr->getIndex() % maxNumInOneBlock_;
        //当服务端创建了对象，而该条消息丢失，之后服务端又销毁该对象时可能发生
        if (blockIndex >= blockInfoVec_.size()) {
            int l = blockIndex - blockInfoVec_.size() + 1;
            //这个l过大说明有异常
            if (l > 1) {
                reportException();
            }
            for (int i = 0; i < l; ++i) {
                BlockInfo<_T> info = {};
                info.head = (_T*)zeroMalloc(blockLen_);
                blockInfoVec_.push_back(info);
            }
        }
        BlockInfo<_T>& blockInfo = blockInfoVec_[blockIndex];
        _T* p = blockInfo.head + index;
        p->setUse(false); 
    }

    



}



#endif

