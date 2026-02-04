#include "Level.h"
#include "Util/uuid.h"
#include "Render/GraphicsLib.h"
#include "Render/ShaderManager.h"
#include "Common/Common.h"
#include "Component/SkeletalMeshComponent.h"
#include "Component/MeshComponent.h"
#include "Common/MsgCenter.h"
#include "System/StaticMeshRasterDefferedLightingSystem.h"
#include "Platform/PlatformMisc.h"
#include "Platform/PlatformIO.h"
#include "Entity/World.h"
#include "Entity/Light.h"

namespace Sun
{
    SLevel::SLevel(SWorld* world)
    {
        world_ = world;
        init();
    }

    SLevel::~SLevel()
    {
        clear();
    }

    void SLevel::prepare() {
        //内建系统准备
        renderPassSystem_->prepare();
        collisionDectSystem_->prepare();

        //gameplay系统准备
        for (auto& it : systems_) {
            it->prepare();
        }
    }

    void SLevel::clear() {
        //TODO
        delete msgCenter_;
        //delete textureCache_;
    }

    //TODO 添加子级obj
    void SLevel::addActor(GameObject* obj ,bool addToBVH)
    {
        if (obj == nullptr) {
            assert(false);
            return;
        }

        actors_.push_back(obj);
        //obj->containerPos_ =  actors_.push_back(obj);
        

        //renderPassSystem_->addGameObject(obj);

        if (obj->supportRender()) {
            renderObjects_.push_back(obj);
        }
        //if (obj->supportCollide()) {
        //    collideObjects_.push_back(obj);    
        //}
        if (addToBVH) {
            collisionDectSystem_->addGameObject(obj);
        }
        if (obj->supportCollideQuery()) {
            collideQueryObjects_.push_back(obj);
        }
        if (obj->supportPhysics()) {
            physicalObjects_.push_back(obj);
        }
        Light* light = dynamic_cast<Light*>(obj);
        if (light !=nullptr) {
            lights_.push_back(light);
        }
    }

    void SLevel::addActors(const std::vector<GameObject*>& actors) {
        for (int i = 0; i < actors.size(); ++i) {
            addActor(actors[i]);
        }
    }

    void SLevel::addSystem(SystemBase* sys) {
        systems_.push_back(sys);
    }

    void SLevel::addSystems(const std::vector<SystemBase*>& vec) {
        systems_.insert(systems_.end(), vec.begin(), vec.end());
    }

    int SLevel::registFuncCalledEveryFrame(std::function<void()> func)
    {
        static int number = 0;
        _funcMap.insert(std::make_pair(number, func));
        number++;
        return number - 1;
    }

    void SLevel::init() {
        msgCenter_ = new MsgCenter();

        staticMeshDefferedLightingSystem_.reset(new StaticMeshRasterDefferedLightingRenderSystem(world_));
        renderPassSystem_.reset(new RenderPassSystem(world_));
        collisionDectSystem_.reset(new CollisionDectSystem(world_));
    }

    void SLevel::tick(float dtime ,const SEvent& ev) {

        for (int i = 0; i < systems_.size(); ++i) {
            systems_[i]->tick(dtime, ev);
        }

        for (auto& it: actors_) {
            it.second->tick(dtime, ev);
        }
        
        if (collisionDectSystem_) {
            collisionDectSystem_->tick(dtime, ev);
        }

        msgCenter_->onNotifyComponentAddedToLevel();

        renderPassSystem_->tick(dtime,ev);

        g_platformIO->swapBuffer();
    }

    std::map<UUID,GameObject*>& SLevel::getActors() {
        return actors_;
    }

    void SLevel::prepareRender() {
        //g_graphicsProxy->recreateGBuffer(info.width, info.height);

        for (auto& it : actors_) {
            it.second->prepareRenderAll();
        }
        //for (auto& it : renderAbles_) {
        //    it->prepareRender();
        //}

    }

    MsgCenter* SLevel::getMsgCenter() {
        return msgCenter_;
    }

    GameObjectContainer& SLevel::getPhysicalObjects() {
        return physicalObjects_;
    }

    void SLevel::addPhysicalObject(GameObject* obj) {
        physicalObjects_.push_back(obj);
    }

    GameObjectContainer& SLevel::getCollideObjects() {
        return collideObjects_; 
    }

    void SLevel::addCollideObject(GameObject* obj) {
        collideObjects_.push_back(obj);
    }

    GameObjectContainer& SLevel::getCollideQueryObjects() {
        return collideQueryObjects_;
    }

    void SLevel::addCollideQueryObject(GameObject* obj) {
        collideQueryObjects_.push_back(obj);
    }

    GameObjectContainer& SLevel::getLights() {
        return lights_;
    }

    DirectLight* SLevel::getDirectLight() {
        for (auto& it : lights_) {
            if (dynamic_cast<DirectLight*>(it.second) != nullptr) {
                return dynamic_cast<DirectLight*>(it.second);
            }
        }
        return nullptr;
    }

}

