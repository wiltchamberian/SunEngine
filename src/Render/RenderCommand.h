/*****************************************************************************
* @brief : renderData
* 渲染数据
* 在渲染开始阶段会对所有需要渲染的actor,提取renderData
* renderData统一描述了不同格式的渲染数据，渲染数据主要包括其
* 关联的着色器等信息，RenderData并不包含真实数据，只是包含
* 数据引用，真实数据统一由资产管理器来管理
* @author : acedtang
* @date : 2021/4/14
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __RENDER_DATA_H
#define __RENDER_DATA_H

#include "Shader.h"
#include "Primitive/SkeletalMeshPrimitive.h"
#include "Primitive/StaticMeshPrimitive.h"
#include "Component/MaterialCom.h"
#include "Assets/Handle.h"
#include "Assets/Skeleton.h"
#include "Math/Matrix4x4.h"

namespace Sun {

    enum RenderCommandType {
        COMMAND_STATIC_MESH = 0,
        COMMAND_SKELETAL_MESH = 1,
    };

    struct RenderCommandHead {
        //标记数据长度（含头）
        unsigned int len;
        //标记数据类型
        RenderCommandType type;
    };

    struct StaticMeshRenderDataContext {
        //后续改为句柄 FIXME
        StaticMeshData* staticMeshData_;
        THandle<MaterialCom> materialHandle_;
        //可能被许多所共用，考虑优化为指针或句柄形式FIXME
        Matrix4x4 mvp;
        //着色器
        Shader* shader;
    };

    struct StaticMeshRenderData {
        RenderCommandHead head;
        StaticMeshRenderDataContext context;
        StaticMeshRenderData() {
            head.type = RenderCommandType::COMMAND_STATIC_MESH;
            head.len = sizeof(StaticMeshRenderData);
        }
    };

    struct SkeletalMeshRenderDataContext {
        SkeletalMeshData* rawMesh_;
        THandle<MaterialCom> materialHanlde_;
        THandle<Skeleton2> skleton_;
        Shader* shader_;
    };

    struct SkeletalMeshRenderData {
        RenderCommandHead head;
        SkeletalMeshRenderDataContext context;
        SkeletalMeshRenderData() {
            head.type = RenderCommandType::COMMAND_SKELETAL_MESH;
            head.len = sizeof(SkeletalMeshRenderData);
        }
    };
}


#endif
