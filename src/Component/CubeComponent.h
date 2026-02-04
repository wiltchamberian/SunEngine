#ifndef __CUBE_COMPONENT_H
#define __CUBE_COMPONENT_H

#include "MeshComponent.h"

namespace Sun {

    class CubeComponent : public MeshComponent
    {
    public:
        CubeComponent();
        CubeComponent(const vec3& center, const float& halfLen,const vec4& color);
        ~CubeComponent();

    public:
        vec3 center_;
        float    halfLen_ = 1.0;
    };

}

#endif