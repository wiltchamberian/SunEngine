/*****************************************************************************
* @brief : vertex
* 设计了若干种比较有代表性的顶点结构体，
* 实际导入数据时直接复用对应类型结构体，从而提高转换效率
* 主要用于GPU渲染，格式与对应的着色器匹配
* @author : acedtang
* @date : 2021/3/20
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __VERTEX_H
#define __VERTEX_H

#include "Math/Vector3D.h"
#include "Math/Vector4D.h"

namespace Sun {

//这些宏定义的值必须和RawMesh数据结构保持一致

#define VECTOR3D_LEN 12
#define VECTOR4D_LEN 16
#define VECTOR6D_LEN 24
#define JOINT_INDEX_LEN 4
#define WEIGHT_LEN 4
#define JOINT_INDEX_AND_WEIGHT_LEN (JOINT_INDEX_LEN+WEIGHT_LEN)
#define BASIC_LEN 64

#define VERTEXPOS_OFFSET 0
#define COLOR_OFFSET 12
#define NORMAL_OFFSET 28
#define TANGENTX_OFFSET 40
#define TANGENTY_OFFSET 52
#define TEXTURECOORD_OFFSET 64

    //类型模板
    struct VertexBlockDescription {
        inline int updateSize() {
            color_offset_ = VECTOR3D_LEN;
            normal_offset_ = color_offset_ + numColorChannels * VECTOR4D_LEN;
            tangent_offset_ = normal_offset_ + numNormals * VECTOR3D_LEN;
            bitangent_offset_ = tangent_offset_ + numTangents * VECTOR3D_LEN;
            textureCoord_offset_ = bitangent_offset_ + numBitangents * VECTOR3D_LEN;
            jointIndex_offset_ = textureCoord_offset_ + numUVChannels * VECTOR3D_LEN;
            weight_offset_ = jointIndex_offset_ + numWeights * sizeof(int);

            size = VECTOR3D_LEN + numColorChannels * VECTOR4D_LEN + numUVChannels * VECTOR3D_LEN + numWeights * JOINT_INDEX_AND_WEIGHT_LEN
                + numTangents * VECTOR3D_LEN +numBitangents * VECTOR3D_LEN + numNormals * VECTOR3D_LEN;
            return size;
        }
        int numUVChannels = 0;
        int numWeights = 0;
        int numColorChannels = 0;
        int numTangents = 0;
        int numBitangents = 0;
        int numNormals = 0;
        int size = 0;

        int color_offset_ = 0;
        int normal_offset_ = 0;
        int tangent_offset_ = 0;
        int bitangent_offset_ = 0;
        int textureCoord_offset_ = 0;
        int jointIndex_offset_ = 0;
        int weight_offset_ = 0;
    };

    template<typename _Type>
    VertexBlockDescription typeToDescription() {
        _Type type;
        return { type.m, type.n, type.t };
    }

 /*   template<int M, int N>
    struct VertexType {
        int m = M;
        int n = N;
        int t = sizeof(Vertex<M, N>)
    };*/

    template<class _T>
    int getStructLen(const _T& t) {
        return sizeof(t);
    }

    template<int M, int N>
    struct Vertex {
        //typedef VertexType<M, N> type;
        Vector3D vertexPosition;
        Vector4D color;
        Vector3D normal;
        Vector3D tangentX;
        Vector3D tangentY;
        Vector3D textureCoord[M];
        int           jointIndex[N];
        float         weight[N];
    };

    template<int M>
    struct Vertex<M, 0>
    {
        Vector3D vertexPosition;
        Vector4D color;
        Vector3D normal;
        Vector3D tangentX;
        Vector3D tangentY;
        Vector3D textureCoord[M];
    };

    template<int N>
    struct Vertex<0, N>
    {
        Vector3D vertexPosition;
        Vector4D color;
        Vector3D normal;
        Vector3D tangentX;
        Vector3D tangentY;
        Vector3D textureCoord[N];
    };

    template<>
    struct Vertex<0, 0>
    {
        Vector3D vertexPosition;
        Vector4D color;
        Vector3D normal;
        Vector3D tangentX;
        Vector3D tangentY;
    };

#if 0
    //静态网格顶点,_x后缀表示纹理坐标个数
    struct Vertex_Static_0 {
        Vector3D vertexPosition;
        Vector4D color;
        Vector3D normal;
        Vector3D tangentX;
        Vector3D tangentY;
    };
    struct Vertex_Static_1 {
        Vector3D vertexPosition;
        Vector4D color;
        Vector3D normal;
        Vector3D tangentX;
        Vector3D tangentY;
        Vector3D textureCoord;
    };
    struct Vertex_Static_2 {
        Vector3D vertexPosition;
        Vector4D color;
        Vector3D normal;
        Vector3D tangentX;
        Vector3D tangentY;
        Vector3D textureCoord[2];
    };
    struct Vertex_Static_4{
        Vector3D vertexPosition;
        Vector4D color;
        Vector3D normal;
        Vector3D tangentX;
        Vector3D tangentY;
        Vector3D textureCoord[4];
    };
    struct Vertex_Static_8 {
        Vector3D vertexPosition;
        Vector4D color;
        Vector3D normal;
        Vector3D tangentX;
        Vector3D tangentY;
        Vector3D textureCoord[8];
    };

    //骨骼蒙皮顶点  _x_y表示纹理坐标个数和权值个数
    struct Vertex_Skinned_1_1 {
        Vector3D vertexPosition;
        Vector4D color;
        Vector3D normal;
        Vector3D tangentX;
        Vector3D tangentY;
        Vector3D textureCoord;
        int           index;
        float         weight;
    };

    struct Vertex_Skinned_2_1 {
        Vector3D vertexPosition;
        Vector4D color;
        Vector3D normal;
        Vector3D tangentX;
        Vector3D tangentY;
        Vector3D textureCoord[2];
        int           index;
        float         weight;
    };

    struct Vertex_Skinned_4_1 {
        Vector3D vertexPosition;
        Vector4D color;
        Vector3D normal;
        Vector3D tangentX;
        Vector3D tangentY;
        Vector3D textureCoord[4];
        int           index;
        float         weight;
    };

    struct Vertex_Skinned_8_1 {
        Vector3D vertexPosition;
        Vector4D color;
        Vector3D normal;
        Vector3D tangentX;
        Vector3D tangentY;
        Vector3D textureCoord[8];
        int           index;
        float         weight;
    };

    struct Vertex_Skinned_8_2 {
        Vector3D vertexPosition;
        Vector4D color;
        Vector3D normal;
        Vector3D tangentX;
        Vector3D tangentY;
        Vector3D textureCoord[8];
        int           index[2];
        float         weight[2];
    };

    struct Vertex_Skinned_8_4 {
        Vector3D vertexPosition;
        Vector4D color;
        Vector3D normal;
        Vector3D tangentX;
        Vector3D tangentY;
        Vector3D textureCoord[8];
        int           index[4];
        float         weight[4];
    };

    struct Vertex_Skinned_8_8 {
        Vector3D vertexPosition;
        Vector4D color;
        Vector3D normal;
        Vector3D tangentX;
        Vector3D tangentY;
        Vector3D textureCoord[8];
        int           index[8];
        float         weight[8];
    };

    struct Vertex_Skinned_8_16 {
        Vector3D vertexPosition;
        Vector4D color;
        Vector3D normal;
        Vector3D tangentX;
        Vector3D tangentY;
        Vector3D textureCoord[8];
        int           index[8];
        float         weight[16];
    };
#endif
}

#endif