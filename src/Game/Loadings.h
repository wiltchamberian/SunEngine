#ifndef __LOADINGS_H
#define __LOADINGS_H

#include <string>
#include "Math/MathUtils.h"
#include "Math/Quaternion.h"
#include "Math/Transform3D.h"
#include "Entity/Pawn.h"
#include "System/SystemBase.h"
#include "GameInclude.h"
#include "RandomizedPrim.h"

namespace Sun {

    class GroundRotateSystem : public SystemBase {
    public:
        virtual void tick(float dtime, const SEvent& ev) {
            static float cumRad = 0;
            static float time = 0;
            time += dtime;
            if (time <= 3) {
                return;
            }
            if (cumRad >= A_PI / 2) {
                return;
            }
            //10s翻转90度
            float degree = 9 * dtime;
            float dRad = degree * A_PI / 180;
            cumRad += dRad;

            //pawn_->relativeTransform_.
            //q.setRotateAxisAndRadius({ 1.0,0.0,0.0 }, dRad);
            //trans.setRotation()

            float R = rotor_->getArmLen();
            float dz = R * cos(dRad) - R;
            float dy = R * sin(dRad);
            rotor_->translateRelativeTransform(vec3(0, dy, dz));
            rotor_->rotateRelativeTransform({ 1.0,0,0 }, -dRad);
        }
        std::shared_ptr<SCameraComponent> rotor_;
    };

    class YRotateSystem : public SystemBase {
    public:
        virtual void tick(float dtime, const SEvent& ev) {
            static float cumRad = 0;
            static float time = 0;
            time += dtime;
            if (time <= 3) {
                return;
            }
            //if (cumRad >= A_PI / 2) {
            //    return;
            //}
 
            float degree = 18 * dtime;
            float dRad = degree * A_PI / 180;
            cumRad += dRad;

            //pawn_->relativeTransform_.
            //q.setRotateAxisAndRadius({ 1.0,0.0,0.0 }, dRad);
            //trans.setRotation()
            //围绕y轴旋转

            actor_->rotate({ 0,1,0 }, dRad);
        }
        GameObject* actor_;
        //sp<GameObject> rotor_;
    };

    class MeshComponent;

    //p1->p2->p3 
    extern std::vector<StaticMeshVertex> buildTriangle(const vec3& p1, const vec3& p2, const vec3& p3 ,bool hasNormal);
    extern std::vector<StaticMeshVertex> buildRectangle(const vec3& p1, const vec3 p2, const vec3 p3, bool hasNormal);    

    //加载默认纹理
    extern std::shared_ptr<MaterialCom> loadMaterial(int id);

    //水体网格 p1是左上角，p2是左下角,p3是右下角,横轴分区n个，纵轴分区m个
    extern GameObject* loadFluid(const vec3& p1,const vec3& p2,const vec3& p3,int n,int m);


    //构建随机迷宫，m*n个格子组成的迷宫，每个各自的尺寸为grid_size,后跟出发点和终止点的坐标
    extern std::vector<GameObject*> createMaze(int m,int n,vec2 grid_size,int start_x,int start_y,int end_x,int end_y,float h);
}

#endif