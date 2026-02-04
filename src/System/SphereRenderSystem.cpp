#include "SphereRenderSystem.h"
#include "Render/GraphicsLib.h"
#include "Render/IndexBuffer.h"
#include "Render/VertexBuffer.h"

namespace Sun {

    void SphereRenderSystem::prepareRender() {
        if (passType_ == RenderPassType::RAY_TRACING) {
            // datacpy Optimize FIXME
            std::vector<SpherePrimitive> datas(spheres_.size());
            for (int i = 0; i < spheres_.size(); ++i) {
                datas[i] = spheres_[i]->spherePrimitive;
            }
            unsigned int h = g_graphicsLib->genBufferData
            (datas.data(), datas.size() * sizeof(SpherePrimitive), EBufferType::SHADER_STORAGE_BUFFER, EDrawMode::STATIC_DRAW);
            g_graphicsLib->bindBufferBase(EBufferType::SHADER_STORAGE_BUFFER, 2, h);
        }
        else if (passType_ == RenderPassType::RASTERIZATION) {

        }
    }

    void SphereRenderSystem::render() {

    }
}