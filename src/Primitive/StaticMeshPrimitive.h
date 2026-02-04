#ifndef __STATIC_MESH_PRIMITIVE_H
#define __STATIC_MESH_PRIMITIVE_H

#include <vector>
#include "Math/Vector2D.h"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"
#include "Math/Transform3D.h"
#include "Math/Matrix3x3.h"
#include "Math/Matrix4x4.h"
#include "Primitive/PrimitiveDefine.h"
#include "Primitive/VertexBufferLayout.h"

namespace Sun {

    //顶点
    struct Vertex {
        vec3 pos;
        vec3 normal;
        vec4 color;
        vec2 texture;
        //vec2 textures[MAX_TEXCOORDS];
        //vec3 tangentX;
        //vec3 tangentY;
        
        bool operator==(const Vertex& other) const {
            return pos == other.pos && color == other.color &&
                texture == other.texture;
        }

        static VertexBufferLayout getLayout() {
            VertexBufferLayout layout;
            unsigned int stride = sizeof(Vertex);
            layout.push_back({ 0,3,EBasicDataType::FLOAT,false,stride,offsetof(Vertex,pos),0 });
            layout.push_back({ 1,3,EBasicDataType::FLOAT,false,stride, offsetof(Vertex,normal),0 });
            layout.push_back({ 2,4,EBasicDataType::FLOAT,false,stride,offsetof(Vertex,color),0 });
            layout.push_back({ 3,2, EBasicDataType::FLOAT, false, stride, offsetof(Vertex,texture),0 });
            //layout.push_back({ 4,3,EBasicDataType::FLOAT ,false, stride, offsetof(Vertex,tangentX) });
            //layout.push_back({ 5,3,EBasicDataType::FLOAT,false,stride,offsetof(Vertex,tangentY) });         
            return layout;
        }
    };

    using StaticMeshVertex = Vertex;

    struct StaticMeshPrimitive : public StaticMeshVertex {
        static int type_;
    };

    class StaticMeshData {
    public:
        std::vector<StaticMeshVertex> vertices_;
        std::vector<int> indices_;
    };

    //超精简的顶点格式（同时保持可用性) ，用于探索将所有不同材质静态目标合批到一起渲染的模式
    //占用内存大幅减少，流出空间记录matrix索引（材质必须相同，无法将所有材质一次性提交到gpu吧)
    class SimpleVertex {
        vec3 position;
        unsigned char color[4];
        unsigned short normal[2]; //用经纬度表示法向法向，显然三个分量表示法向是冗余的，同时角度无需那么细粒度       
        unsigned short texture[2]; //65536足够采样纹理了
        unsigned short tangent; //切线量只记录一个角度即可
    };

    //实例化相关顶点信息
    struct InstancedVertex {
#if 1
        //for opengl glsl compatile, this matrix should be column major,
        //so you need do a transpose for a row major matrix;
        Matrix4x4 model; 

        static VertexBufferLayout getLayout() {
            VertexBufferLayout layout;
            unsigned int stride = sizeof(InstancedVertex);
            layout.push_back({ 10,4,EBasicDataType::FLOAT,false,stride,0,1 });
            layout.push_back({ 11,4,EBasicDataType::FLOAT,false,stride,sizeof(vec4),1 });
            layout.push_back({ 12,4,EBasicDataType::FLOAT,false,stride,2 * sizeof(vec4),1 });
            layout.push_back({ 13,4,EBasicDataType::FLOAT,false,stride,3 * sizeof(vec4),1 });
            return layout;
        }
#endif
#if 0
        Transform3D tf;
        static VertexBufferLayout getLayout() {
            VertexBufferLayout layout;
            unsigned int stride = sizeof(InstancedVertex);
            layout.push_back({ 7,4,EBasicDataType::FLOAT,false,stride,0,1 });
            layout.push_back({ 8,4,EBasicDataType::FLOAT,false,stride,sizeof(vec4),1 });
            layout.push_back({ 9,4,EBasicDataType::FLOAT,false,stride,2 * sizeof(vec4),1 });
            return layout;
        }
#endif
    };

    struct SimpleInstancedVertex {
        vec3 offset;
        inline static VertexBufferLayout getLayout() {
            VertexBufferLayout layout;
            const unsigned int stride = sizeof(SimpleInstancedVertex);
            layout.push_back({ 9,3,EBasicDataType::FLOAT,false,stride,offsetof(SimpleInstancedVertex,offset),1 });
            return layout;
        }
    };
}


#endif 