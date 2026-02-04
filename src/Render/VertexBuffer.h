#ifndef __VERTEX_BUFFER_H
#define __VERTEX_BUFFER_H

#include "Common/BasicDataType.h"

namespace Sun {

    class VertexBuffer {
    public:  
        VertexBuffer();
        ~VertexBuffer();
        VertexBuffer(const void*, unsigned int siz,EDrawMode drawType = EDrawMode::STATIC_DRAW);
        virtual void bind() const = 0;
        virtual void unBind() const = 0;
        inline unsigned int getId() { return id_; }
        inline unsigned int getSize() { return siz_; }
    protected:
        unsigned int id_ = -1;
        unsigned int siz_ = 0;
    };


}

#endif