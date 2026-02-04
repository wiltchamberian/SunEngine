#ifndef __GL_SHADER_H
#define __GL_SHADER_H

#include "Shader.h"
#include "Common/Macros.h"

namespace Sun {

    class GLShader : public Shader {
    public:
        friend class GLGraphicsLib;
        ~GLShader();
        GLShader(const char* str, EShaderType type, bool pathOrSrc);
    protected:
        void createBySrc(const char* src);
        void createByPath(const char* path);
    };

}

#endif