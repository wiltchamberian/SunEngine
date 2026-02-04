#include "Physics.h"
#include "Physics/DistanceField.h"
#include "Math/Geometry.h"

namespace Sun {

    std::pair<float, float> getProjInterval(const Triangle& tri, const vec3& axis) {
        std::pair<float, float> res;

        res.first = dot(tri.v0, axis);
        res.second = res.first;

        for (int i = 1; i < 3; ++i) {
            float v = dot(tri[i], axis);
            res.first = Math::lowerBound(v, res.first);
            res.second = Math::upperBound(v, res.second);
        }

        return res;
    }

    std::pair<float, float>  getProjInterval(const AABB& box, const vec3& axis) {
        std::pair<float, float> res;
        //根据axis指向的象限，可以判断出box上投影最大最小的顶点
        vec3 left;
        vec3 right;
        for (int i = 0; i < 3; ++i) {
            left[i] = axis[i] >= 0 ? box.mins_[i] : box.maxs_[i];
        }
        right = -left;
        res.first = dot(left, axis);
        res.second = dot(right, axis);
        return res;
    }

    bool isRectangleCollide(const BoundingBox& r1, const BoundingBox& r2)
    {
        bool is_not = false;
        is_not = is_not || (r1._center.x + r1._half_x <= r2._center.x - r2._half_x);
        is_not = is_not || (r2._center.x + r2._half_x <= r1._center.x - r1._half_x);
        is_not = is_not || (r1._center.y + r1._half_x <= r2._center.y - r2._half_y);
        is_not = is_not || (r2._center.y + r2._half_y <= r1._center.y - r1._half_y);
        is_not = is_not || (r1._center.z + r1._half_z <= r2._center.z - r2._half_z);
        is_not = is_not || (r2._center.z + r2._half_z <= r1._center.z - r1._half_z);

        return !is_not;
    }

    bool isSphereCollid(const Sphere& p1, const Sphere& p2)
    {
        return (p1.center - p2.center).getSquaredLength() < (p1.r + p2.r) * (p1.r + p2.r);
    }

    bool isAllInFront(const BoundingBox& box, const Plane& plane)
    {
        //boundingBox如果在平面一侧则所有顶点都位于平面一侧，反之亦然
        std::vector<vec3> vec = box.getVertices();
        bool res = false;
        int side;
        for (int i = 0; i < vec.size(); ++i) {
            side = plane.pointSide(vec[i]);
            if (side != PLANE_FRONT) {
                return false;
            }
        }
        return true;
    }

    bool isAllInBack(const BoundingBox& box, const Plane& plane)
    {
        //boundingBox如果在平面一侧则所有顶点都位于平面一侧，反之亦然
        std::vector<vec3> vec = box.getVertices();
        bool res = false;
        int side;
        for (int i = 0; i < vec.size(); ++i) {
            side = plane.pointSide(vec[i]);
            if (side != PLANE_BACK) {
                return false;
            }
        }
        return true;
    }

    bool isCollide(const vec3& center, float radius, const vec3& normal, float dir_dis) {
        float dot = center.dotProduct(normal);
        return fabs(dot - dir_dis) <= radius;
    }

    bool isCollide(const Sphere& sphere, const Plane& plane) {
        float dot = sphere.center.dotProduct(plane.normal);
        return fabs(dot - plane.sd) <= sphere.r;
    }

    bool isCollide(const vec3& center, float r, const vec3& v1, const vec3& v2, const vec3& v3) {
        float d2 = ud2Triangle(center, v1, v2, v3);
        return d2 <= r;
    }

    bool isCollide(const Sphere& sphere, const Triangle& triangle) {
        return isCollide(sphere.center, sphere.r, triangle.v0, triangle.v1, triangle.v2);
    }


    //use SAT 
    bool isCollide(const Triangle& triangle, const AABB& box) {
        //三角形的三个边向量
        vec3 edge1 = triangle.v1 - triangle.v0;
        vec3 edge2 = triangle.v2 - triangle.v1;
        vec3 edge3 = triangle.v0 - triangle.v2;
        //face normal of AABB
        vec3 n1(1, 0, 0);
        vec3 n2(0, 1, 0);
        vec3 n3(0, 0, 1);
        //13个潜在投影轴
        vec3 axis[13] = { n1,n2,n3,
            cross(edge1,edge2),
            cross(n1,edge1),cross(n1,edge2),cross(n1,edge3),
            cross(n2,edge1),cross(n2,edge2),cross(n2,edge3),
            cross(n3,edge1),cross(n3,edge2),cross(n3,edge3)
        };
        for (int i = 0; i < 13; ++i) {
            std::pair<float, float> interval1 = getProjInterval(box, axis[i]);
            std::pair<float, float> interval2 = getProjInterval(triangle, axis[i]);
            if (interval1.second < interval2.first || interval2.second < interval1.first) {
                return false;
            }
        }
        return true;
    }

