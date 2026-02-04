/*****************************************************************************
* @brief : 裸网格数据
* 该结构是一种精巧设计的紧凑的数据格式，主要为了GPU处理方便，同时兼顾了常规顶点数据的读取；
* 通过data()可以取出连续内存块的数据，根据输入顶点格式不同，采用不同的存储方法，并提供统一接口
* VertexBlockDescription描述了结构的具体信息
* RawMesh
* @author : acedtang
* @date : 2021/3/23
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/

#ifndef __RAW_MESH_H
#define __RAW_MESH_H

#include <vector>
#include <map>
#include "Math/Vector4D.h"
#include "Math/Box.h"
#include "Util/Tree.h"
#include "Joint.h"

namespace Sun {
    
#define MAX_MESH_TEXTURE_COORDS 8
#define NUM_BONES_PER_VEREX 4  //对于模型来说或许小了 FIXME
    //顶点关联的bone信息
    struct VertexBoneInfo {
        unsigned int index[NUM_BONES_PER_VEREX];
        float weights[NUM_BONES_PER_VEREX];
    };

    //内存分布图，一个block
    /*
    block{
      Vector3D vertexPosition;
      Vector4D color * n1;
      Vector3D normal * (0 or 1);
      Vector3D tangentX * d=(0 or 1);
      Vector3D tangentY * d;
      Vector3D textureCoord * n2;
      int    jointIndex *  n3;
      float  weight * n3;
    }
    */

    class RawMeshIter: public VertexBlockDescription
    {
        friend class RawMesh;
    private:
        RawMeshIter();
    public:
        RawMeshIter& operator++();
        RawMeshIter operator++(int);
        RawMeshIter& operator--();
        RawMeshIter operator-- (int);

        char* pointer_;
        //int size_ = 0;
        //int color_offset_ = 0;
        //int normal_offset_ = 0;
        //int tangent_offset_ = 0;
        //int bitangent_offset_ = 0;
        //int textureCoord_offset_ = 0;
        //int jointIndex_offset_ = 0;
        //int weight_offset_ = 0;

        void setVertexPosition(const Vector3D& pos);
        void setColor(const Vector4D& color,int index);
        void setNormal(const Vector3D& normal,int index);
        void setTangentX(const Vector3D& tangentX, int index);
        void setTangentY(const Vector3D& tangentY, int index);
        void setTextureCoord(const Vector3D& texCoord,int index);
        void setJointIndex(int joinIndex, int index);
        void setWeight(float weight, int index);

        Vector3D& getVertexPosition();
        Vector4D& getColor();
        Vector3D& getNormal();
        Vector3D& getTangentX();
        Vector3D& getTangentY();
        Vector3D& getTextureCoord(int index);
        int& getJointIndex(int index);
        float& getJointWeight(int index );
    };

    //for easy to operate,let the members to be public
    class RawMesh {
    public:
        void setDescription(const VertexBlockDescription& discription);
        RawMeshIter begin();
        RawMeshIter end();
        char* appendBlock();
        void resize(int siz);
        const char* data();
        char* operator[](int k);
        int size();
        std::vector<char> meshData_;
        std::vector<unsigned int> indices_;
        //数据块结构类型描述
        VertexBlockDescription blockInfo_; 
        int size_;
        AABB aabbox_;

    protected:
        void updateOffset();
    };

}

#endif