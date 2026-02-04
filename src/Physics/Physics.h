#ifndef __PHYSICS_H
#define __PHYSICS_H

#include "boundingVolume.h"
#include "Math/Plane.h"
#include "Math/Triangle.h"
#include "Math/Sphere.h"
#include "Math/Box.h"

/**
 * @projectName   Sun
 * @brief         PhysicsEngine
 * @author        acedtang
 * @date          2021/01/21
 */

namespace Sun
{
    struct Hint {
        float t; //0<=t<=1
        vec3 normal;
        vec3 hintPt;
        bool isHint;
        int targetId;
    };

    //辅助函数 triangle proj to axis
    std::pair<float, float> getProjInterval(const Triangle& tri, const vec3& axis);

    std::pair<float, float>  getProjInterval(const AABB& box, const vec3& axis);

    bool isRectangleCollide(const BoundingBox& r1, const BoundingBox& r2);

    bool isSphereCollid(const Sphere& p1, const Sphere& p2);

    bool isAllInFront(const BoundingBox& box, const Plane& plane);

    bool isAllInBack(const BoundingBox& box, const Plane& plane);

    //判断一个球体是否和一个平面相交
    bool isCollide(const vec3& center, float radius, const vec3& normal, float dir_dis);
    bool isCollide(const Sphere& sphere, const Plane& plane);

    //判断一个球体和一个三角面片是否相交(三角形的顶点应当是逆时针排列的)
    bool isCollide(const vec3& center, float r, const vec3& v1, const vec3& v2, const vec3& v3);
    bool isCollide(const Sphere& sphere, const Triangle& triangle);

    //判断一个三角形面片和一个AABB是否相交(SAT)
    bool isCollide(const Triangle& triangle, const AABB& box);

    //计算线段和三角形面片的交点
    Hint hint_segment_triangle(const vec3& start, const vec3& end, const vec3& a, const vec3& b, const vec3& c);

    //计算线段和不完全圆柱面的交点
    Hint hint_segment_cylinder(const vec3& start, const vec3& end, const vec3& cylinderStart, const vec3& cylinderEnd,
        float r, const vec3& v1, const vec3& v2);

    //计算线段和不完全球面的交点
    Hint hint_segment_sphere(const vec3& start, const vec3& end, const vec3& center, float r, const vec3& axis, const vec3& v1, const vec3& v2);

    //判断一个球的直线运动轨迹是否与一个三角形面片相交，如果相交，返回交点的位置信息，以及碰撞点的法向量信息
    //此算法为笔者自创，非常经典
    Hint isSphereTraceTriangleCollide(const vec3& start, const vec3& end, float radius, const vec3& a, const vec3& b, const vec3& c);

    
};

#endif

