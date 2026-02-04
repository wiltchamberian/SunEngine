#include "GLShaderFactory.h"
#include <fstream>
#include <sstream>
#include "Render/GLFunctions.h"
#include "Util/exception.h"

namespace Sun {

    int GLShaderFactory::createShaderByPath(const char* path, int type) {
		std::fstream fs(path);
		if (!fs.is_open())return -1;

		std::stringstream buffer;
		buffer << fs.rdbuf();
		std::string src = buffer.str();

		return createShaderBySrc(src.c_str(), type);
    }

	// type such as "GL_VERTEX_SHADER"
    int GLShaderFactory::createShaderBySrc(const char* src, int type) {
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

	void GLShaderFactory::deleteShader(int shader) {
		GLFunctions::glDeleteShader(shader); 
	}

}