    Hint hint_segment_triangle(const vec3& start, const vec3& end, const vec3& a, const vec3& b, const vec3& c) {
        Hint hint;
        //三角形平面法向量
        vec3 n = cross(b - a, c - a);
        vec3 d = end - start;
        float A = dot(d, n);
        float B = dot(start - a, n);
        hint.t = -B / A;
        hint.hintPt = start + d * hint.t;
        hint.normal = n;
        if (hint.t < 0 || hint.t>1) {
            hint.isHint = false; return hint;
        }
        if (dot(hint.hintPt - a, cross(b - a, n)) <= 0 &&
            dot(hint.hintPt - b, cross(c - b, n)) <= 0
            && dot(hint.hintPt - c, cross(a - c, n)) <= 0) {
            hint.isHint = true; return hint;
        }
        hint.isHint = false;
        return hint;
    }

    /*
    * start:线段起点 end:线段终点
    * cylinderStart:圆柱底部中心点，cylinderEnd:圆柱顶部中心点,
    * r:圆柱半径, v1:标记圆柱面起始部分，v2:标记圆柱面终止部分 (v1,v2,cylinderEnd-cylinderStart）需构成右手系
    */
    Hint hint_segment_cylinder(const vec3& start, const vec3& end, const vec3& cylinderStart, const vec3& cylinderEnd,
        float r, const vec3& v1, const vec3& v2) {
        Hint hint;

        vec3 n = (cylinderEnd - cylinderStart);
        n.normalize();
        
        vec3 d = end - start;

        // a = cylinderStart  , p =start + d*t;
        //|(px-ax,py-ay,pz-az)-[(px-ax)nx+(py-ay)ny+(pz-az)nz](nx,ny,nz)|^2 - r^2 = 0
        //  (k1*t+b1)^2+(k2*t+b2)^2+(k3*t+b3)^2-r^2=0
        float one_minus_nx2 = 1 - n.x * n.x;
        float one_minus_ny2 = 1 - n.y * n.y;
        float one_minus_nz2 = 1 - n.z * n.z;
        float k1 = d.x * one_minus_nx2 - d.y*n.y*n.x - d.z*n.z*n.x;
        float b1 = (start.x - cylinderStart.x) * one_minus_nx2 - (start.y - cylinderStart.y) * n.y * n.x - (start.z - cylinderStart.z) * n.z * n.x;
        float k2 = d.y * one_minus_ny2 - d.x * n.x * n.y - d.z * n.z * n.y;
        float b2 = (start.y - cylinderStart.y) * one_minus_ny2 - (start.x - cylinderStart.x) * n.x * n.y - (start.z - cylinderStart.z) * n.z * n.y;
        float k3 = d.z * one_minus_nz2 - d.x * n.y * n.z - d.y * n.y * n.z;
        float b3 = (start.z - cylinderStart.z) * one_minus_nz2 - (start.x - cylinderStart.x) * n.x * n.z - (start.y - cylinderStart.y) * n.y * n.z;

        float A = k1 * k1 + k2 * k2 + k3 * k3;
        float B = 2 * k1 * b1 + 2 * k2 * b2 + 2 * k3 * b3;
        float C = b1 * b1 + b2 * b2 + b3 * b3 - r * 3;

        float delta = B * B - 4 * A * C;
        if (delta < 0) {
            //hint.hintPt = false; 
            return hint;
        }
        float sq = sqrt(delta);
        float t1 = (-B + sq) / (2 * A);
        float t2 = (-B - sq) / (2 * A);

        if (t1 > t2) std::swap(t1, t2);

        //decide if t1 is ok
        hint.t = t1;
        hint.hintPt = start + d * hint.t;
        hint.normal = (hint.hintPt - cylinderStart - n * (hint.hintPt - cylinderStart).dotProduct(n)).getNormalized();
        //判断法线是否位于区间v1,v2之间
        vec3 dn = cross(v1, v2);
        vec3 w1 = cross(dn, v1);
        vec3 w2 = cross(v2,dn);
        if ((dot(dn,n)>0 && dot(hint.normal, w1)>=0 && dot(hint.normal, w2)>=0)||      
            (dot(dn,n)<0 && (dot(hint.normal,w1)>=0||dot(hint.normal,w2)>=0)))
        {
            hint.isHint = true;
            return hint;
        }
        //decide if t2 is ok
        hint.t = t2;
        hint.hintPt = start + d * hint.t;
        hint.normal = (hint.hintPt - cylinderStart - n * (hint.hintPt - cylinderStart).dotProduct(n)).getNormalized();
        if ((dot(dn, n) > 0 && dot(hint.normal, w1) >= 0 && dot(hint.normal, w2) >= 0) ||
            (dot(dn, n) < 0 && (dot(hint.normal, w1) >= 0 || dot(hint.normal, w2) >= 0)))
        {
            hint.isHint = true;
            return hint;
        }
        hint.isHint = false;

        return hint;
    }

