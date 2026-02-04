/*****************************************************************************
* @brief : capsule
* 胶囊体
* @author : acedtang
* @date : 2021/3/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __CAPSULE_COMPONENT_H
#define __CAPSULE_COMPONENT_H

namespace Sun {
    class SCapsuleComponent {
    public:
        inline float& halfHeight() {
            return _halfHeight;
        }
        inline float& radius() {
            return _radius;
        }
    protected:
        //中心到顶部或底部半球末端的长度
        float _halfHeight;     //should >=_radius;
        //顶部或底部半球的半径
        float _radius;                 
    };
}

#endif