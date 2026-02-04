#include "RenderAbleCom.h"
#include "MeshComponent.h"
#include "SimpleMaterialCom.h"
#include "PbrMaterialCom.h"
#include "Render/GraphicsLib.h"
#include "Entity/GameObject.h"
#include <cassert>

namespace Sun {

    RenderAbleCom::RenderAbleCom() {

    }

    RenderAbleCom::RenderAbleCom(const RenderAbleCreateInfo& createInfo) {
        switch (createInfo.meshType) {
            case ComType::MESH_COMPONENT:
            {
                meshCom_.reset(new MeshComponent);
            }
            break;
            default:
                break;
        }
        switch (createInfo.materialType) {
            case ComType::SIMPLE_MATERIAL_COM:
            {
                materialCom_.reset(new SimpleMaterialCom());
            }
            break;
            case ComType::PBR_MATERIAL_COM:
            {
                materialCom_.reset(new PbrMaterialCom());
            }
            break;
            default:
                break;
        }
    }

    RenderAbleCom::RenderAbleCom(const std::shared_ptr<MeshInterface>& mesh, const std::shared_ptr<MaterialCom>& material) {
        init(mesh, material);
    }

    void RenderAbleCom::onAddedToLevel() {
        if (meshCom_)
            meshCom_->onAddedToLevel(this);
    }

    void RenderAbleCom::tick(float dtime, const SEvent& ev) {
        if (meshCom_)
            meshCom_->tick(dtime, ev);
    }

    void RenderAbleCom::prepareRender() {

    }

    void RenderAbleCom::beginRender() {

    }

    void RenderAbleCom::render() {

    }

    void RenderAbleCom::endRender() {

    }

    GpuVertexDataHandle RenderAbleCom::getVertexDataHandle() {
        if (meshCom_)
            return meshCom_->getVertexDataHandle();
        else
            assert(false);
        return GpuVertexDataHandle();
    }

    uint32_t RenderAbleCom::getInterfaceFlag() {
        return IFlag_RenderAble;
    }

    void RenderAbleCom::setMeshInterface(const std::shared_ptr<MeshInterface>& mesh) {
        meshCom_ = mesh;
    }

    void RenderAbleCom::allocGpu(S_TYPE drawType) {
        if (meshCom_)
            meshCom_->allocGpu(drawType);
    }

    void RenderAbleCom::releaseGpu() {
        if (meshCom_)
            meshCom_->releaseGpu();
    }

    void RenderAbleCom::setShaderProgram(const ShaderProgram& pro) {
        pro_ = pro;
    }

    void RenderAbleCom::bindUniformData_Transform(const Matrix4x4& view, const Matrix4x4& proj, const Matrix4x4& vp, const Vector3D& cameraPos) {
        if (owner_) {
            owner_->bindUniformData_Transform(view, proj, vp, cameraPos, pro_);
        }
    }

    void RenderAbleCom::unBindUniformData_Transform() {

    }

    void RenderAbleCom::bindUniformData_Material() {
        if(materialCom_)
            materialCom_->bind(pro_);
    }

    void RenderAbleCom::unBindUniformData_Material() {
        if (materialCom_)
            materialCom_->unBind(pro_);
    }

    void RenderAbleCom::bindUniformData() {
        if (meshCom_)
            meshCom_->bindUniformData(pro_);
    }

    void RenderAbleCom::unBindUniformData() {
        if (meshCom_)
            meshCom_->unBindUniformData(pro_);
    }

    void RenderAbleCom::updateVertexData() {
        if (meshCom_)
            meshCom_->updateVertexData();
    }

    //绑定顶点数据阶段 (不同组件的顶点格式不同)
    void RenderAbleCom::bindVertexData() {
        if (meshCom_)
            meshCom_->bindVertexData();
    }

    void RenderAbleCom::drawCall() {
        if (meshCom_)
            meshCom_->drawCall();
    }

    void RenderAbleCom::unBindVertexData() {
        if (meshCom_)
            meshCom_->unBindVertexData();
    }

    //for Multi shaderPass objPass ,if reimplemnt this function,
    //the reuturn vector should include shaderProgram_ and
    //shaderProgram_ must be the first;
    std::vector<ShaderProgram> RenderAbleCom::getMultShaderPass() {
        return {};
    }

    bool RenderAbleCom::hasMultiShaderPass() const {
        return false;
    }

    ShaderProgram RenderAbleCom::getShaderProgram() const {
        return pro_;
    }

    unsigned int RenderAbleCom::getShaderProgramId() const {
        return pro_.getId();
    }

    void RenderAbleCom::setMaterialHandle(const std::shared_ptr<MaterialCom>& ptr) {
        materialCom_ = ptr;
    }

    std::shared_ptr<MaterialCom> RenderAbleCom::getMaterialHandle() const {
        return materialCom_;
    }

    void RenderAbleCom::init(const std::shared_ptr<MeshInterface>& mesh, const std::shared_ptr<MaterialCom>& material) {
        meshCom_ = mesh;
        materialCom_ = material;
    }
}