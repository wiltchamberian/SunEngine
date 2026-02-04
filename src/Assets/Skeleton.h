/*****************************************************************************
* @brief : 骨骼
* @author : acedtang
* @date : 2021/3/24
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SKELETAL_H
#define __SKELETAL_H

#include <vector>
#include <map>
#include "Joint.h"
#include "Util/Factory.h"
#include "Meta/Object.h"
#include "Util/Tree.h"

namespace Sun {
    class SSkeletalMeshSocket;
    class GameObject;

    class Skeleton : public SObject {
        friend class SModelLoader;
        friend class Animation;
        friend class SSkeletalMeshComponent;
    public:
        JointPointer addJoint(JointPointer parent, const Joint& joint);
        JointPointer addJoint2(JointPointer parent, const Joint& joint);
        int jointIndex(JointPointer p);
        int jointIndex(const std::string& str);
        int mapIndex(int);
        std::vector<Joint> exportJointTree();
        const std::vector<Joint>& getJointTree();

        void calculateJointTransform();
    protected:
        //数组中第一个joint是根节点
        //用数组形式存储是为了传给GPU
        std::vector<Joint> jointTree_;

        //key是ori index,value是joinTree_中的索引
        std::map<int, int> jointMap_;

        std::map<std::string,int> jointMap2_;

        //插槽
        std::vector<SSkeletalMeshSocket*> sockets_;
    };

    //该骨骼只是关联一些rotor而已
    class Skeleton2 
    {
        friend class Animation;
        friend class SSkeletalMeshComponent;
    public:
        Guid guid_;
        int mapIndex(int oriIndex) {
            auto it = lookupMap_.find(oriIndex);      
            if (it == lookupMap_.end()) {
                //==ASSERT(false);
                //gltf do not give a clear description how to process this,we
                //just attach it to the root 
                auto it2 =  lookupMap_.find(oriRootIndex);
                if (it2 == lookupMap_.end()) {
                    //ASSERT(false);
                    return 0;
                }
                return it2->second;
            }
            return it->second;
        }
        std::vector<Matrix4x4> calculateJointTransform();

        //骨骼的节点
        std::vector<GameObject*> joints_;
        //和joints_等长，表示每个节点的初始全局逆绑定矩阵(由于动画会改变rotor,因此需要存储一份初始逆绑定矩阵)
        std::vector<Matrix4x4> invBindMatrixes_;//用矩阵貌似计算精度不够
        //std::vector<Transform3D> invBindTransforms_;
        // 
        // 
        //建立原始节点索引到关节数组索引的映射
        std::map<int, int> lookupMap_;
        //记录一个特殊的骨骼根节点，绑定不上的顶点直接绑定到根骨骼
        int oriRootIndex = 0;
    };
}

#endif