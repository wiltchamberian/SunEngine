/*****************************************************************************
* @brief : HintRecordTest
* @author : acedtang
* 静止对象之间的碰撞检测，输出HitRecord
* @date : 2021/8/5
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __HIT_RECORD_TEST_H
#define __HIT_RECORD_TEST_H

#include "HitRecord.h"
#include "Math/Geometry.h"

namespace Sun {

	//直线和三角形
	HitRecord hitLineTriangle(const pt3& pt, const vec3& dir, const Triangle& tri) {
		HitRecord rec;
		return rec;
	}
	
	//线段和三角形
	//思路：三角形可以表示为T(u,v)= A+v(B-A)+w(C-A) (v>=0,w>=0,v+w<=1)
	//直线 R(t)=P+t(Q-P)
	//联立方程可以解出v,w,t
	/*
	[(P-Q)(B-A)(C-A)](t,v,w)^T = [P-A]
	令n=(B-A)*(C-A) ,d=(P-Q)^Tn e=(P-Q)*(P-A) 有
	t=(P-A)^T n/d  v=(C-A)^T e/d  w=-(B-A)^T e/d
	*/
	HitRecord hitSegmentTriangle(const Segment3D& segment, const Triangle& tri) {
		HitRecord rd;
		rd.isHit = false;
		rd.frac = 1.0;
		vec3 ab = tri.v1 - tri.v0;
		vec3 ac = tri.v2 - tri.v0;

		vec3 n = cross(ab, ac);
		float d = dot(segment.start - segment.end, n);
		if (d<=0) {
			return rd;
		}
		float t = dot(segment.start - tri.v0, n);
		if (t < 0 || t>d ) {
			return rd;
		}

		vec3 e = cross(segment.start - segment.end, segment.start - tri.v0);
		float v = dot(ac, e);
		if (v<0.0 || v>d) {
			return rd;
		}
		float w = -dot(ab, e);
		if (w<0.0 || (v+w)>d) {
			return rd;
		}
		
		float ood = 1.0 / d;
		t *= ood;
		rd.frac = t;
		rd.isHit = true;

		return rd;
	}
}

#endif