#include <queue>
#include "Entity/GameObject.h"
#include "Entity/Level.h"
#include "Util/sp.h"
#include "Util/exception.h"
#include "Common/Events.h"
#include "Common/AudioMacros.h"
#include "Component/SimpleTransformCom.h"
#include "Component/LightTransformCom.h"
#include "Audio/Audio.h"
#include "Platform/PlatformMisc.h"
#include "Render/GraphicsLib.h"
#include "Util/uuid.h"

namespace Sun {

    GameObject::GameObject() {
        guid_ = UUID::create();
    }

    GameObject::GameObject(const GameObjectCreateInfo& createInfo) {
        guid_ = UUID::create();
        switch (createInfo.transformType) {
            case SIMPLE_TRANSFORM_COM:
            {
                getComponent<TransformCom>().reset(new SimpleTransformCom);
            }
            break;
            case LIGHT_TRANSFORM_COM: 
            {
                getComponent<TransformCom>().reset(new LightTransformCom);
            }
            break;
            default:
            {
                break;
            }
        }
        if (auto it = getComponent<TransformCom>()) {
            it->setRelativeTransform(createInfo.transform);
        }
        setName(createInfo.name);
        objType_ = createInfo.type;
        return;
    }

    void GameObject::bindTransform() {
        if (auto it = getComponent<TransformCom>()) {
            it->bind(pro_);
        }
        else {
            //assert(false);
        }
    }

    void GameObject::bindTransform(const ShaderProgram& pro) {
        if (auto it = getComponent<TransformCom>()) {
            if (!pro.isValid()) {
                it->bind(pro_);
            }
            else {
                it->bind(pro);
            } 
        }
        else {
            //assert(false);
        }
    }

    void GameObject::bindShaderProgram() {
        if(g_graphicsLib)
            g_graphicsLib->useProgram(pro_);
    }

    void GameObject::setNeedDisplay(bool need) {
        isNeedDisplay_ = need;
    }

    bool GameObject::isNeedDisplay() {
        return isNeedDisplay_;
    }

    void GameObject::prepareRenderAll() {
        travel<GameObject*>(this, [](GameObject* gameobj) {
            gameobj->prepareRender();
            });
    }

    void GameObject::renderAll() {
        travel<GameObject*>(this, [this](GameObject* gameobj) mutable {
            gameobj->render();
            });
    }

    void GameObject::tickAll(float dtime, const SEvent& ev) {
        travel<GameObject*>(this, [dtime, ev](GameObject* gameobj) {
            gameobj->tick(dtime, ev);
            });
    }

    void GameObject::setRigidTransform(const RigidTransform& trans) {
        rigidTransform_ = trans;
    }

    void GameObject::tryMove(const vec3& v) {
        rigidTransform_.translate = v;
    }

    void GameObject::tryRotate(const vec3& axis, float radius) {
        rigidTransform_.qua = Quaternion::fromRotateAxisAndRadius(axis, radius);
    }

    void GameObject::moveRight(float dis, bool updateDescendants)
    {
        if (auto it = getComponent<TransformCom>())
            it->translateRelativeTransform(vec3(dis, 0, 0));
        updateWorldTransform();
        if (updateDescendants) {
            updateDescendantsTransform();
        }
    }

    void GameObject::moveForward(float dis, bool updateDescendants)
    {
        if (auto it = getComponent<TransformCom>())
            it->translateRelativeTransform(vec3(0, 0, -dis));
        updateWorldTransform();
        if (updateDescendants) {
            updateDescendantsTransform();
        }
    }

    void GameObject::moveUp(float dis, bool updateDescendants)
    {
        if (auto it= getComponent<TransformCom>())
            it->translateRelativeTransform(vec3(0, dis, 0));
        updateWorldTransform();
        if (updateDescendants) {
            updateDescendantsTransform();
        }
    }

    void GameObject::move(const vec3& v, bool updateDescendants) {
        if (auto it = getComponent<TransformCom>())
            it->translateRelativeTransform(v);
        updateWorldTransform();
        if (updateDescendants) {
            updateDescendantsTransform();
        }
    }

    void GameObject::move(float x, float y, float z, bool updateDescendants)
    {
        if (auto it = getComponent<TransformCom>())
            it->translateRelativeTransform(vec3(x, y, z));
        updateWorldTransform();
        if (updateDescendants) {
            updateDescendantsTransform();
        }
    }

