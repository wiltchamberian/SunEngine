#include "MeshComponent.h"
#include "Primitive/StaticMeshPrimitive.h"
#include "Render/GraphicsLib.h"

namespace Sun {

    MeshComponent::~MeshComponent() {
        releaseGpu();
    }

    void MeshComponent::prepareRender() {

    }

    void MeshComponent::beginRender() {

    }

    void MeshComponent::endRender() {

    }

    void MeshComponent::tick(float dtime, const SEvent& ev) {
        return;
    }

    void MeshComponent::allocGpu(EDrawMode drawType) {
        if (gpuVertexDataHandle_.vao== INVALID_HANDLE) {
            gpuVertexDataHandle_.mode = triangleType_;
            g_graphicsLib->applyGpuVertexData(gpuVertexDataHandle_, Vertex::getLayout(), (void*)getVerticesData(), getVerticesSize(), sizeof(Vertex),
                (void*)getIndicesData(), getIndicesSize(), sizeof(int), drawType);
        }
        else {
            assert(false); //TODO ,alloc used gpu handle
        }
    }

    void MeshComponent::releaseGpu() {
        g_graphicsLib->deleteVertexArray(gpuVertexDataHandle_.vao);
        gpuVertexDataHandle_.vao = INVALID_HANDLE;
        for (auto& it : gpuVertexDataHandle_.vec) {
            g_graphicsLib->deleteBufferData(it.ebo);
            g_graphicsLib->deleteBufferData(it.vbo);
        }
        gpuVertexDataHandle_.vec.clear();
    }

    std::vector<ShaderProgram> MeshComponent::getMultShaderPass() {
        return {};
    }

    bool MeshComponent::hasMultiShaderPass() const{
        return false;
    }

    void MeshComponent::setVertices(const std::vector<Vertex>& vertices) {
        vertices_ = vertices;
    }

    void MeshComponent::setVertices(std::vector<Vertex>&& vertices) {
        vertices_ = std::move(vertices);
    }

    void MeshComponent::setIndices(const std::vector<int>& indices) {
        indices_ = indices;
    }

    void MeshComponent::setIndices(std::vector<int>&& indices) {
        indices_ = std::move(indices);
    }

    std::vector<Vertex>& MeshComponent::getVertices() {
        return vertices_;
    }

    std::vector<int>& MeshComponent::getIndices() {
        return indices_;
    }

    const Vertex* MeshComponent::getVerticesData() const {
        return vertices_.data();
    }

    size_t MeshComponent::getVerticesSize() const {
        return vertices_.size();
    }

    const int* MeshComponent::getIndicesData() const{
        return indices_.data();
    }

    size_t MeshComponent::getIndicesSize() const {
        return indices_.size();
    }
}