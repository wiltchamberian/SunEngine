#include "GLIndexBuffer.h"
#include "GLFunctions.h"

namespace Sun {

    GLIndexBuffer::GLIndexBuffer() {}

    GLIndexBuffer::GLIndexBuffer(const unsigned int* data, unsigned int count) {
        static_assert(sizeof(GLuint) == sizeof(unsigned int), "GLuint != unsigend int");
        count_ = count;
        glGenBuffers(1, &id_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    }

    GLIndexBuffer::~GLIndexBuffer(){
        glDeleteBuffers(1, &id_);
    }

    void GLIndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    }

    void GLIndexBuffer::unBind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}