#ifndef __SIDE_H
#define __SIDE_H

#include "Math/Plane.h"
#include "winding.h"

namespace Sun {
    //由一个平面和平面上一个多边形构成,并没有存多边形，而是存着点索引
    class Side
    {
    public:
        Plane plane;
        std::vector<int> indexes;
    };
}

#endif