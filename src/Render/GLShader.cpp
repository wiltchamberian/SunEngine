#include "GLShader.h"
#include <fstream>
#include <sstream>
#include "Util/exception.h"
#include "GLFunctions.h"
#include "Common/Macros.h"
#include "GLEnumMap.h"

namespace Sun {

	GLShader::~GLShader() {
		glDeleteShader(id_);
	}

	GLShader::GLShader(const char* str, EShaderType type, bool pathOrSrc)
	{
		type_ = type;
		if (pathOrSrc) {
			createByPath(str);
		}
		else {
			createBySrc(str);
		}
	}

	void GLShader::createBySrc(const char* src) {
		id_ = glCreateShader(gl_enum_map(type_));
		glShaderSource(id_, 1, &src, NULL);
		glCompileShader(id_);
		int  success = 0;
		char infoLog[512];
		glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id_, 512, NULL, infoLog);
			ASSERT(false);
		}
	}

	void GLShader::createByPath(const char* path) {
		std::fstream fs(path);
		if (!fs.is_open()) {
			ASSERT(false);
			fs.close();
			return;
		}

		std::stringstream buffer;
		buffer << fs.rdbuf();
		std::string src = buffer.str();

		createBySrc(src.c_str());
	}

}