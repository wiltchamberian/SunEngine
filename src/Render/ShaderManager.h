/**
 * @projectName   Sun
 * @brief         ShaderManager
 * 着色器管理类，用于管理着色器以及与相应的组件的映射关系
 * @author        acedtang
 * @date          2021/05/7
 */

#ifndef __SHADER_MANAGER_H
#define __SHADER_MANAGER_H

#include "Render/RenderHandle.h"
#include "Render/RenderPass.h"
#include "Common/glsl.h"

namespace Sun {
    
    //对应RHI2的GraphicsPipelineMgr
    class ShaderManager
    {
    public:
        //加载shader配置文件
        void loadShaders();
        void createShaders();
        ShaderProgram getShaderProgram(const std::string& name) const;
    protected:
        void createRayTracingShader();
        void createRayTracingShader2();

    public:
        std::map<std::string, ShaderProgram> shaderMap_;

        ShaderProgram rayGenProgram_;
        ShaderProgram rayTracingProgram_;
        ShaderProgram rayTracingShowProgram_;
    };

    extern ShaderManager* g_shaderManager;

}

#endif
