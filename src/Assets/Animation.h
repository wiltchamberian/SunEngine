/*****************************************************************************
* @brief : animation
* 动画
* 骨骼蒙皮动画，
* @author : acedtang
* @date : 2021/3/23
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/

#ifndef __ANIMATION_H
#define __ANIMATION_H

#include <vector>
#include <string>
#include <tuple>
#include "Math/Quaternion.h"
#include "Math/Vector3D.h"
#include "Math/MathUtils.h"
#include "Util/Factory.h"
#include "Util/Guid.h"

namespace Sun {
    struct VectorKey {
        double time_;
        vec3 value_;
        double& key() {
            return time_;
        }
        vec3& value() {
            return value_;
        }
    };
    struct QuaternionKey {
        double time_;
        Quaternion value_;
        double& key() {
            return time_;
        }
        Quaternion& value() {
            return value_;
        }
    };

    enum AnimBehaviour { //from assimp,may revise FIXME
        //缺省
        AnimBehaviour_DEFAULT = 0x0,
        //选择最近的值，不插值
        AnimBehaviour_CONSTANT = 0x1,
        //最近的两个线性差值
        AnimBehaviour_LINEAR = 0x2,
        // If the animation key go from n to mand the current
        // time is t, use the value at(t - n) % (| m - n | ).* /
        AnimBehaviour_REPEAT = 0x3,
    };

    class GameObject;
    //节点动画
    class NodeAnim {
    public:
        NodeAnim();
        NodeAnim(const NodeAnim& cpy);
        NodeAnim(NodeAnim&& cpy);
        NodeAnim& operator = (const NodeAnim& cpy);
        bool move();
        std::tuple<VectorKey, QuaternionKey, VectorKey> interpolate(float duration);
        double endTime();

        std::string nodeName_;
        int oriNodeIndex_;

        //动画分多个时间节点绘制，数组中每一项对应某个时间节点的状态信息
        //实际绘制时，根据插值得到正确值
        std::vector<VectorKey> positionKeys;
        std::vector<QuaternionKey> rotationKeys;
        std::vector<VectorKey> scalingKeys;

        //表示插值的方式
        AnimBehaviour preState;
        AnimBehaviour postState;

        //标记当前动画进行到哪一个阶段，实际就是positionKeys的索引
        int index = 0;
        //关联的rotor (used by gltfloader2)
        GameObject* gameobj_;
    };

    class AnimationSystem;
    struct Animation {  //from assimp,may optimize FIXME
        std::string name;

        //含义是该动画有多少个tick
        double duration;

        //每秒多少次tick
        double ticksPerSecond;
        
        //每个关节的动画
        std::vector<NodeAnim> channels;

        //TODO
        //网格动画
      
        //变形网格动画

        AnimationSystem* animSys = nullptr;

        Guid guid_;
    };
}

#endif