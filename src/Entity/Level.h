/*****************************************************************************
* @brief : level
* @author : acedtang
* @date : 2021/3/20
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/
#ifndef __METEORGAMEWORLD_H
#define __METEORGAMEWORLD_H

#include <map>
#include <set>
#include <functional>
#include "Entity/Character.h"
#include "Entity/GameObject.h"
#include "Entity/GameObjectContainer.h"
#include "Component/ActorComponent.h"
#include "System/SystemBase.h"
#include "System/StaticMeshRasterDefferedGeoSystem.h"
#include "System/StaticMeshRasterDefferedLightingSystem.h"
#include "Common/Events.h"
#include "Common/MsgCenter.h"
#include "Assets/StaticMesh.h"
#include "ObjectFactory.h"
#include "Util/sp.h"
#include "Util/Guid.h"
#include "Util/GeneralMemoryPool.h"
#include "Primitive/ImageCache.h"
#include "System/RenderPassSystem.h"
#include "System/CollisionDectSystem.h"
#include "Physics/BVH.h"
#include "Entity/DirectLight.h"

namespace Sun
{
    class SWorld;
    class SkeletalRasterRenderPass;   
    class StaticMeshRasterRenderPass; 
    class SLevel
    {
    public:
        SLevel(SWorld* world);
        ~SLevel();
      
        void prepare();
        //reset to the state just like a new SLevel
        void clear();
        void addActor(GameObject* obj ,bool addedToBvh = false);
        void addActors(const std::vector<GameObject*>& actors);
        //void addComponent(SActorComponent* com);
        void addSystem(SystemBase* sys);
        void addSystems(const std::vector<SystemBase*>& vec);
        /*sp<SActor> getObject(const std::string& name);*/
        //this is a interface for thirdparty join up
        int registFuncCalledEveryFrame(std::function<void()> func);
        void tick(float dtime, const SEvent& ev);
        std::map<UUID, GameObject*>& getActors();

        template<class _Actor>
        sp<_Actor> createActor(bool addToLevel = true);

        template<class _Com>
        _Com* createComponent(bool addToLevel = true);

        void prepareRender();

        MsgCenter* getMsgCenter();

        GameObjectContainer& getPhysicalObjects();
        void addPhysicalObject(GameObject*);

        GameObjectContainer& getCollideObjects();
        void addCollideObject(GameObject*);

        GameObjectContainer& getCollideQueryObjects();
        void addCollideQueryObject(GameObject*);

        GameObjectContainer& getLights();
        DirectLight* getDirectLight();

    protected:
        void init(); 
        
        std::map<int, std::function<void()>> _funcMap;

        //gameplay systems 外部系统
        std::vector<SystemBase*> systems_;

        //std::set<RenderAble*> renderAbles_;

        ObjectFactory objFactory_;
        MsgCenter* msgCenter_;
        //TextureCache* textureCache_;

        //std::unique_ptr<SkeletalRasterRenderSystem> skeletalRasterRenderSystem_;
        //std::unique_ptr<Fluid2dSystem> fluid2dSystem_;
        //std::unique_ptr<CommonRenderSystem> commonRenderSystem_;

        //deffered render
        std::unique_ptr<StaticMeshRasterDefferedGeoRenderSystem> staticMeshDefferedGeoSystem_;
        std::unique_ptr<StaticMeshRasterDefferedLightingRenderSystem> staticMeshDefferedLightingSystem_;

        //内建系统
        std::unique_ptr<RenderPassSystem> renderPassSystem_;
        std::unique_ptr<CollisionDectSystem> collisionDectSystem_;

        //所有actors放在这了
        GameObjectContainer actors_;
        //支持渲染的actors
        GameObjectContainer renderObjects_;
        //支持物理仿真操作的gameobject
        GameObjectContainer physicalObjects_;
        //支持碰撞的gameobject (可以简单理解为需要存入bvhTree的objects)
        GameObjectContainer collideObjects_;
        //支持碰撞查询的gameobject(可以简单理解为需要通过bvh查询碰撞的objects,注意
        //这些gameobject可能在，也可能不在bvhTree中
        GameObjectContainer collideQueryObjects_;

        //灯光gameobject
        GameObjectContainer lights_;

        SWorld* world_ = nullptr;
    };

    template<class _Actor>
    sp<_Actor> SLevel::createActor(bool addToLevel) {
        sp<_Actor> s = ActorFactory<_Actor>::get()->malloc();
        if (addToLevel) {
            addObject(s);
        }
        return s;
    }

    template<class _Com>
    _Com* SLevel::createComponent(bool addToLevel) {
        _Com* c = objFactory_.createObj<_Com>();
        if (addToLevel) {
            addComponent(c);
        }
        return c;
    }

}


#endif

