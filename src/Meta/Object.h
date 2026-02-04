/*****************************************************************************
* @brief : object
* 提供序列化，反射等功能
* @author : acedtang
* @date : 2021/3/19
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/
#ifndef __OBJECT_H
#define __OBJECT_H

#include <string>
#include <unordered_map>
#include "Util/Guid.h"

namespace Sun {

    template<class _T>
    inline _T* createDefaultSubObject() {
        return new _T();
    }

    class SObject {
    public:
        void setName(const std::string& name);
        inline Guid& guid() {
            return guid_;
        }
        inline std::string& name() {
            return name_;
        }
    protected:
        Guid guid_;
        std::string name_;

        //此设计不确定是否ok FIXME
        //所有继承自Object的对象都有唯一guid,并以指针形式存放在这里，
        //这样内存分配释放统一由Object管理，就不存在任何内存问题
        //考虑到数据量较大，用hashMap管理是适合的
        static std::unordered_map<Guid, SObject*> allObjects_;
    };
}


#endif


