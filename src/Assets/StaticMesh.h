/*****************************************************************************
* @brief : staticMesh
* A Static Mesh is a piece of geometry that consists of a set of polygons that can be cached in video memory and rendered by the graphics card
* 静态网格是由一组多边形组成的几何体，这些多边形可以缓存在显存中并由图形卡渲染
* @author : acedtang
* @date : 2021/3/20
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/
#ifndef __STATIC_MESH_H
#define __STATIC_MESH_H

#include <vector>
#include <math.h>
#include <string>
#include "Math/Matrix4x4.h"
#include "Math/Vector2D.h"
#include "Math/Vector3D.h"
#include "Primitive/Texture.h"
#include "Render/RenderHandle.h"
#include "Util/SmartPointer.h"
#include "Meta/Object.h"
#include "Assets/StaticMeshSection.h"


namespace Sun
{
    using Point3D = Vector3D<float>;
    using Point2D = Vector2D<float>;
    using Point4D = Vector4D<float>;

    class StaticMesh
    {
        friend class AssetsMgr;
    public:
        StaticMesh();
    public:
        std::vector<StaticMeshSection> meshSections_;
        Guid guid_;
    };
}






#endif
