#include "World.h"
#include "Util/HFile.h"
#include "Util/Factory.h"
#include "System/PlayerControlSystem.h"
#include "Component/LightTransformCom.h"

namespace Sun {

    SWorld::SWorld() {
        init();
    }

    SWorld::~SWorld() {
        clear();
    }

    SWorld::SWorld(const char* filepath) {
        loadFile(filepath);
    }

    void SWorld::init() {
        playerControlSystem_ = new PlayerControlSystem(this);
    }

    void SWorld::clear() {
        delete playerControlSystem_;
        for (auto& it : levels_) {
            delete it;
        }
        levels_.clear();
    }

    void SWorld::load(const char* filepath , bool isClear) {
        if (isClear)clear();
        loadFile(filepath);
    }

    void SWorld::prepare() {
        SLevel* level = getCurLevel();
        if (level) {
            level->prepare();
        }
    }

    void SWorld::save(const char* filepath) {
        //TODO
    }

    void SWorld::addMainPlayer() {
        if (mainPlayer_==nullptr) {
            Character* character = ActorFactory<Character>::get()->malloc();
            character->setName("character");
            SharedPtr<LightTransformCom> trans(new LightTransformCom);
            character->setComponent(trans);
            mainPlayer_ = character;
            playerControlSystem_->actor_ = character;

            


            character->setWorld(this);
            
        }
    }

    SLevel* SWorld::createLevel() {
        SLevel* level = new SLevel(this);
        levels_.push_back(level);
        return level;
    }

    void SWorld::addLevel(SLevel* level) {
        levels_.push_back(level);
    }

    void SWorld::addObject(GameObject* obj) {
        auto level = getCurLevel();
        if (level) {
            level->addActor(obj);
        }
    }

    SLevel* SWorld::getCurLevel() {
        if (cur_ < 0 || cur_ >= levels_.size()) {
            ASSERT(false);
            return nullptr;
        }
        return levels_[cur_];
    }

    Character* SWorld::getMainPlayerPtr() {
        return mainPlayer_;
    }

    Matrix4x4 SWorld::getMainPlayerCameraProjMatrix() {
        return mainPlayer_->getProjectionMatrix();
    }

    Matrix4x4 SWorld::getMainPlayerCameraViewMatrix() {
        return mainPlayer_->getViewMatrix();
    }

    Matrix4x4 SWorld::getMainPlayerCameraVPMatrix() {
        return mainPlayer_->getProjectionMatrix() * mainPlayer_->getViewMatrix();
    }

    Transform3D SWorld::getMainPlayerCameraViewTransform() {
        return mainPlayer_->getViewTransform();
    }

    vec3 SWorld::getMainPlayerViewPos() {
        return mainPlayer_->getViewPos();
    }

    UserSetting& SWorld::getUserSetting()
    {
        return setting_;
    }

    SharedPtr<SCameraComponent> SWorld::getCameraCom() {
        return getMainPlayerPtr()->getCameraCom();
    }

    void SWorld::setCameraComRelativePosition(const vec3& vec) {
        getMainPlayerPtr()->getCameraCom()->setRelativePosition(vec);
    }

    void SWorld::tick(float dtime, const SEvent& ev) {
        if(playerControlSystem_)
            playerControlSystem_->tick(dtime, ev);
        SLevel* level = getCurLevel();
        if (level == nullptr) {
            ASSERT(false);
            return;
        }
        level->tick(dtime, ev);
    }

    void SWorld::loadFile(const char* filepath) {

    }
    
}