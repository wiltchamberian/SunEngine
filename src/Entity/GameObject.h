/*****************************************************************************
* @brief : GameObject
* @author : acedtang
* @date : 2021/3/20
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __GAMEOBJECT_H
#define __GAMEOBJECT_H

#include <vector>
#include <array>
#include <memory>
#include "Util/Define.h"
#include "Math/Transform3D.h"
#include "Util/sp.h"
#include "Util/Tree.h"
#include "Util/uuid.h"
#include "Util/base/containers/flat_map.h"
#include "Physics/HitRecord.h"
#include "Common/Events.h"
#include "Common/Define.h"
#include "Meta/ComRegister.h"
#include "Component/TransformCom.h"
#include "Component/MeshInterface.h"
#include "Component/MaterialCom.h"
#include "Component/PhysicalStateCom.h"
#include "Component/BVCom.h"

namespace Sun {

    //class SActor;
    class SLevel;

    enum class EGameObjectType {
        STATIC,//标记该object不会运动
        DYNAMIC,//标记该object会运动
    };

    struct GameObjectCreateInfo {
        std::string name;
        EComType transformType;
        Transform3D transform;
        EGameObjectType type = EGameObjectType::STATIC;
    };

    

    SCLASS()
    class GameObject :public SId,public TreeNodeBase<GameObject*> {
        friend class SRenderHelper;
        friend class SLevel;
        friend class RenderPassSystem;
        friend class GameObjectCompare;

    public:
        //just for test MOVE IT TODO
        AABB lastAABB;
        AABB extendAABB;
        vec3 lastV;

		GameObject();
        GameObject(const GameObjectCreateInfo& createInfo);

        void bindTransform();
        void bindTransform(const ShaderProgram& pro);
        void bindShaderProgram();

        void setNeedDisplay(bool need);
        bool isNeedDisplay();
        //遍历自身及所有子节点调用prepareRender
        void prepareRenderAll();
        void renderAll();
        void tickAll(float dtime, const SEvent& ev);

        void setRigidTransform(const RigidTransform& trans);
        void tryMove(const vec3& v);
        void tryRotate(const vec3& axis, float radius);
        //along the x axis
        void moveRight(float dis ,bool updateDescendants = true);
        //along the -z axis
        void moveForward(float dis ,bool updateDescendants = true);
        //along the y axis
        void moveUp(float dis , bool updateDescendants = true);

        void move(const vec3& v ,bool updateDescendants=true);
        void move(float x, float y, float z ,bool updateDescendants = true);
        void moveTo(float x, float y, float z, bool updateDescendants = true);
        void rotate(const vec3& axis, float radians , bool updateDescendants = true);
        void scale(float scale_x, float scale_y, float scale_z , bool updateDescendants = true);
        Matrix4x4 getWorldMatrix() const;
        Transform3D getWorldTransform() const;
        void setRelativeTransformation(const Transform3D& trans ,bool updateDescents = true);
        Transform3D getRelativeTransform() const;
        vec3 getPosition() const;
        //只需处理自身的组件逻辑，无需处理子gameobject相关逻辑
        virtual void tick(float dtime, const SEvent& ev);
        virtual void prepareRender();
        virtual void render();
        void updateDescendantsTransform();
        AABB getAABB() const;
        AABB getExpectDynamicAABB() const;

        void hitResponse(const HitRecord& record,GameObject* target);
    public:
        void setWorldTransform(const Transform3D& transform);
        
        //将rotor从树结构中解除并销毁
        void destroy();
        void addChildObj(GameObject* obj);
        void attachTo(GameObject* obj);

        UUID getGuid() const;

        void setName(const std::string& str);
        std::string getName() const;

    public: // from RenderAble
        virtual void onAddedToLevel();
        GpuVertexDataHandle getVertexDataHandle();


        void allocGpu(EDrawMode drawType);
        void releaseGpu();
        void setShaderProgram(const ShaderProgram& pro);
        void bindUniformData_Material();
        void unBindUniformData_Material();
        virtual void bindUniformData();
        virtual void unBindUniformData();
        void updateVertexData();
        void bindVertexData();
        void unBindVertexData();
        void drawCall();
        std::vector<ShaderProgram> getMultiShaderPrograms();
        bool hasMultiShaderPass() const;
        inline ShaderProgram getShaderProgram() const {
            return pro_;
        }
        unsigned int getShaderProgramId() const {
            return pro_.getId();
        }

        inline void setVertexDataHandle(const GpuVertexDataHandle& handle) {
      /*      if (components_.find(ComTag<MeshInterface>::tag)>0) {*/
                std::dynamic_pointer_cast<MeshInterface>(components_[ComTag<MeshInterface>::tag])->setVertexDataHandle(handle);
            /*}*/
        }

        inline friend bool operator< (GameObject& m1, GameObject& m2) {
            if (m1.getShaderProgram() < m2.getShaderProgram()) return true;
            else if (m1.getShaderProgram() > m2.getShaderProgram()) return false;
            if (*m1.getComponent<MeshInterface>() < *m2.getComponent<MeshInterface>()) return true;
            else if (*m2.getComponent<MeshInterface>() < *m1.getComponent<MeshInterface>()) return false;

            return false;
        }
        void setFaceCull(bool cull) { faceCull_ = cull; }
        bool getFaceCull() { return faceCull_; }
        void setCulled(bool culled) { culled_ = culled; }
        bool getCulled() { return culled_; }
        inline bool needDisplay();

        bool useInstanceDrawing() const;
        void setInstanceDrawing(bool use);
    protected:
        inline void updateWorldTransform();
        //将rotor从树结构中解除
        void detach();

        
        ShaderProgram pro_;

        SLevel* level_ = nullptr;
        UUID guid_;
        //标记其在GameObjectConatiner中的位置,当将其存入GameObjectContainer中时赋值
        void* containerPos_=nullptr;
    public:
        bool supportTransform() const;
        bool supportRender() const;
        bool supportPhysics() const;
        virtual bool supportCollide() const;
        virtual bool needAddToBVH() const;
        virtual bool supportCollideQuery() const;
        BoundingVolumeType getBoundingVolumeType() const;
		//SharedPtr<TransformCom> transformCom_;
  //      SharedPtr<MeshInterface> meshCom_;
  //      SharedPtr<MaterialCom> materialCom_;
  //      SharedPtr<PhysicalStateCom> stateCom_;
  //      SharedPtr<BoundingVolumeCom> bvCom_;
        //标记无碰撞状态下的预期单帧位移旋转矢量
        RigidTransform rigidTransform_;

        template<class _T ,class ..._Args>
        SharedPtr<_T> addComponent(_Args... args) {
            static_assert(ComTag<_T>::tag != static_cast<uint16>(-1));

            //auto it = components_.find(ComTag<_T>::tag);
            //if (it != components_.end()) return it->second;
            //SharedPtr<_T> ptr(new _T(::std::forward<_Args>(args)));
            //components_.insert(std::make_pair(ComTag<_T>::tag, ptr));
            //return ptr;

            SharedPtr<_T> ptr(new _T(::std::forward<_Args>(args)));
            components_[ComTag<_T>::tag] = ptr;
        }

        template<class _T>
        void setComponent(const SharedPtr<_T>& com) {
            static_assert(ComTag<_T>::tag != static_cast<uint16>(-1));
            components_[ComTag<_T>::tag] = com;
        }

        //template<class _T>
        //SharedPtr<_T> setComponent(_T* ptr) {
        //    components_[ComTag<_T>::tag].reset(SharedPtr<_T>(ptr));
        //    return std::dynamic_pointer_cast<_T>(components_[ComTag<_T>::tag]);
        //}

        template<class _T>
        FORCE_INLINE SharedPtr<_T> getComponent() const {
            static_assert(ComTag<_T>::tag != static_cast<uint16>(-1));

            //auto it = components_.find(ComTag<_T>::tag);
            //if (it != components_.end()) return std::dynamic_pointer_cast<_T>(it->second);
            //return nullptr;

            return std::static_pointer_cast<_T>(components_[ComTag<_T>::tag]);
        }

        //this should be revised to flat_map!
        //std::unordered_map<uint16, SharedPtr<IComponent>> components_;
        std::array<SharedPtr<IComponent>,8> components_;

        //标记其在BVH_Tree中的位置
        int32 proxyId_;
        EGameObjectType objType_ = EGameObjectType::STATIC;
    protected:
        bool isNeedDisplay_ = true;
        bool faceCull_ = true; //面剔除
        bool culled_ = false; //是否被剔除渲染（视锥体剔除，遮挡剔除等)
        //实例化渲染
        bool instanceDraw_ = false;
        
        std::string name_;
    };

    struct GameObjectCompare {
    public:
        FORCE_INLINE bool operator() (GameObject* p1, GameObject* p2) const {
#if 1
            if (p1->pro_.id_ < p2->pro_.id_) return true;
            else if (p2->pro_.id_ < p1->pro_.id_) return false;
            else if (p1->components_[ComTag<MaterialCom>::tag] < p2->components_[ComTag<MaterialCom>::tag]) return true;
            else if (p2->components_[ComTag<MaterialCom>::tag] < p1->components_[ComTag<MaterialCom>::tag]) return false;
            else if (p1->components_[ComTag<MeshInterface>::tag] < p2->components_[ComTag<MeshInterface>::tag]) return true;
            return false;
#endif 
            //相比上面的比较运算，在10000次数量级的比较下，该方法耗时大约是上者的50倍，
            //以下函数都内联，为何差异会这么大?
#if 0
            if (p1->getShaderProgram() < p2->getShaderProgram()) return true;
            else if (p2->getShaderProgram() < p1->getShaderProgram()) return false;
            else if (p1->getMaterialHandle() < p2->getMaterialHandle()) return true;
            else if (p2->getMaterialHandle() < p1->getMaterialHandle()) return false;
            else if (p1->getMeshInterface() < p2->getMeshInterface()) return true;
            return false;
#endif
        }

        bool equal(GameObject* p1, GameObject* p2) const {
            return (p1->pro_.id_ == p2->pro_.id_)
                && (p1->getComponent<MaterialCom>() == p2->getComponent<MaterialCom>())
                && (p1->getComponent<MeshInterface>() == p2->getComponent<MeshInterface>());
        }
    };
}


#endif