#include "GLProgramFactory.h"
#include "Render/GLFunctions.h"
#include "Util/exception.h"

namespace Sun {

    int GLProgramFactory::createProgram(int shader) {
        const GLubyte* pVersion = GLFunctions::glGetString(GL_VERSION);
        const GLubyte* pGlslVersion = GLFunctions::glGetString(GL_SHADING_LANGUAGE_VERSION);

		int id = GLFunctions::glCreateProgram();
		GLFunctions::glAttachShader(id, shader);
		GLFunctions::glLinkProgram(id);
		int success = 0;
		char infoLog[512];
		GLFunctions::glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success) {
			GLFunctions::glGetProgramInfoLog(id, 512, NULL, infoLog);
			ASSERT(false);
		}
		GLFunctions::glUseProgram(id);
		return id;
    }

    int GLProgramFactory::createProgram(const std::vector<int>& shaders) {
		const GLubyte* pVersion = GLFunctions::glGetString(GL_VERSION);
		const GLubyte* pGlslVersion = GLFunctions::glGetString(GL_SHADING_LANGUAGE_VERSION);

		int id = GLFunctions::glCreateProgram();
		for (int i = 0; i < shaders.size(); ++i) {
			GLFunctions::glAttachShader(id, shaders[i]);
		}
		
		GLFunctions::glLinkProgram(id);
		int success = 0;
		char infoLog[512];
		GLFunctions::glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success) {
			GLFunctions::glGetProgramInfoLog(id, 512, NULL, infoLog);
			ASSERT(false);
		}
		GLFunctions::glUseProgram(id);
		return id;
    }

}