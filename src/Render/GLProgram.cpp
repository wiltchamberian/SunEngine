#include "GLProgram.h"
#include "Render/GLFunctions.h"
#include "Util/exception.h"

namespace Sun {

    GLProgram::GLProgram(std::shared_ptr<Shader> shader) {
		const GLubyte* pVersion = glGetString(GL_VERSION);
		const GLubyte* pGlslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

		id_ = glCreateProgram();
		glAttachShader(id_, shader->getId());
		glLinkProgram(id_);
		int success = 0;
		char infoLog[512];
		glGetProgramiv(id_, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(id_, 512, NULL, infoLog);
			ASSERT(false);
		}
		glUseProgram(id_);
    }

    GLProgram::GLProgram(const std::vector<std::shared_ptr<Shader>>& shaders) {
		const GLubyte* pVersion = glGetString(GL_VERSION);
		const GLubyte* pGlslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

		id_ = glCreateProgram();
		for (int i = 0; i < shaders.size(); ++i) {
			glAttachShader(id_, shaders[i]->getId());
		}

		glLinkProgram(id_);
		int success = 0;
		char infoLog[512];
		glGetProgramiv(id_, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(id_, 512, NULL, infoLog);
			ASSERT(false);
		}
		glUseProgram(id_);
    }

    GLProgram::~GLProgram() {
		glDeleteShader(id_);
    }


}