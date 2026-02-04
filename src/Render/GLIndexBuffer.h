#ifndef __GL_INDEX_BUFFER_H
#define __GL_INDEX_BUFFER_H

#include "IndexBuffer.h"

namespace Sun {

    class GLIndexBuffer : public IndexBuffer {
    public:
        GLIndexBuffer();
        GLIndexBuffer(const unsigned int* data, unsigned int count);
        ~GLIndexBuffer();
        void bind() const override;
        void unBind() const override;
    };


}

#endif 