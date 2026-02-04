/*****************************************************************************
* @brief : Missile
* 导弹类，
* @author : acedtang
* 物体的物理状态信息结构
* @date : 2021/8/7
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __MISSILE_H
#define __MISSILE_H

#include "Math/Sphere.h"
#include "GameObject.h"
#include "Physics/State.h"

namespace Sun {

	struct MissileCreateInfo {
		State state;
		Sphere sphere;
	};

	class Missile : public GameObject
	{
	public:
		Missile();
		Missile(const MissileCreateInfo& createInfo);
		~Missile();
		void tick(float dtime, const SEvent& ev) override;
		bool supportCollideQuery() const override;
		bool needAddToBVH() const override;
	protected:
	};


}


#endif