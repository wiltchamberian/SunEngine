#ifndef __SKELETAL_MESH_PRIMITIVE_H
#define __SKELETAL_MESH_PRIMITIVE_H

#include <memory>
#include <vector>
#include "PrimitiveDefine.h"
#include "Primitive/VertexBufferLayout.h"
#include "Math/Vector2D.h"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"

namespace Sun {

    struct SkeletalMeshVertex {
        vec3 pos;
        vec3 normal;
        vec4 color;
        vec2 textures[MAX_TEXCOORDS];
        vec3 tangentX; 
        vec3 tangentY;
        int           jointIndex[MAX_TOTAL_INFLUENCES];
        float           jointWeight[MAX_TOTAL_INFLUENCES];
        //this is for merge invented by ace
        //int           matrixIndex;

        SkeletalMeshVertex() {
            memset(this, 0, sizeof(*this));
        }

        bool operator==(const SkeletalMeshVertex& other) const {
            return pos == other.pos && color == other.color &&
                textures[0] == other.textures[0];
        }

        static VertexBufferLayout getLayout() {
            VertexBufferLayout layout;
            const unsigned int stride = sizeof(SkeletalMeshVertex);
            layout.push_back({ 0,3,EBasicDataType::FLOAT,false,stride,offsetof(SkeletalMeshVertex,pos) });
            layout.push_back({ 1,3,EBasicDataType::FLOAT,false,stride, offsetof(SkeletalMeshVertex,normal) });
            layout.push_back({ 2,4,EBasicDataType::FLOAT,false,stride,offsetof(SkeletalMeshVertex,color) });
            layout.push_back({ 3,2 * MAX_TEXCOORDS, EBasicDataType::FLOAT, false, stride, offsetof(SkeletalMeshVertex,textures) });
            layout.push_back({ 4,3,EBasicDataType::FLOAT,false,stride,offsetof(SkeletalMeshVertex,tangentX) });
            layout.push_back({ 5,3,EBasicDataType::FLOAT,false,stride,offsetof(SkeletalMeshVertex,tangentY) });
            layout.push_back({ 6,MAX_TOTAL_INFLUENCES, EBasicDataType::INT, false, stride, offsetof(SkeletalMeshVertex,jointIndex) });
            layout.push_back({ 7,MAX_TOTAL_INFLUENCES, EBasicDataType::FLOAT, false, stride, offsetof(SkeletalMeshVertex,jointWeight) });
            return layout;
        }
    };

    struct SkeletalMeshPrimitive :public SkeletalMeshVertex
    {
        static int type_;
    };

    //AOS
    struct SkeletalMeshData {
        std::vector<SkeletalMeshVertex> vertices_;
        std::vector<int> indices_;
    };

    class SkeletalMeshDataSOA {
        //顶点位置数组
        std::vector<vec3> vertexPositions;

        //法线数组
        std::vector<vec3> normals;
        //主切线数组
        std::vector<vec3> tangentX;
        //副切线数组
        std::vector<vec3> tangentY;

        //颜色数组
        std::vector<vec4> colors;

        //纹理坐标数组(为兼容性，采用3d纹理)
        std::vector<vec3> textures[MAX_TEXCOORDS];
        //标记纹理类别
        std::vector<unsigned int> numUVComponents[MAX_TEXCOORDS];

        //顶点索引信息(必然是3的倍数)
        std::vector<unsigned int> indices;

    };
}


#endif 