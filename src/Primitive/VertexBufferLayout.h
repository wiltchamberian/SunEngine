#ifndef __VERTEX_BUFFER_LAYOUT_H
#define __VERTEX_BUFFER_LAYOUT_H

#include <vector>
#include "VertexBufferElement.h"


namespace Sun {

    //using VertexBufferLayout = std::vector<VertexLayoutElement>;

    class VertexBufferLayout : public std::vector<VertexLayoutElement> {

    };
}

#endif