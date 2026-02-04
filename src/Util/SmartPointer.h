/*****************************************************************************
* @brief : smartpointer
* 对普通指针提供一层封装，用于表示该指针的申请释放由某种特殊的内存池管理
* @author : acedtang
* @date : 2021/4/7
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SMARTPOINTER_H
#define __SMARTPOINTER_H

#include <memory>

namespace Sun
{
    template<typename _T, typename MemoryPool>
    struct CommonPtr {
        _T* ptr;
        //重载运算符
    };
    
    template <typename T> using ptr = CommonPtr<T, int>;
}

#endif