    void GameObject::moveTo(float x, float y, float z, bool updateDescendants)
    {
        if (auto it = getComponent<TransformCom>())
            it->setRelativeTranslation(vec3(x, y, z));
        updateWorldTransform();
        if (updateDescendants) {
            updateDescendantsTransform();
        }
    }

    void GameObject::rotate(const vec3& axis, float radians, bool updateDescendants)
    {
        if (auto it = getComponent<TransformCom>())
            it->rotateRelativeTransform(axis, radians);
        updateWorldTransform();
        if (updateDescendants) {
            updateDescendantsTransform();
        }
    }

    void GameObject::scale(float scale_x, float scale_y, float scale_z, bool updateDescendants)
    {
        if (auto it = getComponent<TransformCom>()) {
            it->scaleRelativeTransform(vec3(scale_x, scale_y, scale_z));
        } 
        updateWorldTransform();
        if (updateDescendants) {
            updateDescendantsTransform();
        }
    }

    Matrix4x4 GameObject::getWorldMatrix() const
    {
        if (auto it = getComponent<TransformCom>())
            return it->getWorldMatrix();
        return Matrix4x4();
    }

    Transform3D GameObject::getWorldTransform() const
    {
        if (auto it = getComponent<TransformCom>())
            return it->getWorldTransform();
        else
            assert(false);
        return Transform3D();
    }

    void GameObject::setRelativeTransformation(const Transform3D& trans , bool updateDescents)
    {
        if (auto it = getComponent<TransformCom>())
            it->setRelativeTransform(trans);
        updateWorldTransform();    
        if(updateDescents)
            updateDescendantsTransform();
    }

    Transform3D GameObject::getRelativeTransform() const {
        if (auto it = getComponent<TransformCom>())
            return it->getRelativeTransform();
        return Transform3D();
    }

    vec3 GameObject::getPosition() const {
        if (auto it = getComponent<TransformCom>()) {
            return it->getRelativeTransform().getTranslation();
        }
        return vec3(0, 0, 0);
    }

    //该tick只需处理rotor自身的组件，无需处理子rotor相关逻辑
    void GameObject::tick(float dtime,const SEvent& ev) {
        if (auto it = getComponent<MeshInterface>()) {
            it->tick(dtime, ev);
        }
    }

    void GameObject::prepareRender() {

    }

    void GameObject::render() {

    }

    void GameObject::updateDescendantsTransform()
    {
        for (GameObject* it = firstChild_; it != nullptr; it = it->nextSlibing_) {
            travel <GameObject*>(it, [](GameObject* top) {
                top->setWorldTransform(top->parent_->getWorldTransform() * top->getRelativeTransform());
                });
        }
    }

    AABB GameObject::getAABB() const {
        if (auto it = getComponent<BoundingVolumeCom>()) {
            return it->getAABB();
        }
        else {
            //assert(false);
        }
        return AABB();
    }

    AABB GameObject::getExpectDynamicAABB() const {
        AABB cur = getAABB();
        AABB res = cur.expandByTranslate(rigidTransform_.translate);
        return res;
    }

    void GameObject::hitResponse(const HitRecord& record, GameObject* target) {
        if (objType_ == EGameObjectType::STATIC) {
            return;
        }

        auto bvCom = getComponent<BoundingVolumeCom>();
        auto stateCom = getComponent<PhysicalStateCom>();
        if (record.isHit) {
            //这里简单处理，利用触碰点法线和运动矢量做一个反弹，后续细化
            vec3 oldWolrdPos = getComponent<TransformCom>()->getWorldTransform().getTranslation();
            vec3 reflectTranslate = reflect(-rigidTransform_.translate, record.normal) * (1 - record.frac);
            vec3 lastPos = oldWolrdPos + rigidTransform_.translate * record.frac + reflectTranslate;
            vec3 newVelocity = reflect(-stateCom->velocity, record.normal);
   
            move(lastPos - oldWolrdPos);
            stateCom->velocity = newVelocity;

            bvCom->updateBoundingVolume(lastPos - oldWolrdPos);

            if (g_audioEngine) {
                g_audioEngine->play3DAsync(AUDIO_SOLID_COLLISION);
            }
        }
        else {
            move(rigidTransform_.translate);

            bvCom->updateBoundingVolume(rigidTransform_.translate);
        }
        
    }

