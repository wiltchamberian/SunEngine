/*****************************************************************************
* @brief : DistanceTest
* @author : acedtang
* 计算两个物体之间的距离
* @date : 2021/8/4
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __CLOSEST_TEST_H
#define __CLOSEST_TEST_H

#include "Math/Geometry.h"
#include "Math/Triangle.h"

namespace Sun {

	//计算点到球体最近点
	inline pt3 closestPointSphere(const pt3& pt, const Sphere& sphere) {
		vec3 v = (pt - sphere.center).getNormalized();
		return sphere.center + v * sphere.r;
	}


	//计算点到AABB最近点
	//思路：假设target为所求，作矢量pt-target,不难看出，这个矢量的任意一个分量
	//的取值，取决于在pt的对应分量和box的mins_,maxs_的对应分量之间的关系
	inline pt3 closestPointAABB(const pt3& pt, const AABB& box) {
		float dx1 = pt.x - box.maxs_.x;
		float dx2 = pt.x - box.mins_.x;
		float dx = dx1 > 0 ? dx1 : (dx2 < 0 ? dx2 : 0);

		float dy1 = pt.y - box.maxs_.y;
		float dy2 = pt.y - box.mins_.y;
		float dy = dy1 > 0 ? dy1 : (dy2 < 0 ? dy2 : 0);

		float dz1 = pt.z - box.maxs_.z;
		float dz2 = pt.z - box.mins_.z;
		float dz = dz1 > 0 ? dz1 : (dz2 < 0 ? dz2 : 0);

		return pt - vec3(dx, dy, dz);
	}


	//点到平面的最近点
	inline pt3 closestPointPlane(const pt3& pt, const Plane& plane) {
		float d = dot(pt, plane.normal) - plane.sd;
		return pt - plane.normal * d;
	}

	
	//点到线段的最近点
	inline pt3 closestPointSegment(const pt3& pt, const Segment3D& segment) {
		vec3 v = segment.end - segment.start;
		float t = dot(pt - segment.start, v) / dot(v, v);
		t = Math::upperBound(t, 0.0);
		t = Math::lowerBound(t, 1.0);
		return segment.start + v * t;
	}

	//点到射线最近的点
	inline pt3 closestPointRay(const pt3& pt, const Ray& ray) {
		float d = dot(pt - ray.ori, ray.dir);
		d = d < 0 ? 0 : d;
		return ray.ori+ ray.dir * d;
	}

	//点到OBB的最近点(pt位于obb内部时返回pt)
	inline pt3 closestPointOBB(const pt3& pt, const OBB& obb) {
		pt3 res = obb.center;
		float dist = 0;
		//将pt投影到obb坐标系下并据此计算最近点
		vec3 v = pt - obb.center;
		dist = dot(v, obb.v0);
		if (dist > obb.e0)dist = obb.e0;
		if (dist < -obb.e0) dist = -obb.e0;
		res += obb.v0 * dist;
		dist = dot(v, obb.v1);
		if (dist > obb.e1)dist = obb.e1;
		if (dist < -obb.e1) dist = -obb.e1;
		res += obb.v1 * dist;
		dist = dot(v, obb.v2);
		if (dist > obb.e2) dist = obb.e2;
		if (dist < -obb.e2) dist = -obb.e2;
		res += obb.v2 * dist;

		return res;
	}

	//点到三角形的最近点
	inline pt3 closestPointTriangle(const pt3& pt, const Triangle& tri) {

	}

}

#endif