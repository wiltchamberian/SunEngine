#ifndef __PRIMITIVE_DEFINE_H
#define __PRIMITIVE_DEFINE_H

namespace Sun {

#define MAX_TEXCOORDS 1
#define MAX_TOTAL_INFLUENCES 4

    //定义了原语的类型
    enum ShapeType {
        BOX,
        STATIC_MESH,
        SKELETAL_MESH,
        SPHERE
    };

    template<class _T>
    struct PHandle {
        int h;
        PHandle<_T>() { 
        }
        PHandle<_T>(int handle)
        :h(handle)
        {

        }
    };

}


#endif 