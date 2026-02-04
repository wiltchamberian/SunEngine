#include "Animation.h"

namespace Sun {

    NodeAnim::NodeAnim() {}
    NodeAnim::NodeAnim(const NodeAnim& cpy) {
        positionKeys = std::move(cpy.positionKeys);
        rotationKeys = std::move(cpy.rotationKeys);
        scalingKeys = std::move(cpy.scalingKeys);
        nodeName_ = cpy.nodeName_;
        oriNodeIndex_ = cpy.oriNodeIndex_;
        preState = cpy.preState;
        postState = cpy.postState;
    }
    NodeAnim::NodeAnim(NodeAnim&& cpy) {
        positionKeys = cpy.positionKeys;
        rotationKeys = cpy.rotationKeys;
        scalingKeys = cpy.scalingKeys;
        nodeName_ = cpy.nodeName_;
        oriNodeIndex_ = cpy.oriNodeIndex_;
        preState = cpy.preState;
        postState = cpy.postState;
    }
    NodeAnim& NodeAnim::operator = (const NodeAnim& cpy) {
        positionKeys = cpy.positionKeys;
        rotationKeys = cpy.rotationKeys;
        scalingKeys = cpy.scalingKeys;
        nodeName_ = cpy.nodeName_;
        oriNodeIndex_ = cpy.oriNodeIndex_;
        preState = cpy.preState;
        postState = cpy.postState;
        return *this;
    }
    bool NodeAnim::move() {
        bool res = false;
        index += 1;
        if (index == positionKeys.size()) {
            index = 0;
            res = true;
        }
        return res;
    }
    std::tuple<VectorKey, QuaternionKey, VectorKey> NodeAnim::interpolate(float duration) {
        std::tuple<VectorKey, QuaternionKey, VectorKey> tu;

        std::get<0>(tu) = Math::interpolate_linear(positionKeys.begin(), positionKeys.end(), duration);
        //std::get<1>(tu) = Math::interpolate_linear(rotationKeys.begin(), rotationKeys.end(), duration);
        std::get<2>(tu) = Math::interpolate_linear(scalingKeys.begin(), scalingKeys.end(), duration);

#if 1
        //四元数的球面线性插值
        bool binGo = false;
        for (int i = 0; i < rotationKeys.size(); ++i) {
            if (duration < rotationKeys[i].time_) {
                if (i == 0) {
                    std::get<1>(tu) = rotationKeys[i];
                    binGo = true;
                    break;
                }
                else {
                    double frac = (duration - rotationKeys[i - 1].time_) / (rotationKeys[i].time_ - rotationKeys[i - 1].time_);
                    //球面线性插值
                    std::get<1>(tu).value_ = Quaternion::slerp(rotationKeys[i - 1].value_, rotationKeys[i].value_, frac);
                    binGo = true; break;
                }
            }
        }
        if (!binGo && !rotationKeys.empty()) {
            std::get<1>(tu) = rotationKeys.back();
        }
#endif

        return tu;
    }

    double NodeAnim::endTime() {
        double time1 = positionKeys.empty() ? 0 : positionKeys.back().time_;
        double time2 = rotationKeys.empty() ? 0 : rotationKeys.back().time_;
        double time3 = scalingKeys.empty() ? 0 : scalingKeys.back().time_;
        return Math::upperBound<double>(time1, time2, time3);
    }


}