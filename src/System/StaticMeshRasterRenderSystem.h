/*****************************************************************************
* @brief : StaticMeshRasterRenderSystem
* 静态网格渲染同道组件 
* 对于不同的渲染管线，不同的组件，需要有一个类做Batching，该类是对
* StaticMesh的光栅化通道的batching
* @author : acedtang
* @date : 2021/3/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __STATIC_MESH_RASTER_RENDER_SYSTEM_H
#define __STATIC_MESH_RASTER_RENDER_SYSTEM_H

#include <vector>
#include "Math/Matrix3x3.h"
#include "Math/Matrix4x4.h"
#include "System/RenderSystem.h"
#include "Render/RenderHandle.h"
#include "Assets/StaticMeshSection.h"

namespace Sun {

    class MeshComponent;
    class StaticMeshRasterRenderSystem : public RenderSystem {
    public:
        StaticMeshRasterRenderSystem() {
            //passType_ = RenderPassComType::RP_STATIC_MESH;
            renderComType_ = ComponentType::COM_STATIC_MESH;
        }
        //will be called on game start
        virtual void prepareRender() override;

        //will be called every frame before all render
        virtual void beginRender() override;

        //will be called every frame between beginRender and endRender
        virtual void render() override;

        //will be called every frame after render
        virtual void endRender() override;

    protected:
    public:
        std::vector<RenderAbleCom*> renderComponents_;

        //for drawing one drawBatch
        std::vector<ShaderProgram> pros_;
        std::vector<std::shared_ptr<MaterialCom>> materialComs_;
        std::vector<GpuVertexDataHandle> renderHandles_;

        //std::vector<Matrix4x4> mvps_;
        //std::vector<Matrix3x3> normalMats_;
        //std::vector<Matrix4x4> models_;
        std::vector<Matrix4x4> views_;
        std::vector<Matrix4x4> projs_;
        std::vector<Vector3D> viewPoses_;

        std::vector<std::shared_ptr<TransformCom>> transformComs_;

        Vector3D viewPos_;

        //for instance rendering
        
        std::vector<Vector3D> offsets_;
    };

}


#endif 