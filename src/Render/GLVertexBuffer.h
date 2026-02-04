#ifndef __GL_VERTEX_BUFFER_H
#define __GL_VERTEX_BUFFER_H

#include "VertexBuffer.h"

namespace Sun {

    class GLVertexBuffer : public VertexBuffer {
    public:
        GLVertexBuffer(const void* data, unsigned int siz,EDrawMode buffType = EDrawMode::STATIC_DRAW);
        ~GLVertexBuffer();

        void bind() const override;
        void unBind() const override;
    };

}

#endif