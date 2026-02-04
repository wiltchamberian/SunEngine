#ifndef __GL_SHADER__FACTORY_H
#define __GL_SHADER_FACTORY_H

#include "ShaderFactory.h"

namespace Sun {

    class GLShaderFactory : public ShaderFactory
    {
    public:
        virtual int createShaderByPath(const char* path, int type) override;
        virtual int createShaderBySrc(const char* src, int type) override;

        virtual void deleteShader(int shader) override;
    };

}

#endif 