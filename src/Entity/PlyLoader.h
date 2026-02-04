/*****************************************************************************
* @brief : PlyLoader
* Ω‚Œˆply∏Ò Ω
* @author : acedtang
* @date : 2021/5/21
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __PLY_LOADER_H
#define __PLY_LOADER_H

#include <chrono>
#include "tinyply.h"
#include "Primitive/StaticMeshPrimitive.h"
#include "Component/MeshComponent.h"
#include "Entity/GameObject.h"

namespace Sun {

    struct float2 { float x, y; };
    struct float3 { float x, y, z; };
    struct double3 { double x, y, z; };
    struct uint3 { uint32_t x, y, z; };
    struct uint4 { uint32_t x, y, z, w; };

    class manual_timer
    {
        std::chrono::high_resolution_clock::time_point t0;
        double timestamp{ 0.0 };
    public:
        void start() { t0 = std::chrono::high_resolution_clock::now(); }
        void stop() { timestamp = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count() * 1000.0; }
        const double& get() { return timestamp; }
    };

    struct geometry
    {
        std::vector<float3> vertices;
        std::vector<float3> normals;
        std::vector<float2> texcoords;
        std::vector<uint3> triangles;
    };

    class PlyLoader
    {
    public:
        GameObject* load(const std::string& path);
        void setDefaultColor(const vec4& color);
    protected:
        vec4 defaultColor_;
    };

}


#endif