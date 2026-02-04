/*****************************************************************************
* @brief : Handle
* 句柄类，用于资源管理
* @author : acedtang
* @date : 2021/4/13
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __HANDLE_H
#define __HANDLE_H

#include "Util/Define.h"
#include "Util/exception.h"
#include "Math/NumericLimits.h"
#include "Util/Guid.h"
#include "Platform/PlatformMisc.h"

namespace Sun {
    template<class Tag>
    class THandle {
        uint32_t index_;
        Guid     magic_;
    public: //FIXME ,change to private
        //重载 赋值运算符，使得赋值时增加引用计数
        //THandle<Tag>& operator = (const THandle<Tag>& th)
        //{
        //    AssetsMgr* pm = Singleton<AssetsMgr>::get();
        //    THandleMgr<Tag, THandle<Tag>>& mgr = pm->getMgr<Tag>();
        //    *this = th;
        //    //赋值时引用计数+1,需要解决循环引用问题(FIXME)
        //    mgr.addCount(*this);
        //    return *this;
        //}

        THandle()
            :index_(0)
        {
            magic_ = {};
        }   
        THandle(uint32_t index, Guid guid) {
            index_ = index;
            magic_ = guid;
        }
        ~THandle()
        {
            //THandleMgr<Tag, THandle<Tag>>& mgr = Singleton<AssetsMgr>::get()->getMgr<Tag>();
            ////析构时引用计数-1
            //mgr.release(*this);
        }
        void init(uint32 index) {
            ASSERT(isNull());
            ASSERT(index <= MAX_int32);
            index_ = index;
            magic_ = createGuid();
        }
        void init(uint32 index, const Guid& guid) {
            ASSERT(isNull());
            ASSERT(index <= MAX_int32);
            index_ = index;
            magic_ = guid;
        }
        bool isValid() const {
            return !isNull();
        }
        bool isNull() const {
            return magic_.empty();
        }
        Guid getMagic() const {
            return magic_;
        }
        uint32_t getIndex() const {
            return index_;
        }
    };

}


#endif