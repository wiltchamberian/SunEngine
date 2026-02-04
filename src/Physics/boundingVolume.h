#ifndef __BOUNDINGVOLUME_H
#define __BOUNDINGVOLUME_H

#include <vector>
#include "Math/Vector3D.h"
#include "Math/Matrix4x4.h"

namespace Sun {
    //AABB包围盒
    struct BoundingBox
    {
        vec3 _center;
        float _half_x;
        float _half_y;
        float _half_z;
        std::vector<vec3> getVertices() const;
    };

    //胶囊体
    struct BoundingCapsule
    {
        vec3 _center1;
        vec3 _center2;
        float _r;
    };

    //定向包围盒
    struct OrientedBoundingBox
    {
        Matrix4x4 _trans;
        float _half_x;
        float _half_y;
        float _half_z;
    };
}



#endif