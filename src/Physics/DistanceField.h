/*****************************************************************************
* @brief : DistanceField
* 距离场函数合集,ud打头的表示无向距离场，sd打头的表示有向距离场
* @author : acedtang
* @date : 2021/6/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __DISTANCE_FIELD_H
#define __DISTANCE_FIELD_H

#include "Math/Vector3D.h"
#include "Math/Geometry.h"

namespace Sun {

    //计算p到三角形abc的无向距离
    inline float udTriangle(const vec3& p, const vec3& a, const vec3& b, const vec3& c) {
        vec3 ab = b - a; vec3 ap = p - a;
        vec3 bc = c - b; vec3 bp = p - b;
        vec3 ca = a - c; vec3 cp = p - c;
        //反向法线
        vec3 nor = ab.crossProduct(ca);

        /*
        三目运算符的第一项通过叉乘判p在三角形所在平面的投影点位于三角形三条边的哪一侧
        如果都是内侧，则输出3.0
        最后一项计算点到平面的距离平方，此时也就是到三角形面片的距离(p在内侧)
        如果p点的投影点位于三角形外部，
        则lowerBound的三项分别计算p到三角形三条边的投影距离，不过区别在于，
        当投影点超出三角形边线段的的范围，输出的是到线段端点的距离(clamp函数的用意)
        */
        return sqrt(
            (sign(dot(cross(ab, nor), ap)) +
                sign(dot(cross(bc, nor), bp)) +
                sign(dot(cross(ca, nor), cp)) < 2.0)
            ?
            Math::lowerBound(dot2(ab * clamp(dot(ab, ap) / dot2(ab), 0.0, 1.0) - ap),
                dot2(bc * clamp(dot(bc, bp) / dot2(bc), 0.0, 1.0) - bp),
                dot2(ca * clamp(dot(ca, cp) / dot2(ca), 0.0, 1.0) - cp))
            :
            dot(nor, ap) * dot(nor, ap) / dot2(nor));
    }

    //计算p到三角形abc的无向距离的平方
    inline float ud2Triangle(const vec3& p, const vec3& a, const vec3& b, const vec3& c) {
        vec3 ab = b - a; vec3 ap = p - a;
        vec3 bc = c - b; vec3 bp = p - b;
        vec3 ca = a - c; vec3 cp = p - c;
        //反向法线
        vec3 nor = ab.crossProduct(ca);

        //解释同udTriangle
        return
            (sign(dot(cross(ab, nor), ap)) +
                sign(dot(cross(bc, nor), bp)) +
                sign(dot(cross(ca, nor), cp)) < 2.0)
            ?
            Math::lowerBound(dot2(ab * clamp(dot(ab, ap) / dot2(ab), 0.0, 1.0) - ap),
                dot2(bc * clamp(dot(bc, bp) / dot2(bc), 0.0, 1.0) - bp),
                dot2(ca * clamp(dot(ca, cp) / dot2(ca), 0.0, 1.0) - cp))
            :
            dot(nor, ap) * dot(nor, ap) / dot2(nor);
    }

	//点到平面的有向距离
	inline float sdPointPlane(const pt3& pt, const Plane& plane) {
		return dot(pt, plane.normal) - plane.sd;
	}

	//点到线段的无向距离平方
	inline float ud2PointSegment(const pt3& pt, const Segment3D& segment) {
        vec3 ab = segment.end - segment.start;
        vec3 ac = pt - segment.start;
        vec3 bc = pt - segment.end;
        
        float e = dot(ac, ab);
        if (e <= 0.0) return dot(ac, ac);

        float f = dot(ab, ab);
        if (e >= f) return dot(bc, bc);

        return dot(ac, ac) - e * e / f; //勾股定理
	}

    //点到AABB的无向平方距离
    inline float ud2PointAABB(const pt3& pt, const AABB& box) {
        vec3 v = pt - box.mins_;

        float dx = 0;
        if (v.x < 0) dx = v.x;
        float lx = box.maxs_.x - box.mins_.x;
        if (v.x > lx) dx = v.x - lx;

        float dy = 0;
        if (v.y < 0) dy = v.y;
        float ly = box.maxs_.y - box.mins_.y;
        if (v.y > ly) dy = v.y - ly;

        float dz = 0;
        if (v.z < 0) dz = v.z;
        float lz = box.maxs_.z - box.mins_.z;
        if (v.z > lz) dz = v.z - lz;

        return dx * dx + dy * dy + dz * dz;
    }
    
    //点到OBB的无向距离平方
    inline float ud2PointOBB(const pt3& pt, const OBB& box) {

    }
}


#endif
