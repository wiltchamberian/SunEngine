/*****************************************************************************
* @brief : handleMgr
* 句柄管理器，管理所有內部资产
* @author : acedtang
* @date : 2021/4/7
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __HANDLE_MGR_H
#define __HANDLE_MGR_H

#include <string>
#include <vector>
#include <unordered_map>
#include <numeric>
#include "Util/exception.h"
#include "Util/SmartPointer.h"
#include "Util/Guid.h"
#include "Handle.h"
#include "Math/NumericLimits.h"

namespace Sun {
    
    template<class Data, class Handle>
    class THandleMgr
    {
    private:
        void addCount(const Handle& handle) {
            uint32 index = handle.getIndex();
            ASSERT(index >= 0 && index < refCounts_.size());
            if (index >= 0 && index < refCounts_.size()) {
                refCounts_[index] += 1;
                ASSERT(refCounts_[index] < MAX_int32);
            }     
        }
        void subCount(const Handle& handle) {
            uint32 index = handle.getIndex();
            ASSERT(index >= 0 && index < refCounts_.size());
            if (index >= 0 && index < refCounts_.size()) {
                refCounts_[index] -= 1;
                ASSERT(refCounts_[index] < MAX_int32);
            }
        }
        std::vector<Data> userVec_;
        std::vector<Guid> magicVec_;
        //2021/5/12添加引用计数，与userVec 1-1对应
        std::vector<int> refCounts_;
        std::vector<uint32_t> freeVec_;

        std::unordered_map<Guid, uint32, GuidHashFunc, GuidEqual> lookup_;

        //标记有无执行过release操作
        bool isReleased_ = false;
    public:
        THandleMgr() {}
        ~THandleMgr() {}

        Data* getData(size_t index);

        //提供通过索引访问句柄的接口
        Handle getHandle(size_t index);

        Handle acquire();

        Data* acquireData();

        //申请内存，内部会生成guid，并返回新内存的句柄
        Data* acquire(Handle& handle);
        //使用guid查找对应的内存句柄，如果找到返回对应句柄；否则，相当于用该guid申请新内存并返回新内存句柄
        Data* acquire(Handle& handle, const Guid& guid);
        void release(Handle handle);

        //根据句柄提取数据(解引用)
        Data* dereference(Handle handle);

        uint32_t getUsedHandleCount() const
        {
            return magicVec_.size() - freeVec_.size();
        }
    };

    template<class Data, class Handle>
    Data* THandleMgr<Data, Handle>::getData(size_t index) {
        if (index < 0 || index >= userVec_.size())
            return nullptr;

        return &userVec_[index];
    }

    template<class Data, class Handle>
    Handle THandleMgr<Data, Handle>::getHandle(size_t index) {
        if (isReleased_) {
            ASSERT(false && "release has been called ,so the index may not match!");
        }
        if (index < 0 || index >= userVec_.size()) {
            //返回空句柄
            return Handle();
        }
        return Handle(index, magicVec_[index]);
    }

    template<class Data, class Handle>
    Handle THandleMgr<Data, Handle>::acquire() {
        Handle handle;
        acquire(handle);
        return handle;
    }

    template<class Data, class Handle>
    Data* THandleMgr<Data, Handle>::acquire(Handle& handle) {
        uint32_t index;
        if (freeVec_.empty()) {
            index = magicVec_.size();
            userVec_.emplace_back();
            handle.init(index);
            magicVec_.push_back(handle.getMagic());
            refCounts_.emplace_back(1);
            lookup_.insert(std::make_pair(handle.getMagic(),index));
        }
        else {
            index = freeVec_.back();
            freeVec_.pop_back();
            handle.init(index);
            magicVec_[index] = handle.getMagic();
            refCounts_[index] = 1;
            lookup_.insert(std::make_pair(handle.getMagic(), index));
        }
        return &userVec_[index];
    }

    template<class Data, class Handle>
    Data* THandleMgr<Data, Handle>::acquire(Handle& handle, const Guid& guid) {
        auto it = lookup_.find(guid);
        if (it != lookup_.end())
        {
            refCounts_[it->second]++;
            return &userVec_[it->second];
        }
        uint32_t index;
        if (freeVec_.empty()) {
            index = magicVec_.size();
            userVec_.emplace_back();
            refCounts_.push_back(1);
            handle.init(index);
            magicVec_.push_back(handle.getMagic());
        }
        else {
            index = freeVec_.back();
            freeVec_.pop_back();
            handle.init(index ,guid);
            magicVec_[index] = handle.getMagic();
            refCounts_[index] = 1;
        }
        return &userVec_[index];
    }

    template<class Data, class Handle>
    Data* THandleMgr<Data, Handle>::acquireData() {
        Handle handle;
        Data* d = acquire(handle);
        return d;
    }

    template<class Data, class Handle>
    void THandleMgr<Data, Handle>::release(Handle handle) {
        uint32_t index = handle.getIndex();
        ASSERT(index >= 0 && index < userVec_.size());
        ASSERT(handle.getMagic() == magicVec_[index]);

        refCounts_[index] -= 1;
        //引用计数为0 才释放内存
        if (refCounts_[index] <= 0) {
            magicVec_[index] = 0;
            freeVec_.push_back(index);
            lookup_.erase(handle.getMagic());
            refCounts_[index] = 0;

            isReleased_ = true;
        }

        
    }

    template<class Data, class Handle>
    Data* THandleMgr<Data, Handle>::dereference(Handle handle) {
        if (handle.isNull()) return 0;
        auto it = lookup_.find(handle.getMagic());
        if ( it== lookup_.end()) {
            return 0;
        }
        uint32 index = it->second;
        if (index >= userVec_.size() || magicVec_[index] != handle.getMagic()) {
            ASSERT(0);
            return 0;
        }
        return &userVec_[index];
    }


      
}

#endif 



