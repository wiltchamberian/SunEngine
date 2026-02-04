#include "GLVertexArray.h"
#include "GLFunctions.h"
#include "Util/exception.h"

namespace Sun {

    GLVertexArray::GLVertexArray() {
        glGenVertexArrays(1, &id_);
    }

    GLVertexArray::~GLVertexArray() {
        glDeleteVertexArrays(1, &id_);
    }

    void GLVertexArray::bind() {
        glBindVertexArray(id_);
    }

    void GLVertexArray::unBind() {
        glBindVertexArray(0);
    }

    void GLVertexArray::addVertexBuffer(std::shared_ptr<VertexBuffer> vb) {
        vb_ = vb;
        vb_->bind();
    }

    void GLVertexArray::addVertexBufferLayout(const VertexBufferLayout& layout) {
        for (int i = 0; i < layout.size(); ++i) {
            const VertexLayoutElement& ele = layout[i];
            glEnableVertexAttribArray(ele.layoutIndex);
            if (ele.type == EBasicDataType::FLOAT) {
                glVertexAttribPointer(ele.layoutIndex, ele.count, GL_FLOAT, ele.normalized, ele.stride, (void*)ele.offset);
            }
            else if (ele.type == EBasicDataType::INT) {
                glVertexAttribIPointer(ele.layoutIndex, ele.count, GL_INT, ele.stride, (void*)ele.offset);
            }
            else {
                ASSERT(false);
            }
        }
    }

    void GLVertexArray::addIndexBuffer(std::shared_ptr<IndexBuffer> eb) {
        eb_ = eb;
        eb_->bind();
    }

    unsigned int GLVertexArray::getId() const {
        return id_;
    }

    unsigned int GLVertexArray::getIndiceCount() const {
        if(eb_)
            return eb_->getCount();
        return 0;
    }

    unsigned int GLVertexArray::getBufByteSize() const {
        if (vb_) {
            return vb_->getSize();
        }
        return 0;
    }

    void GLVertexArray::bindVB() {
        if (vb_) {
            vb_->bind();
        }
    }

    void GLVertexArray::unBindVB() {
        if (vb_) {
            vb_->unBind();
        }
    }

}