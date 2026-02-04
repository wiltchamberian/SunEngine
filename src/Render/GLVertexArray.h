#ifndef __GL_VERTEX_ARRAY_H
#define __GL_VERTEX_ARRAY_H

#include "VertexArray.h"

namespace Sun {

    class GLVertexArray : public VertexArray {
    public:
        GLVertexArray();
        ~GLVertexArray();
        void bind() override;
        void unBind() override;
        void addVertexBuffer(std::shared_ptr<VertexBuffer> vb) override;
        void addVertexBufferLayout(const VertexBufferLayout& layout) override;
        void addIndexBuffer(std::shared_ptr<IndexBuffer> eb) override;
        unsigned int getId() const override;
        unsigned int getIndiceCount() const override;
        unsigned int getBufByteSize() const override;
        void bindVB() override;
        void unBindVB() override;
    protected:
        unsigned int id_;
        std::shared_ptr<VertexBuffer> vb_;
        std::shared_ptr<IndexBuffer> eb_;
        VertexBufferLayout layout_;
    };
}


#endif