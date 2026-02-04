#ifndef __WINDING_H
#define __WINDING_H

#include <vector>
#include "Math/Vector3D.h"

namespace Sun {
    //a winding gives the bounding points of a convex polygon
    //凸多边形的边界点集合
    class Winding
    {
    public:
        std::vector<vec3> _pts;
    };
}


#endif