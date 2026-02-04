#include "StaticMeshRasterRenderSystem.h"
#include "Primitive/StaticMeshPrimitive.h"
#include "Component/RenderAbleCom.h"
#include "Assets/AssetsMgr.h"
#include "Rotor/CameraRotor.h"
#include "Render/RenderManager.h"
#include "Render/GraphicsProxy.h"
#include "Render/GraphicsLib.h"
#include "Render/GraphicsCache.h"
#include "Render/RenderHandle.h"
#include "Engine.h"
#include "Util/exception.h"

namespace Sun {
    //will be called on game start
    void StaticMeshRasterRenderSystem::prepareRender() {

    }

    //batching,古典的合批方法是将材质相同的静态纹理合并到一起，
    //对于动态目标，因为纹理和transform都可能变化，
    void StaticMeshRasterRenderSystem::beginRender() {
        std::vector<StaticMeshVertex> datas;
        SWorld* world = SUN->getWorld();

#if 1
        Matrix4x4 view = world->getMainPlayerCameraViewMatrix();
        Matrix4x4 proj = world->getMainPlayerCameraProjMatrix();
        viewPos_ = world->getMainPlayerViewPos();
#endif

        for (int i = 0; i < renderComponents_.size(); ++i) {
            //StaticMesh* mesh = assetsMgr.dereference(meshComponents_[i]->mesh_);
            RenderAbleCom* com = renderComponents_[i];

            if (com) {
                Matrix4x4 model = renderComponents_[i]->owner_->worldMatrix();
                Matrix4x4 mvp = proj * view * model;
                Matrix3x3 normalMat = (view * model).toMatrix3x3().getInverse().getTranspose();                    
                GpuVertexDataHandle handle; 
                           
                auto meshCom = com->getMeshCom<MeshComponent>();
                if (numInstance_ <= 0) {
                    meshCom->allocGpu();
                }
                else {
                    //commitDataInstance();
                }
                handle = meshCom->getVertexDataHandle();
                //handle.triangleType_ = renderComponents_[i]->getMeshCom()->getPrimitiveType();

                //Material* material = materialMgr.dereference(sec.materialHandle_);
                //extractingMaterialInfo(handle, renderComponents_[i]->getMaterialHandle());
                materialComs_.push_back(com->getMaterialHandle());
                    
                renderHandles_.push_back(handle);
                pros_.push_back(com->getShaderProgram());

                /*mvps_.push_back(mvp);
                normalMats_.push_back(normalMat);
                models_.push_back(model);*/
                views_.push_back(view);
                projs_.push_back(proj);
                transformComs_.push_back(com->getTransformCom());
            }
        }


    }

    //will be called every frame between beginRender and endRender
    void StaticMeshRasterRenderSystem::render() {
        if (g_graphicsLib) {
            for (int i = 0; i < renderHandles_.size(); ++i) {
                // one drawCall
                GpuVertexDataHandle& handle = renderHandles_[i];
                ShaderProgram pro = pros_[i];
                std::shared_ptr<MaterialCom> material = materialComs_[i];

                g_graphicsLib->useProgram(pro);
                //#define GL_FRAMEBUFFER 0x8D40 FIXME
                //g_graphicsLib->bindFrameBuffer(0x8D40,g_graphicsProxy->gbuf_.gBuffer);
                //g_graphicsLib->clear({ 0,0,0,1 });

                //bindMatrices(viewPos_, mvps_[i], normalMats_[i], models_[i], pro);
                transformComs_[i]->bind(views_[i], projs_[i], viewPos_, pro);
                
                //bindMaterial(handle,pro);
                material->bind(pro);

                //drawCall(handle, pro, numInstance_);
                g_graphicsLib->drawCall(handle);
                
                //解绑
                //unBindMaterial(handle, pro);
                material->unBind(pro);

                //g_graphicsLib->bindFrameBuffer(0x8D40, 0);
            }
        }
    }

    void StaticMeshRasterRenderSystem::endRender() {
        if (g_graphicsLib) {
            renderComponents_.clear();
            for (int i = 0; i < renderHandles_.size(); ++i) {
                GpuVertexDataHandle& handle = renderHandles_[i];
                for (int j = 0; j < PBR_MAP_NUM; ++j) {
                    if (renderHandles_[i].maps_[j].handle_ >= 0) {
                        //g_graphcisCache->unUseTexture(renderHandles_[i].maps_[j].textureId_);
                        //g_graphicsLib->deleteTexture();
                        g_graphicsCache->applyForReleaseTexture(renderHandles_[i].maps_[j].source_);
                    }
                }
            }
            renderHandles_.clear();
            mvps_.clear();
            models_.clear();
            normalMats_.clear();
            viewPos_.clear();
        }
    }
}