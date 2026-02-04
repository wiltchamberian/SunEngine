#include "AnimationSystem.h"
#include "Assets/AssetsMgr.h"
#include "Common/Common.h"
#include "Common/KeyBoard.h"

namespace Sun {

    void AnimationSystem::prepare() {

    }

    void AnimationSystem::tick(float dtime, const SEvent& ev) {
        if (ev.type == SEventType::SE_KEY) {
            if (ev.value == 'f' && g_keys['f'].down == true) {
                setBeginPlayAnim(true);
            }
        }
        if (!isPlayingAnim_) return;
        if (isFirstEnterAnim_ == true) {
            animDuration_ = 0;
            isFirstEnterAnim_ = false;
            backupTransforms();
        }
        bool allEnd = play(animDuration_);
        if (allEnd) {
            setBeginPlayAnim(false);
            animDuration_ = 0;
        }
        else {
            animDuration_ += dtime;
        }
    }

    bool AnimationSystem::play(float duration) {
        //AssetsMgr::AnimationMgr& animMgr = Singleton<AssetsMgr>::get()->getMgr<Animation>();
        std::shared_ptr<Animation> anim = animHandle_;
        if (anim == nullptr) return true;
        
        bool allEnd = true;
        for (NodeAnim& nodeAnim : anim->channels) {
            GameObject*& rotor = nodeAnim.gameobj_;
            ASSERT(rotor != nullptr);
            //¿˚”√duration≤Â÷µ
            double endTime = nodeAnim.endTime();
            if (duration <= endTime) {
                allEnd = false;
            }
            auto tr = nodeAnim.interpolate(duration);
            rotor-> setRelativeTransformation(Transform3D(std::get<0>(tr).value_, std::get<1>(tr).value_, std::get<2>(tr).value_) ,false);
        }
        if(actor_)
            actor_->updateDescendantsTransform();

        return allEnd;
    }

    void AnimationSystem::setBeginPlayAnim(bool begin) {
        isPlayingAnim_ = begin;
    }

    void AnimationSystem::setHandle(const std::shared_ptr<Animation>& handle) {
        animHandle_ = handle;
    }

    void AnimationSystem::backupTransforms() {
        //AssetsMgr::AnimationMgr& animMgr = Singleton<AssetsMgr>::get()->getMgr<Animation>();
        //Animation* anim = animMgr.dereference(animHandle_);
        std::shared_ptr<Animation> anim = animHandle_;
        if (anim == nullptr) return;

        size_t siz = anim->channels.size();
        rotorTransforms_.resize(siz);
        int i = 0;
        for (NodeAnim& nodeAnim : anim->channels) {
            rotorTransforms_[i] = nodeAnim.gameobj_->getRelativeTransform();
            ++i;
        }
        return;
    }

    void AnimationSystem::setBackTransforms() {
        /*AssetsMgr::AnimationMgr& animMgr = Singleton<AssetsMgr>::get()->getMgr<Animation>();
        Animation* anim = animMgr.dereference(animHandle_);*/
        std::shared_ptr<Animation> anim = animHandle_;
        if (anim == nullptr) return;

        int i = 0;
        size_t siz = anim->channels.size();
        ASSERT(siz == rotorTransforms_.size());
        for (NodeAnim& nodeAnim : anim->channels) {
            nodeAnim.gameobj_->setRelativeTransformation(rotorTransforms_[i], false);
            ++i;
        }
        if(actor_)
            actor_->updateDescendantsTransform();
    }

}