#include "PointLight.h"
#include "Render/GraphicsLib.h"
#include "Render/ShaderManager.h"
#include "Entity/GameObject.h"

namespace Sun {

    void PointLight::onAddedToLevel() {
        static int uniformId = 0;
        const int uniformNum = 100; //should be equal to the shader
        uniformId = uniformId % uniformNum;
        uniformId_ = uniformId;
        
        if (!g_graphicsLib) return;
        if (true /*info.passType == RenderPassType::RASTERIZATION*/) {
            //必须要调用才能生效
            g_graphicsLib->useProgram(g_shaderManager->getShaderProgram(SP_NORMAL_PASS));
            //g_graphicsLib->getUni
            unsigned int id = g_shaderManager->getShaderProgram(SP_NORMAL_PASS).getId();
            vec4 pos = getWorldMatrix() * vec4(0, 0, 0, 1);
            vec3 pt = vec3(pos.x, pos.y, pos.z);

            std::string posStr = "ptLights[" + std::to_string(uniformId) + "].lightPos";
            std::string colorStr = "ptLights[" + std::to_string(uniformId) + "].lightColor";
            g_graphicsLib->setVec3(id, posStr, pt);
            g_graphicsLib->setVec3(id, colorStr, color_);
            g_graphicsLib->setInt(id, "numPtLight", (uniformId + 1));

            g_graphicsLib->useProgram(g_shaderManager->getShaderProgram(SP_STATIC_MESH_RENDER_DEFFERED_SHADER));
            id = g_shaderManager->getShaderProgram(SP_STATIC_MESH_RENDER_DEFFERED_SHADER).getId();
            g_graphicsLib->setVec3(id, posStr, pt);
            g_graphicsLib->setVec3(id, colorStr, color_);
            g_graphicsLib->setInt(id, "numPtLight", (uniformId + 1));

        }

        uniformId++;
    }

    void PointLight::tick(float dtime, const SEvent& ev) {
        static float time = 0;
        time += dtime;
        vec3 lightColor;
        lightColor.x = sin(time * 2.0f) * 300.;
        lightColor.y = sin(time * 0.7f) * 300.;
        lightColor.z = sin(time * 1.3f) * 300.;
        //color_ = lightColor;
    }

    void PointLight::bindUniformData() {
        std::string colorStr = "ptLights[" + std::to_string(uniformId_) + "].lightColor";
        g_graphicsLib->setVec3(pro_.getId(), colorStr, color_);
    }
#if 0
    void PointLightComponent::bind(const ShaderProgram& pro) {
        if (!g_graphicsLib) return;

        /*g_graphicsLib->useProgram(g_renderManager->staticMeshProgram_);
        std::string colorStr = "ptLights[" + std::to_string(uniformId_) + "].lightColor";
        g_graphicsLib->setVec3(g_renderManager->staticMeshProgram_->getId(), colorStr, lightColor);*/
        
    }
#endif
}