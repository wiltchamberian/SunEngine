#include "Program.h"
#include <fstream>
#include <sstream>
#include "Util/Define.h"
#include "PCH.h"

namespace Sun {

    Program::Program() {

    }

    Program::Program(const char* vertexPath, const char* fragmentPath)
        :vertexShaderPath_(vertexPath)
        ,fragmentShaderPath_(fragmentPath)
    {
    }

    bool Program::readVertexShader(const char* filepath) {
        std::fstream fs(filepath);
        if (!fs.is_open())return false;

        std::stringstream buffer;
        buffer << fs.rdbuf();
        vertexShader_ = buffer.str();

        return true;
    }

    bool Program::readFragmentShader(const char* filepath) {

        std::fstream fs(filepath);
        if (!fs.is_open())return false;

        std::stringstream buffer;
        buffer << fs.rdbuf();
        fragmentShader_ = buffer.str();

        return true;
    }

}