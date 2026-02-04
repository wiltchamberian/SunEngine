#include "GLVertexBuffer.h"
#include "GLFunctions.h"
#include "Render/GLEnumMap.h"

namespace Sun {

    GLVertexBuffer::GLVertexBuffer(const void* data, unsigned int siz, EDrawMode drawType) {
        siz_ = siz;
        unsigned int usage = gl_enum_map(drawType);
        glGenBuffers(1, &id_);
        glBindBuffer(GL_ARRAY_BUFFER, id_);
        glBufferData(GL_ARRAY_BUFFER, siz, data, usage);
    }

    GLVertexBuffer::~GLVertexBuffer() {
        glDeleteBuffers(1, &id_);
    }

    void GLVertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, id_);
    }

    void GLVertexBuffer::unBind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


}