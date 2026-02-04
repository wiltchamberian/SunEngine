#include "Shader.h"
#include <fstream>
#include <sstream>
#include "PCH.h"
#if defined OPENGL
#include "GLFunctions.h"
#endif
#include "Util/exception.h"

namespace Sun {

	Shader::Shader() {
	}

    Shader::Shader(const char* str, EShaderType type, bool pathOrSrc)
	:type_(type)
	{

    }

    Shader::~Shader() {

    }

}