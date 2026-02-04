/*****************************************************************************
* @brief : bones
* @author : acedtang
* @date : 2021/3/23
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/

#ifndef __JOINT_H
#define __JOINT_H

#include <vector>
#include <string>
#include "Util/Tree.h"
#include "Math/Transform3D.h"

namespace Sun {
    template<class _T>
    struct IndexPointer {
        IndexPointer()
            :index_(0)
            ,container_(nullptr)
        {}
        IndexPointer(std::vector<_T>* vec, int index)
            :index_(index)
            , container_(vec)
        {}
        IndexPointer(_T* t)
            :index_(0)
            , container_(nullptr)
        {}
        
        int index_;
        std::vector<_T>* container_;
        _T* operator->() {
            return &(*container_)[index_];
        }
        _T& operator* () {
            return (*container_)[index_];
        }
        operator bool() {
            return !(container_ == nullptr);
        }
        
    };

    struct Joint : public TreeNodeBase<IndexPointer<Joint>>
    {
        std::string name_;
        int oriIndex;
        //Transform3D oriRelativeTransform_;
        Transform3D relativeTransform_;
        Transform3D offsetTransform_;

        //经过动画计算的结果矩阵(初始等于relativeTransform_)
        Transform3D animTransform_;

        Transform3D jointTransform_;
    };

    typedef IndexPointer<Joint> JointPointer;
}


#endif