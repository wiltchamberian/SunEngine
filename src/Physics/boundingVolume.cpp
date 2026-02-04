#include "boundingVolume.h"

namespace Sun {

    std::vector<vec3> BoundingBox::getVertices() const
    {
        std::vector<vec3> res;
        res.push_back({ _center.x - _half_x,_center.y - _half_y,_center.z - _half_z });
        res.push_back({ _center.x - _half_x,_center.y - _half_y,_center.z + _half_z });
        res.push_back({ _center.x - _half_x,_center.y + _half_y,_center.z - _half_z });
        res.push_back({ _center.x - _half_x,_center.y + _half_y,_center.z + _half_z });
        res.push_back({ _center.x + _half_x,_center.y - _half_y,_center.z - _half_z });
        res.push_back({ _center.x + _half_x,_center.y - _half_y,_center.z + _half_z });
        res.push_back({ _center.x + _half_x,_center.y + _half_y,_center.z - _half_z });
        res.push_back({ _center.x + _half_x,_center.y + _half_y,_center.z + _half_z });

        return res;
    }

}
