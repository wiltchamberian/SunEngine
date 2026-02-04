#ifndef __VERTEX_BUFFER_ELEMENT_H
#define __VERTEX_BUFFER_ELEMENT_H

#include "Common/Macros.h"
#include "Common/BasicDataType.h"

namespace Sun {
    struct VertexLayoutElement {
        //对应glsl中location的值
        unsigned int layoutIndex;
        //基础数据类型个数
        unsigned int count;   
        EBasicDataType type;
        //是否归一化
        bool normalized;
        //该元素在内存中相邻两次出现之间的间隔
        unsigned int stride;
        //单次出现相对起始位置的偏移量
        unsigned int offset;
        //added for instanced render
        int attributeDivisor = 0;
    };
}

#endif