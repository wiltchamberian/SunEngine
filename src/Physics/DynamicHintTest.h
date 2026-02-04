/*****************************************************************************
* @brief : DynamicHintTest
* @author : acedtang
* 运动物体碰撞检测
* @date : 2021/8/5
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __DYNAMIC_HINT_TEST_H
#define __DYNAMIC_HINT_TEST_H

#include "HitRecord.h"
#include "Math/Geometry.h"
#include "Math/Triangle.h"
#include "Math/Cylinder.h"
#include "Math/Capsule.h"
#include "Physics/ClosestTest.h"
#include "Physics/HintTest.h"

namespace Sun {

	//运动的点和静止的三角形（等价于线段和三角形)
	//思路：三角形可以表示为T(u,v)= A+v(B-A)+w(C-A) (v>=0,w>=0,v+w<=1)
	//直线 R(t)=P+t(Q-P)
	//联立方程可以解出v,w,t
	/*
	[(P-Q)(B-A)(C-A)](t,v,w)^T = [P-A]
	令n=(B-A)*(C-A) ,d=(P-Q)^Tn e=(P-Q)*(P-A) 有
	t=(P-A)^T n/d  v=(C-A)^T e/d  w=-(B-A)^T e/d
	*/
	inline HitRecord dhitPointTriangle(const pt3& pStart, const pt3& pEnd, const Triangle& tri) {
		HitRecord rd;
		rd.isHit = false;
		rd.frac = 1.0;
		vec3 ab = tri.v1 - tri.v0;
		vec3 ac = tri.v2 - tri.v0;

		vec3 n = cross(ab, ac);
		float d = dot(pStart - pEnd, n);
		if (d <= 0) {
			return rd;
		}
		float t = dot(pStart - tri.v0, n);
		if (t < 0 || t>d) {
			return rd;
		}

		vec3 e = cross(pStart - pEnd, pStart - tri.v0);
		float v = dot(ac, e);
		if (v<0.0 || v>d) {
			return rd;
		}
		float w = -dot(ab, e);
		if (w<0.0 || (v + w)>d) {
			return rd;
		}

		float ood = 1.0 / d;
		t *= ood;
		rd.frac = t;
		rd.isHit = true;

		return rd;
		
	}

	inline HitRecord dhitRayTriangle(const pt3& pStart, const vec3& dir, const Triangle& tri) {
		HitRecord rd;
		rd.isHit = false;
		rd.frac = 1.0;
		vec3 ab = tri.v1 - tri.v0;
		vec3 ac = tri.v2 - tri.v0;

		vec3 n = cross(ab, ac);
		float d = dot(-dir, n);
		if (d <= 0) {
			return rd;
		}
		float t = dot(pStart - tri.v0, n);
		if (t < 0) {
			return rd;
		}

		vec3 e = cross(-dir, pStart - tri.v0);
		float v = dot(ac, e);
		if (v<0.0 || v>d) {
			return rd;
		}
		float w = -dot(ab, e);
		if (w<0.0 || (v + w)>d) {
			return rd;
		}

		float ood = 1.0 / d;
		t *= ood;
		rd.frac = t;
		rd.isHit = true;

		return rd;
	}

	//射线和球体相交，基本思路解参数方程
	/*
	 射线: R(t)= P+t*d (t>=0)d为归一化射线方向矢量,和球坐标方程联立，得
	 dot(P+t*d-C,P+t*d-C)=r*r ,
	 设m = P-C 有
	 dot(m+td,m+td)=r*r =>
	 dot(d,d)t*t + 2dot(m,d)t+dot(m,m)-r*r=0  (利用dot(d,d)=1) =>
	 t*t+2dot(m,d)t+dot(m,m)-r*r =0
	 令b=dot(m,d) c=dot(m,m)-r*r, 方程解为 t = -b+-sqrt(b*b-c)
	*/
	//输入 ray的方向矢量必须归一化了
	inline HitRecord dhitRaySphere(const Ray& ray, const Sphere& sphere) {
		HitRecord rec;
		rec.frac = MAX_flt;
		rec.isHit = false;
		
		vec3 m = ray.ori - sphere.center;
		float b = dot(m, ray.dir);
		float c = dot(m, m) - sphere.r * sphere.r;
		//c>0表示射线原点位于球外部;b>0表示射线远离球体
		if (c > 0.0 && b > 0.0) {
			return rec;
		}
		float discr = b * b - c;
		//表示无交点
		if (discr < 0) {
			return rec;
		}
		//计算最小的t值
		float t = -b - sqrt(discr);
		if (t < 0) {
			t = 0.0;
		}
		rec.frac = t;
		rec.isHit = true;
		rec.p = ray.ori + ray.dir * t;
		rec.normal = (rec.p - sphere.center).getNormalized();
		return rec;
	}

	//线段和圆柱体的碰撞检测，思路:
	/*
	* 可以列出圆柱的方程（不考虑底部和顶部),和直线的参数方程，联立求解
	*/
	inline HitRecord dhitSegmentCylinder(const pt3& sa, const pt3& sb, const Cylinder& cylinder) {
		HitRecord hit;
		hit.frac = 1.0;
		hit.isHit = false;

		//圆柱的中心线的方向矢量
		vec3 d = cylinder.q - cylinder.p;
		//圆柱底部中心到射线起始点
		vec3 m = sa - cylinder.p;
		//射线方向矢量
		vec3 n = sb - sa;

		float md = dot(m, d);
		float nd = dot(n, d);
		float dd = dot(d, d);

		//说明线段整个位于圆柱底部平面下方，因此不可能相交
		if (md < 0.0 && md + nd < 0.0) {
			return hit;
		}
		//说明线段整个位于圆柱顶部平面上方，则不可能相交
		if (md > dd && md + nd > dd) {
			return hit;
		}
		/*
		 设线段为X= a+t*(b-a) , v = X-p, 则v=a+t*(b-a)-p,
		 令m=a-p, n = b-a, 有 v= m+n*t
		 令w=dot(v,d)/dot(d,d)*d 为v在d方向上的投影,
		 有 dot(v-w,v-w)-r*r = 0
		 将v=m+n*t 代入上式并化简，得到
		 at^2+2bt+c=0 其中
		 a=dot(d,d)dot(n,n)-dot(n,d)^2 
		 b=dot(d,d)dot(m,n)-dot(n,d)dot(m,d)
		 c=dot(d,d)(dot(m,m)-r*r)-dot(m,d)^2
		 t=[-b+-sqrt(bb-ac)]/a
		 把d,n表示为长度乘以单位向量形式，不难看出a=dot(cross(d,n),dot(d,n))，
		 这表示a=0,意味着d,n平行 ,且(a>=0恒成立)
		 同理: c =dot(cross(d,m),cross(d,m))-dot(d,d)*r*r;
		 这表示c<0意味着A位于cylinder的surface内部，反之位于外部
		*/
		float t = 0.;
		float nn = dot(n, n);
		float mn = dot(m, n);
		float k = dot(m, m) - cylinder.r * cylinder.r;
		float a = dd * nn - nd * nd;
		float c = dd * k - md * md;
		if (a < EPSILON_flt) {
			if (c > 0.0f) return hit;
			if (md < 0.0f) {
				t = -mn / nn; //A点位于圆柱底部下方
			}
			else if (md > dd) {
				t = (nd - mn) / nn; //A点位于圆柱顶部上方
			}
			else {
				t = 0.0;
			}
			hit.isHit = true;
			hit.frac = t;
			return hit;
		}
		float b = dd * mn - nd * md;
		float discr = b * b - a * c;
		//无交点
		if (discr < 0) return hit;

		t = (-b - sqrt(discr)) / a;
		//交点位于线段外部
		if (t < 0.0 || t>1.0) return hit;
		//交点低于圆柱底部平面:dot(m+nt,d)=dot(m,d)+tdot(n,d)=md+t*nd
		if (md + t * nd < 0.0f) {
			//nd<=0意味着射线指向与d相反的一侧
			if (nd <= 0) return hit;
			t = -md / nd;

			hit.isHit =  k + 2 * t * (mn + t * nn) <= 0.0f;
			hit.frac = t;
			return hit;
		}
		else if (md + t * nd > dd) {
			if (nd >= 0) return hit;
			t = (dd - md) / nd;
			hit.isHit = k + dd - 2 * md + t * (2 * (mn - nd) + t * nn) <= 0.0f;
			hit.frac = t;
			return hit;
		}
		hit.frac = t;
		hit.isHit = true;
		return hit;
	}

	//射线和胶囊体碰撞检测
	/*
	* 思路基本同上
	*/
	inline HitRecord dhitRayCapsule(const Ray& ray, const Capsule& capsule) {
		HitRecord hit;
		hit.frac = 1.0;
		hit.isHit = false;

		//如果射线起始点位于胶囊体内部
		if (pointInCapsule(ray.ori, capsule)) {
			hit.frac = 0.0;
			hit.isHit = true;
			hit.p = ray.ori;
			return hit;
		}

		vec3 sa = ray.ori;
		vec3 sb = ray.ori + ray.dir;

		//胶囊体的中心线的方向矢量
		vec3 d = capsule.q - capsule.p;
		//胶囊体底部中心到射线起始点
		vec3 m = sa - capsule.p;
		//射线方向矢量
		vec3 n = sb - sa;

		float md = dot(m, d);
		float nd = dot(n, d);
		float dd = dot(d, d);

		
		/*
		 设线段为X= a+t*(b-a) , v = X-p, 则v=a+t*(b-a)-p,
		 令m=a-p, n = b-a, 有 v= m+n*t
		 令w=dot(v,d)/dot(d,d)*d 为v在d方向上的投影,
		 有 dot(v-w,v-w)-r*r = 0
		 将v=m+n*t 代入上式并化简，得到
		 at^2+2bt+c=0 其中
		 a=dot(d,d)dot(n,n)-dot(n,d)^2
		 b=dot(d,d)dot(m,n)-dot(n,d)dot(m,d)
		 c=dot(d,d)(dot(m,m)-r*r)-dot(m,d)^2
		 t=[-b+-sqrt(bb-ac)]/a
		 把d,n表示为长度乘以单位向量形式，不难看出a=dot(cross(d,n),dot(d,n))，
		 这表示a=0,意味着d,n平行 ,且(a>=0恒成立)
		 同理: c =dot(cross(d,m),cross(d,m))-dot(d,d)*r*r;
		 这表示c<0意味着A位于cylinder的surface内部，反之位于外部
		*/
		float t = 0.;
		float nn = dot(n, n);
		float mn = dot(m, n);
		float k = dot(m, m) - capsule.r * capsule.r;
		float a = dd * nn - nd * nd;
		float c = dd * k - md * md;
		if (a < EPSILON_flt) {
			if (c > 0.0f) return hit;
			if (md < 0.0f) {
				t = -mn / nn; //A点位于圆柱底部下方
			}
			else if (md > dd) {
				t = (nd - mn) / nn; //A点位于圆柱顶部上方
			}
			else {
				t = 0.0;
			}
			hit.isHit = true;
			hit.frac = t;
			return hit;
		}
		float b = dd * mn - nd * md;
		float discr = b * b - a * c;
		//无交点
		if (discr < 0) return hit;

		t = (-b - sqrt(discr)) / a;
		//交点位于线段外部
		if (t < 0.0 || t>1.0) return hit;
		//交点低于圆柱底部平面:dot(m+nt,d)=dot(m,d)+tdot(n,d)=md+t*nd
		if (md + t * nd < 0.0f) {
			//nd<=0意味着射线指向与d相反的一侧
			if (nd <= 0) return hit;
			t = -md / nd;

			hit.isHit = k + 2 * t * (mn + t * nn) <= 0.0f;
			hit.frac = t;
			return hit;
		}
		else if (md + t * nd > dd) {
			if (nd >= 0) return hit;
			t = (dd - md) / nd;
			hit.isHit = k + dd - 2 * md + t * (2 * (mn - nd) + t * nn) <= 0.0f;
			hit.frac = t;
			return hit;
		}
		hit.frac = t;
		hit.isHit = true;
		return hit;
	}

	//运动的球体和AABB的检测(FIXME)
	inline HitRecord dhitSphereAABB(const Sphere& sphere, const vec3& d, const AABB& box) {
		HitRecord rec;
		rec.isHit = false;
		rec.frac = 1.0;
		vec3 v = box.maxs_ - box.mins_;

		Plane planes[6];
		planes[0] = Plane(vec3(-1,0,0),-box.mins_.x);
		planes[1] = Plane(vec3(1, 0, 0), box.maxs_.x);
		planes[2] = Plane(vec3(0, -1, 0), -box.mins_.y);
		planes[3] = Plane(vec3(0, 1, 0), box.maxs_.y);
		planes[4] = Plane(vec3(0, 0, -1), -box.mins_.z);
		planes[5] = Plane(vec3(0, 0, 1), box.maxs_.z);
		int clipPlaneId = -1;
		float t = 1.0;
		float d1 = 0.0;
		float d2 = 0.0;
		float projLen = 0.0;
		float enterFrac = -1.0;
		float leaveFrac = 1.0;
		float f = 0.0;
		for (int i = 0; i < 6; ++i) {
			d1 = dot(sphere.center -planes[i].normal*sphere.r , planes[i].normal) - planes[i].sd;
			d2 = dot(sphere.center + d - planes[i].normal * sphere.r, planes[i].normal) - planes[i].sd;
			if (d1 > 0 && d2 > 0) {
				return rec;
			}
			if (d1 <= 0 && d2 <= 0) {
				continue;
			}
			//进入平面
			if (d1 > d2) {
				f = d1 / (d1 - d2);
				f = f < 0 ? 0 : f;
				if (f > enterFrac) {
					enterFrac = f;
					clipPlaneId = i;
				}
			}
			//离开平面
			else {
				f = d1 / (d1 - d2);
				f = f > 1 ? 1 : f;
				if (f < leaveFrac) {
					leaveFrac = f;
				}
			}
		}
		if (enterFrac < leaveFrac && clipPlaneId>=0) {
			rec.isHit = true;
			rec.frac = enterFrac;
			rec.p = sphere.center + d * rec.frac - planes[clipPlaneId].normal * sphere.r;
			rec.normal = planes[clipPlaneId].normal;
		}

		return rec;
	}

	//运动球体和运动的球体 ,
	/*
	思路：根据伽利略变换，可以看成第一个球体静止;第二个球体相对于第一个球体运动
	假设第二个球体球心的运动方程为R(t)= b+t*d;第一个球球心为a
	则碰撞时满足:
	dot(R(t)- a,R(t)-a)=(r1+r2)^2 令 b-a=m ,=>
	dot(d,d)t^2+2dot(m,d)t+dot(m,m)-(r1+r2)^2 = 0
	*/
	//返回的碰撞信息法向矢量为把第一个球看为静止，其上的法向矢量
	inline HitRecord dhitSphereSphere(const Sphere& sphere0, const vec3& d0,
		const Sphere& sphere1, const vec3& d1) 
	{
		HitRecord rec;
		rec.isHit = false;
		rec.frac = MAX_flt;

		//第二个球相对于第1个球的运动分量
		vec3 d = d1 - d0;
		vec3 m = sphere1.center - sphere0.center;

		float a = dot(d, d);
		float b = dot(m, d);
		float c = dot(m, m) - (sphere0.r + sphere1.r) * (sphere0.r + sphere1.r);
		
		float discr = b * b - a * c;
		if (discr < 0 ) {
			return rec;
		}

		float t = (-b - sqrt(discr)) / a;
		if (t < 0.0 || t>1.0) {
			return rec;
		}

		rec.frac = t;
		rec.isHit = true;
		rec.normal = ((sphere1.center - sphere0.center) + (d1 - d0) * t);
		rec.p = rec.normal *(sphere0.r/(sphere0.r+sphere1.r))+sphere0.center+d0*t;
		rec.normal.normalize();

		return rec;
	}

	//运动球体和静止平面
	/*
	* 思路： 参数方程
	* 球心 R(t) = C+d*t;和平面碰撞时
	* dot(R,n)-sd = r; =>
	* dot(C+dt,n)-sd = r
	* dot(C,n)+dot(d,n)*t -sd - r=0;
	* a= dot(d,n) b=dot(C,n)-sd-r
	* a*t+b=0
	*/
	//平面的法线必须已经归一化
	inline HitRecord dhitSpherePlane(const Sphere& sphere, const vec3& d, const Plane& plane) {
		HitRecord rec;
		rec.isHit = false;
		rec.frac = MAX_flt;

		float a = dot(d, plane.normal);
		float b = dot(sphere.center, plane.normal)-plane.sd - sphere.r;

		//运动方向与平面平行
		if (fabs(a) < EPSILON_flt) {
			return rec;
		}
		
		float t = -b / a;
		if (t < 0 || t>1) {
			return rec;
		}

		rec.isHit = true;
		rec.frac = t;
		rec.p = sphere.center + d * t - plane.normal * sphere.r;
		rec.normal = plane.normal;

		return rec;
	}

	//运动球体和线段的碰撞检测
	/*思路：参数方程法
	* 
	*/
	inline HitRecord dhitSphereSegment(const Sphere& sphere, const vec3& d, const Segment3D& segment) {


	}


	//两个凸体的碰撞动态碰撞检测
	inline HitRecord dConvexCollide();
}


#endif