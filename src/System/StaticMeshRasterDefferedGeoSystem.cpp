#include "StaticMeshRasterDefferedGeoSystem.h"
#include "Assets/AssetsMgr.h"
#include "Primitive/StaticMeshPrimitive.h"
#include "Engine.h"
#include "Render/ShaderManager.h"
#include "Render/GraphicsProxy.h"
#include "Render/GraphicsLib.h"

namespace Sun {

	//will be called on game start
	void StaticMeshRasterDefferedGeoRenderSystem::prepareRender()
	{
	}


	//will be called every frame before all render
	void StaticMeshRasterDefferedGeoRenderSystem::beginRender() {
#if 0
        std::vector<StaticMeshVertex> datas;
        //auto assetsMgr = Singleton<AssetsMgr>::get()->getMgr<StaticMesh>();
        sp<SCameraRotor> camera = SUN->getWorld()->getUsingCamera();
        Matrix4x4 view = camera->worldTransform_.toMatrix().getInverse();
        Matrix4x4 p //auto materialMgr = Singleton<AssetsMgr>::get()->getMgr<Material>();
       roj = camera->calculateProjectionMatrix();
        viewPos_ = camera->getViewPos();
        for (int i = 0; i < meshComponents_.size(); ++i) {
            if (g_renderManager && g_graphicsProxy) {
                Matrix4x4 model = meshComponents_[i]->owner_->worldMatrix();
                Matrix4x4 mvp = proj * view * model;
                Matrix3x3 normalMat = (view * model).toMatrix3x3().getInverse().getTranspose();
                std::vector<StaticMeshVertex>& vec = meshComponents_[i]->vertices_;
                RenderHandle handle;
                if (numInstance_<=0) {
                    handle = g_graphicsProxy->commitStaticMeshDataToGPU(vec.data(), vec.size() * sizeof(StaticMeshVertex), meshComponents_[i]->indices_.data(), meshComponents_[i]->indices_.size() * sizeof(int), g_renderManager->staticMeshProgram_);
                }
                else {
                    handle = g_graphicsProxy->commitStaticMeshInstanceDataToGPU(vec.data(), vec.size() * sizeof(StaticMeshVertex), meshComponents_[i]->indices_.data(), meshComponents_[i]->indices_.size() * sizeof(int), g_renderManager->staticMeshProgram_,
                        offsets_.data(), numInstance_ * sizeof(vec3));
                }
                handle.triangleType_ = meshComponents_[i]->triangleType_;

                //gbuffer
                //setupGBuffer(info.width,info.height);

                //Material* material = materialMgr.dereference(sec.materialHandle_);
                extractingMaterialInfo(handle, meshComponents_[i]->getMaterialHandle());

                renderHandles_.push_back(handle);

                mvps_.push_back(mvp);
                normalMats_.push_back(normalMat);
                models_.push_back(model);

            }
        }
#endif
	}

	//will be called every frame between beginRender and endRender
	void StaticMeshRasterDefferedGeoRenderSystem::render() {

	}

	//will be called every frame after render
	void StaticMeshRasterDefferedGeoRenderSystem::endRender() {

	}

}
