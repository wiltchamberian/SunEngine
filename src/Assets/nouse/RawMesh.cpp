#include "RawMesh.h"

namespace Sun {



    void RawMesh::setDescription(const VertexBlockDescription& discription)
    {
        blockInfo_ = discription;
    }

    RawMeshIter RawMesh::begin()
    {
        RawMeshIter iter;
        iter.pointer_ = &(*meshData_.begin());
        iter.size = blockInfo_.size;
        iter.color_offset_ = blockInfo_.color_offset_;
        iter.normal_offset_ = blockInfo_.normal_offset_;
        iter.tangent_offset_ = blockInfo_.tangent_offset_;
        iter.bitangent_offset_ = blockInfo_.bitangent_offset_;
        iter.textureCoord_offset_ = blockInfo_.textureCoord_offset_;
        iter.jointIndex_offset_ = blockInfo_.jointIndex_offset_;
        iter.weight_offset_ = blockInfo_.weight_offset_;
        iter.numColorChannels = blockInfo_.numColorChannels;
        iter.numUVChannels = blockInfo_.numUVChannels;
        iter.numWeights = blockInfo_.numWeights;
        iter.numNormals = blockInfo_.numNormals;
        iter.numTangents = blockInfo_.numTangents;
        iter.numBitangents = blockInfo_.numBitangents;

        return iter;
    }

    RawMeshIter RawMesh::end()
    {
        RawMeshIter iter;
        iter.pointer_ = &(*meshData_.begin());
        iter.size = blockInfo_.size;
        iter.color_offset_ = blockInfo_.color_offset_;
        iter.normal_offset_ = blockInfo_.normal_offset_;
        iter.tangent_offset_ = blockInfo_.tangent_offset_;
        iter.bitangent_offset_ = blockInfo_.bitangent_offset_;
        iter.textureCoord_offset_ = blockInfo_.textureCoord_offset_;
        iter.jointIndex_offset_ = blockInfo_.jointIndex_offset_;
        iter.weight_offset_ = blockInfo_.weight_offset_;
        iter.numColorChannels = blockInfo_.numColorChannels;
        iter.numUVChannels = blockInfo_.numUVChannels;
        iter.numWeights = blockInfo_.numWeights;
        iter.numNormals = blockInfo_.numNormals;
        iter.numTangents = blockInfo_.numTangents;
        iter.numBitangents = blockInfo_.numBitangents;

        return iter;
    }

    RawMeshIter::RawMeshIter()
    {

    }

    RawMeshIter& RawMeshIter::operator++() {
        pointer_ += size;
        return *this;
    }

    RawMeshIter RawMeshIter::operator++(int k) {
        RawMeshIter res = *this;
        pointer_ += size * k;
        return res;
    }

    RawMeshIter& RawMeshIter::operator--() {
        RawMeshIter res = *this;
        pointer_ -= size;
        return res;
    }

    RawMeshIter RawMeshIter::operator--(int k) {
        RawMeshIter res = *this;
        pointer_ -= size * k;
        return res;
    }

    void RawMeshIter::setVertexPosition(const Vector3D& pos)
    {
        *((Vector3D*)(pointer_)) = pos;
    }

    void RawMeshIter::setColor(const Vector4D& color, int index)
    {
        *(Vector4D*)(pointer_ + color_offset_ + VECTOR4D_LEN* index) = color;
    }

    void RawMeshIter::setNormal(const Vector3D& normal, int index)
    {
        *(Vector3D*)(pointer_ + normal_offset_ + index* VECTOR3D_LEN) = normal;
    }

    void RawMeshIter::setTangentX(const Vector3D& tangentX, int index)
    {
        *(Vector3D*)(pointer_ + tangent_offset_ + VECTOR3D_LEN * index ) = tangentX;
    }

    void RawMeshIter::setTangentY(const Vector3D& tangentY, int index)
    {
        *(Vector3D*)(pointer_ + bitangent_offset_ + VECTOR3D_LEN * index) = tangentY;
    }

    void RawMeshIter::setTextureCoord(const Vector3D& texCoord, int index)
    {
        *(Vector3D*)(pointer_ + textureCoord_offset_ + VECTOR3D_LEN * index) = texCoord;
    }

    void RawMeshIter::setJointIndex(int jointIndex, int index) {
        *(int*)(pointer_ + jointIndex_offset_ + sizeof(int) * index) = jointIndex;
    }

    void RawMeshIter::setWeight(float weight, int index)
    {
        *(float*)(pointer_ + weight_offset_ + sizeof(float) * index) = weight;
    }

    Vector3D& RawMeshIter::getVertexPosition()
    {
        return *((Vector3D*)(pointer_));
    }

    Vector4D& RawMeshIter::getColor()
    {
        return *((Vector4D*)(pointer_ + color_offset_));
    }

    Vector3D& RawMeshIter::getNormal()
    {
        return *((Vector3D*)(pointer_ + normal_offset_));
    }

    Vector3D& RawMeshIter::getTangentX()
    {
        return *((Vector3D*)(pointer_ + tangent_offset_));
    }

    Vector3D& RawMeshIter::getTangentY()
    {
        return *((Vector3D*)(pointer_ + bitangent_offset_));
    }

    Vector3D& RawMeshIter::getTextureCoord(int index)
    {
        return *(Vector3D*)(pointer_ + tangent_offset_ + VECTOR3D_LEN * index);
    }

    int& RawMeshIter::getJointIndex(int index)
    {
        return *(int*)(pointer_ + jointIndex_offset_ + sizeof(int) * index);
    }

    float& RawMeshIter::getJointWeight(int index)
    {
        return *(float*)(pointer_ + weight_offset_ + sizeof(float) * index);
    }

    char* RawMesh::appendBlock()
    {
        meshData_.resize(meshData_.size() + blockInfo_.size);
        size_ += 1;
        //make sure this is ok
        return &(*meshData_.end())- blockInfo_.size;
    }

    void RawMesh::resize(int siz)
    {
        size_ = siz;
        meshData_.resize(blockInfo_.size * siz);
    }

    const char* RawMesh::data()
    {
        return meshData_.data();
    }

    char* RawMesh::operator[](int k) {
        return meshData_.data() + k * blockInfo_.size;
    }

    int RawMesh::size()
    {
        return size_;
    }

    void RawMesh::updateOffset()
    {

    }

}

