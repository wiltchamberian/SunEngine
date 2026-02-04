/*****************************************************************************
* @brief : HintTest
* @author : acedtang
* 判断两个静止物体是否碰撞，函数全部返回bool变量
* @date : 2021/8/4
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __HINT_TEST_H
#define __HINT_TEST_H

#include "Math/Vector3D.h"
#include "Math/Box.h"
#include "Math/Sphere.h"
#include "Math/Geometry.h"
#include "Math/Capsule.h"
#include "Math/Line.h"
#include "Physics/DistanceField.h"
#include "Physics/ClosestTest.h"

namespace Sun {

	//计算点是否在球内部
	inline bool pointInSphere(const pt3& pt, const Sphere& sphere) {
		return (pt - sphere.center).getSquaredLength() <= sphere.getSquareRadius();
	}

	//计算点是否在AABB内部
	inline bool pointInAABB(const pt3& pt, const AABB& box) {
		return (pt.x >= box.mins_.x && pt.y >= box.mins_.y && pt.z >= box.mins_.z)
			&& (pt.x <= box.maxs_.x && pt.y <= box.maxs_.y && pt.z <= box.maxs_.z);
	}

	//判断点是否在平面上
	inline bool pointOnPlane(const pt3& pt, const Plane& plane) {
		float d = dot(pt, plane.normal) - plane.sd;
		return fabs(d) < FLT_EPSILON;
	}

	//点是否在线段上
	inline bool pointOnSegment(const pt3& pt, const Segment3D& segment) {
		pt3 cl = closestPointSegment(pt, segment);
		return dot(pt - cl, pt - cl) < FLT_EPSILON;
	}

	//点是否在射线上
	inline bool pointOnRay(const pt3& pt, const Ray& ray) {
		float d = dot(pt - ray.ori, ray.dir);
		if (d < 0) return false;
		vec3 v = cross(pt - ray.ori, ray.dir);
		return fabs(v.x) < FLT_EPSILON && fabs(v.y) < FLT_EPSILON && fabs(v.z) < FLT_EPSILON;
	}

	//检查两个球体是否碰撞
	inline bool hintSphereSphere(const Sphere& sp1, const Sphere& sp2) {
		float ds = (sp1.center - sp2.center).getSquaredLength();
		float d2 = (sp1.r + sp2.r) * (sp1.r + sp2.r);
		return ds <= d2;
	}

	//球体和AABB是否碰撞
	inline bool hintSphereAABB(const Sphere& sp1, const AABB& aabb) {
		pt3 cl = closestPointAABB(sp1.center, aabb);
		return dot2(cl - sp1.center) <= sp1.r * sp1.r;
	}

	//球体和平面是否相交
	inline bool hintSpherePlane(const Sphere& sp, const Plane& plane) {
		//计算球心到平面的有向距离
		float dis = dot(sp.center, plane.normal) - plane.sd;
		return dis * dis < sp.r * sp.r;
	}

	//点是否位于胶囊体内部 
	/*
	* 思路，计算点到胶囊体中心线段的距离，判断距离是否小于r即可
	*/
	inline bool pointInCapsule(const pt3& pt, const Capsule& capsule) {
		float dis = ud2PointSegment(pt, *(const Segment3D*)(&capsule));
		return dis <= capsule.r * capsule.r;
	}

	//AABB和AABB是否相交,分离轴的思想
	inline bool hintAABB_AABB(const AABB& box1, const AABB& box2) {
		return (box1.mins_.x <= box2.maxs_.x && box1.maxs_.x >= box2.mins_.x)
			&& (box1.mins_.y <= box2.maxs_.y && box1.maxs_.y >= box2.mins_.y)
			&& (box1.mins_.z <= box2.maxs_.z && box1.maxs_.z >= box2.mins_.z);
	}

	//AABB和平面是否相交, SAT,简化为只需考虑平面法线
	inline bool hintAABB_Plane(const AABB& box, const Plane& plane) {
		//找到平面法线所对应的象限
		vec3 v(sign(plane.normal.x), sign(plane.normal.y), sign(plane.normal.z));
		vec3 c = box.center();
		//该象限对应的box顶点和中心对称点
		vec3 v1 = c + v * (box.maxs_ - c);
		vec3 v2 = c * 2 - v1;

		float d1 = dot(v1, plane.normal);
		float d2 = dot(v2, plane.normal);
		if (d2 < d1) std::swap(d1, d2);

		return d1 <= plane.sd && plane.sd <= d2;
	}

	//obb和平面是否相交 SAT,分离轴选择过obb中心，方向为plane法线的直线
	//r=+-(e0 dot(v0,n)+-(e1 dot(v1,n))+- (e2 dot(v2,n)) （8个点的投影方程)
	inline bool hintOBB_Plane(const OBB& obb, const Plane& plane) {
		//8个顶点到分离轴的投影点 和  obb中心 的距离的最大值
		float r = obb.e0 * fabs(dot(obb.v0, plane.normal)) +
			obb.e1 * fabs(dot(obb.v1, plane.normal)) +
			obb.e2 * fabs(dot(obb.v2, plane.normal));
		//obb中心到平面的有向距离
		float d = dot(obb.center, plane.normal) - plane.sd;
		return fabs(d) <= r;
	}

	//线段和AABB是否相交 slab算法
	inline bool hintAABB_Segment(const AABB& box, const vec3& p0, const vec3 p1) {
		float tx0 = (box.mins_.x - p0.x) / (p1.x - p0.x);
		float tx1 = (box.maxs_.x - p0.x) / (p1.x - p0.x);
		float ty0 = (box.mins_.y - p0.y) / (p1.y - p0.y);
		float ty1 = (box.maxs_.y - p0.y) / (p1.y - p0.y);
		float tz0 = (box.mins_.z - p0.z) / (p1.z - p0.z);
		float tz1 = (box.maxs_.z - p0.z) / (p1.z - p0.z);

		float l = Math::upperBound(tx0, ty0, tz0);
		float h = Math::lowerBound(tx1, ty1, tz1);

		return l < h && (l >= 0) && (h <= 1);
	}

	//AABB和球体是否相交 通过计算球中心到box的距离，比较距离和球半径即可
	inline bool hintAABB_Sphere(const AABB& box, const Sphere& sphere) {
		float dist2 = ud2PointAABB(sphere.center, box);		
		return dist2 < sphere.r;
	}

	//OBB和球体是否相交 
	inline bool hintOBB_Sphere(const OBB& box, const Sphere& sphere) {
		//float dist2 = ud2PointOBB()
	}

	//直线和三角形是否相交
	inline bool hintLine_Triangle(const Line& line, const Triangle& tri) {
		vec3 pa = tri.v0 - line.pt;
		vec3 pb = tri.v1 - line.pt;
		vec3 pc = tri.v2 - line.pt;

		float u = scalarTripleProduct(line.dir, pc, pb);
		float v = scalarTripleProduct(line.dir, pa, pc);
		float w = scalarTripleProduct(line.dir, pb, pa);

		return (u > 0 && v > 0 && w > 0) || (u < 0 && v < 0 && w < 0);
	}
}




#endif