#ifndef __SHADER_H
#define __SHADER_H

#include "Common/Macros.h"
#include "Common/BasicDataType.h"

namespace Sun {

    class Shader {
    public:
        inline unsigned int getId() const { return id_; }
    public:
        Shader();
        Shader(const char* str, EShaderType type ,bool pathOrSrc);
        ~Shader();
    public:
        unsigned int id_;
        EShaderType type_;
    };

}

#endif