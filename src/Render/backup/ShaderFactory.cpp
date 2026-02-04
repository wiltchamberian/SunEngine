#include "ShaderFactory.h"
#include "PCH.h"
#include <fstream>

namespace Sun {

    std::shared_ptr<Shader> ShaderFactory::createShaderByPath(const char* path, int type) {
#if defined OPENGL
        std::fstream fs(path);
        if (!fs.is_open())return -1;

        std::stringstream buffer;
        buffer << fs.rdbuf();
        std::string src = buffer.str();

        return createShaderBySrc(src.c_str(), type);
#elif defined VULKAN
#endif
    }

    std::shared_ptr<Shader> ShaderFactory::createShaderBySrc(const char* src, int type) {
        int id = GLFunctions::glCreateShader(type);
        GLFunctions::glShaderSource(id, 1, &src, NULL);
        GLFunctions::glCompileShader(id);
        int  success = 0;
        char infoLog[512];
        GLFunctions::glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            GLFunctions::glGetShaderInfoLog(id, 512, NULL, infoLog);
            ASSERT(false);
        }
        return id;
    }
}