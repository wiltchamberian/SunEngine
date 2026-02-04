/**
 * @projectName   Sun
 * @brief         ShaderFacotry
 * @author        acedtang
 * @date          2021/5/6
 */

#ifndef __SHADER_FACTORY_H
#define __SHADER_FACTORY_H

#include <memory>
#include "Shader.h"

namespace Sun {

    class ShaderFactory
    {
    public:
        virtual int createShaderByPath(const char* path, int type);
        virtual int createShaderBySrc(const char* src, int type);
        virtual void deleteShader(int shader) = 0;
    };

    class ShaderFactory {
    public:
        std::shared_ptr<Shader> createShaderByPath(const char* path, int type);
        std::shared_ptr<Shader> createShaderBySrc(const char* src, int type);
    };

}

#endif