#ifndef __VERTEX_ARRAY_H
#define __VERTEX_ARRAY_H

#include <memory>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Primitive/VertexBufferLayout.h"

namespace Sun {

    class VertexArray {
    public:    
        VertexArray();
        ~VertexArray();
        virtual void bind() = 0;
        virtual void unBind() = 0;
        virtual void addVertexBuffer(std::shared_ptr<VertexBuffer> vb) = 0;
        virtual void addVertexBufferLayout(const VertexBufferLayout& layout) = 0;
        virtual void addIndexBuffer(std::shared_ptr<IndexBuffer> eb) = 0;
        virtual unsigned int getId() const = 0;
        virtual unsigned int getIndiceCount() const = 0;
        virtual unsigned int getBufByteSize() const = 0;
        virtual void bindVB() = 0;
        virtual void unBindVB() = 0;
    protected:
    };


}

#endif