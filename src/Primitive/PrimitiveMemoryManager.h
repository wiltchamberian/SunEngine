/*****************************************************************************
* @brief : PrimitiveMemoryManager   (FIXME!)
* 原语内存管理器  
* 专门为GPU访问设计的底层数据管理系统
* 目前按照最简单的方法实现，所有数据存储在一块连续区域上
* 由于是pod类型，拷贝安全，因此采用vector的自适应增长内存管理
* 
* 由于primitive对象之间可以认为存储顺序无关（primitive表示空间几何体，它们的顺序和内存中存放的顺序无关)
* 因此，释放primitive对象，采用末尾替换的方式（也就是把尾部的原语直接移动到释放点)
* 由于内存可能更新，指针的访问方式无效，因此通过primitiveEntity的索引来访问，同时，提供了
* 访问器，
* 
* 另外因为将不同类型的primitive存放在一起，尺度可能不同，因此需要更新策略需要单独实现，并且可能会
* 存在内存间隔，这里的优化策略留给后续改进
* 
* 由于原语可能发生移动，则其对应的数组索引也会改变，gameplay层不能直接持有primitive索引，而是持有primitive
* 关联的primitiveEntity的索引，通过primitiveEntity中存放的primitive索引再去访问primitive，
* primitive移动时由primitiveManager修改primitiveEntity中关联的primitive索引
* 当primitive发生删除时，对应的primitiveEntity置为无效，后续分配primitiveEntity可能位于该无效位置上，
* 此时gameplay层将引用到错误的primitiveEntity,由于释放primitive的操作通常由gameplay层来完成，这里的安全性暂时交给
* gameplay层控制
* 
* 同时，考虑到GPU的上下文切换优化，需要将同类别的原语合并到一起，因此我们可以拓展支持多个原语管理系统
* 每个管理系统支持 一种类型的原语，这对于单个原语系统支持多类型的灵活性并不矛盾（单个原语系统也可以支持单类别）
* 但是目前，为避免复杂性，我们不采用多个管理体系，采用单个管理体系，因为原语顺序无关，我们采用内存置换的方式（算法）
* 来调整原语内部结构
* 
* 由于内存置换成本过高，几乎相当于拷贝，其实际意义不大，
* 另外混合存储模式带来的内存管理（也就是空闲内存填充会有很大问题），可能带来
* 大量内存碎片，得不偿失，最终还是采取同类型的原语放在一起存储
* 
* 主要优点如下:
* 1,编码简单
* 2，无内存碎片（释放的区域可以被有效填充)
* 3, 无需内存置换，可以复用到GPU
* 还是改为同类型的原语存放在一起的模式
* 
* 同时为了提高灵活性，entity不仅能所引到不同的类别的单个对象
* 也可以索引到某个类别的一个连续区间（该区间至少作为整体释放或者删除，甚至可以缩短)
* 句柄中添加信息表示这是一个区间还是单个值
* 
* 所有以上功能都被覆盖在PrimitiveMemoryManager中
* 
*
* @author : acedtang
* @date : 2021/5/9
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __PRIMITIVE_MANAGER_H
#define __PRIMITIVE_MANAGER_H

#include <vector>
#include "Util/exception.h"
#include "PrimitiveDefine.h"
#include "StaticMeshPrimitive.h"
#include "SpherePrimitive.h"
#include "SkeletalMeshPrimitive.h"
#include "BoxPrimitive.h"
#include "PrimitiveEntity.h"

namespace Sun {

    //伪单例
    class PrimitiveMemoryManager {
    public:
        template<class _T>
        PHandle<_T> acquire() {
            int h = acquire(_T::type_);
            return PHandle<_T>(h);
        }
        template<class _T>
        _T* deReference(const PHandle<_T>& h) {
            _T* res = nullptr;
            bool ok = deReference(h.h, res);
            if (ok == false) return nullptr;
            return res;
        }

        //删除
        bool destroy(int h);

    protected:
        //返回primitiveEntity索引 ,-1为无效值 ,申请内存
        int acquire(int primitiveType);
        bool deReference(int h, BoxPrimitive*& data);
        bool deReference(int h, SpherePrimitive*& data);
        bool deReference(int h, StaticMeshPrimitive*& data);
        bool deReference(int h, SkeletalMeshPrimitive*& data);

        std::vector<BoxPrimitive> boxPrimitives_;
        //from index of boxPrimitives to index of entity
        std::vector<int> boxReverseMap_;

        std::vector<SpherePrimitive> spherePrimitives_;
        std::vector<int> sphereReverseMap_;

        std::vector<StaticMeshPrimitive> staticMeshPrimitives_;
        std::vector<int> staticMeshReverseMap_;

        std::vector<SkeletalMeshPrimitive> skeletalMeshPrimitives_;
        std::vector<int> skeletalMeshReverseMap_;
        
        //entity指向上面的索引
        std::vector<PrimitiveEntity> primitiveEntities_;
        std::vector<int> freedEntities_;
    };

    extern PrimitiveMemoryManager g_primitiveMgr;
}

#endif