    Hint hint_segment_sphere(const vec3& start, const vec3& end, const vec3& center, float r, const vec3& axis, const vec3& v1, const vec3& v2) {
        Hint hint;

        vec3 d = end - start;
        // s= start;
        // |s+d*t - center|^2 = r^2;  (k1*t+b1)^2+(k2*t+b2)^2+(k3*t+b3)^2-r^2=0
        
        float b1 = start.x - center.x;
        float b2 = start.y - center.y;
        float b3 = start.z - center.z;

        float A = d.x * d.x + d.y * d.y + d.z * d.z;
        float B = 2 * b1 * d.x + 2 * b2 * d.y + 2 * b3 * d.z;
        float C = b1 * b1 + b2 * b2 + b3 * b3 - r * r;
        float delta = B * B - 4 * A * C;
        
        if (delta < 0) {
            hint.isHint = false; return hint;
        }

        float sq = sqrt(delta);
        float t1 = (-B + sq) / (2 * A);
        float t2 = (-B - sq) / (2 * A);

        if (t1 > t2) std::swap(t1, t2);

        //check t1
        hint.t = t1;
        hint.hintPt = start + d * hint.t;
        hint.normal = (hint.hintPt - center).getNormalized();
        
        vec3 w1 = cross(axis, v1);
        vec3 w2 = cross(v2, axis);
        vec3 dn = cross(v1, v2);

        if ((dot(dn, axis) > 0 && dot(w1,hint.normal)>=0 && dot(w2,hint.normal)>=0)
            ||((dot(dn,axis)< 0) && dot(w1,hint.normal)>=0 || dot(w2,hint.normal)>=0)) {
            hint.isHint = true; return hint;
        }
        //check t2
        hint.t = t2;
        hint.hintPt = start + d * hint.t;
        hint.normal = (hint.hintPt - center).getNormalized();

        if ((dot(dn, axis) > 0 && dot(w1, hint.normal) >= 0 && dot(w2, hint.normal) >= 0)
            || ((dot(dn, axis) < 0) && dot(w1, hint.normal) >= 0 || dot(w2, hint.normal) >= 0)) {
            hint.isHint = true; return hint;
        }

        hint.isHint = false;
        return hint;
    }

    Hint isSphereTraceTriangleCollide(const vec3& start, const vec3& end, float r, const vec3& a, const vec3& b, const vec3& c) {
        Hint hint;

        //求出三角形面片以r为半径的外包络
        
        vec3 ab = b - a;
        vec3 ac = c - a;
        vec3 n = cross(ab, ac).getNormalized();

        vec3 up = n * r;

        Hint hints[8];
        hints[0] = hint_segment_triangle(start,end,a+up,b+up,c+up);
        hints[1] = hint_segment_triangle(start, end, a -up, b - up, c - up);
        hints[2] = hint_segment_cylinder(start, end, a, b, r, up, -up);
        hints[3] = hint_segment_cylinder(start, end, b, c, r, up, -up);
        hints[4] = hint_segment_cylinder(start, end, c, a, r, up, -up);
        hints[5] = hint_segment_sphere(start, end, a, r, up, cross(up, ac), cross(ab, up));
        hints[6] = hint_segment_sphere(start, end, b, r, up, cross(up, -ab), cross(c - b, up));
        hints[7] = hint_segment_sphere(start, end, c, r, up, cross(up, b - c), cross(a - c, up));

        //找出其中的碰撞最近的点
        float t = 2;
        for (int i = 0; i < 8; ++i) {
            if (hints[i].isHint) {
                if (hints[i].t < t) {
                    t = hints[i].t;
                    hint = hints[i];
                }
            }
        }

        return hint;
    }
}
