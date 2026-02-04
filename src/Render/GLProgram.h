#ifndef __GL_PROGRAM_H
#define __GL_PROGRAM_H

#include "Program.h"
#include "Shader.h"

namespace Sun {

    class GLProgram : public ShaderProgram
    {
    public:
        GLProgram(std::shared_ptr<Shader> shader);
        GLProgram(const std::vector<std::shared_ptr<Shader>>& vec);
        ~GLProgram();

    protected:
    };

}

#endif