#include "Program.h"
#include "PCH.h"
#include "Util/exception.h"
#include "Render/Shader.h"
#if defined OPENGL
#include "Render/GLFunctions.h"
#elif defined VULKAN
#endif

namespace Sun {

	ShaderProgram::ShaderProgram() {

	}

	ShaderProgram::ShaderProgram(std::shared_ptr<Shader> shader) {
    }

	ShaderProgram::ShaderProgram(const std::vector<std::shared_ptr<Shader>>& shaders) {

	}

	ShaderProgram::~ShaderProgram() {

    }

}