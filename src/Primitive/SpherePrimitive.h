#ifndef __SPHERE_PRIMITIVE_H
#define __SPHERE_PRIMITIVE_H

#include "Math/Vector3D.h"
#include "Math/Vector4D.h"
#include "Math/Sphere.h"

namespace Sun {

    struct SpherePrimitive:public Sphere
    {
        vec4 color_;
        static int type_;
    };
}


#endif