    void GameObject::detach() {
        detachFrom<GameObject*>(this);
    }

    bool GameObject::supportTransform() const {
        return getComponent<TransformCom>() != nullptr;
    }

    bool GameObject::supportRender() const {
        return getComponent<MeshInterface>() != nullptr;
    }

    bool GameObject::supportPhysics() const {
        return getComponent<PhysicalStateCom>() != nullptr;
    }

    bool GameObject::supportCollide() const {
        return getComponent<BoundingVolumeCom>()!=nullptr;
    }

    bool GameObject::needAddToBVH() const {
        return true;
    }

    bool GameObject::supportCollideQuery() const {
        return false;
    }

    BoundingVolumeType GameObject::getBoundingVolumeType() const {
        if (auto it = getComponent<BoundingVolumeCom>()) {
            return it->getBoundingVolumeType();
        }
        return BoundingVolumeType::BV_NULL;
    }

    void GameObject::setWorldTransform(const Transform3D& transform) {
        if (auto it  =getComponent<TransformCom>()) {
            it->setWorldTransform(transform);
        }
    }

    void GameObject::destroy() {
        detach();
    }

    void GameObject::addChildObj(GameObject* gameobj) {
        addChild<GameObject*>(this, gameobj);
    }

    void GameObject::attachTo(GameObject* gameobj) {
        if (!gameobj)return;
        gameobj->addChildObj(this);
    }

    UUID GameObject::getGuid() const {
        return guid_;
    }

    void GameObject::setName(const std::string& str) {
        name_ = str;
    }

    std::string GameObject::getName() const {
        return name_;
    }

    bool GameObject::needDisplay() {
        return isNeedDisplay_ && pro_.isValid();
    }

    bool GameObject::useInstanceDrawing() const {
        return instanceDraw_;
    }

    void GameObject::setInstanceDrawing(bool use) {
        instanceDraw_ = use;
    }

    void GameObject::updateWorldTransform() {
        if (parent_) {
            setWorldTransform(parent_->getWorldTransform() * getRelativeTransform());
        }
        else {
            setWorldTransform(getRelativeTransform());
        }
    }

    void GameObject::onAddedToLevel() {

    }

    GpuVertexDataHandle GameObject::getVertexDataHandle() {
        if (auto it = getComponent<MeshInterface>())
            return it->getVertexDataHandle();
        else
            assert(false);
        return GpuVertexDataHandle();
    }

    void GameObject::allocGpu(EDrawMode drawType) {
        if (auto it = getComponent<MeshInterface>())
            it->allocGpu(drawType);
    }

    void GameObject::releaseGpu() {
        if (auto it = getComponent<MeshInterface>())
            it->releaseGpu();
    }

    void GameObject::setShaderProgram(const ShaderProgram& pro) {
        pro_ = pro;
    }

    void GameObject::bindUniformData_Material() {
        if (auto it = getComponent<MaterialCom>())
            it->bind(pro_);
    }

    void GameObject::unBindUniformData_Material() {
        if (auto it = getComponent<MaterialCom>())
            it->unBind(pro_);
    }

    void GameObject::bindUniformData() {
        if (auto it = getComponent<MeshComponent>())
            it->bindUniformData(pro_);
    }

    void GameObject::unBindUniformData() {
        if (auto it = getComponent<MeshComponent>())
            it->unBindUniformData(pro_);
    }

    void GameObject::updateVertexData() {
        if (auto it = getComponent<MeshComponent>())
            it->updateVertexData();
    }

    void GameObject::bindVertexData() {
        if (auto it = getComponent<MeshComponent>())
            it->bindVertexData();
    }

    void GameObject::unBindVertexData() {
        if (auto it = getComponent<MeshComponent>())
            it->unBindVertexData();
    }

    void GameObject::drawCall() {
        if (auto it = getComponent<MeshComponent>())
            it->drawCall();
    }

    std::vector<ShaderProgram> GameObject::getMultiShaderPrograms() {
        return {};
    }

    bool GameObject::hasMultiShaderPass() const {
        return false;
    }
}