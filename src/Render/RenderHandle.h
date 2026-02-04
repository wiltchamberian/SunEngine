#ifndef __RENDER_HANDLE_H
#define __RENDER_HANDLE_H

#include <memory>
#include "Render/Shader.h"
#include "Render/IndexBuffer.h"
#include "Render/VertexBuffer.h"
#include "Render/VertexArray.h"
#include "Component/MaterialCom.h"
#include "Primitive/Texture.h"
#include "Render/Program.h"
#include "Common/Common.h"
#include "Common/BasicDataType.h"

namespace Sun
{
    //因为渲染信息可能存在gpu可能存在gpu,这里表示gpu端的数据
    struct RenderHandle
    {
        ~RenderHandle() {
            clear();
        }
        
        EPrimitiveType triangleType_ = EPrimitiveType::TRIANGLES;

        std::shared_ptr<VertexArray> vao;
        //std::shared_ptr<VertexBuffer> vbo;
        //std::shared_ptr<IndexBuffer> ebo;
        ShaderProgram pro;
        //材质
        std::shared_ptr<MaterialCom> material_;
        //TextureHandle albedoMap_;
        //TextureHandle metallicMap_;
        //TextureHandle roughnessMap_;
        //TextureHandle normalMap_;
        Texture maps_[PBR_MAP_NUM];

        //for fluid
        Texture hFluidTex;

        //gbuffers
        unsigned int gBuffer = -1;
        unsigned int gPosition = -1;
        unsigned int gNormal = -1;
        unsigned int gAlbedo = -1;
        unsigned int gMRA = -1;

        void clear() {
            vao = nullptr;
            //pro.reset();
            
            //g_graphicsLib->deleteTexture(textureId_);
 
        }

        RenderHandle() {
        }
    };
}


#endif