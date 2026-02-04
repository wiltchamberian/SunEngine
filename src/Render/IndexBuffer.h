#ifndef __INDEX_BUFFER_H
#define __INDEX_BUFFER_H

namespace Sun {

    class IndexBuffer {
        friend class BufferFactory;
    public:
        IndexBuffer();
        IndexBuffer(const unsigned int* data, unsigned int count);
        ~IndexBuffer();
        inline unsigned int getCount() { return count_; }
        inline unsigned int getId() { return id_; }
        virtual void bind() const = 0;
        virtual void unBind() const = 0;
    protected:
        unsigned int id_ = 0;
        unsigned int count_ = 0;
    };


}

#endif