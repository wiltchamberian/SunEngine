#ifndef __PHYSICAL_STATE_COM_H
#define __PHYSICAL_STATE_COM_H

#include "Physics/State.h"
#include "Component/LightTransformCom.h"
#include "Component/IComponent.h"

namespace Sun {

	struct PhysicalStateCreateInfo{
		//速度
		vec3 velocity;
		//初始速率(默认方向)
		float speed;
		//加速度(单位m/s^2)
		vec3 accel;
		//质量倒数 （单位:kg)
		float invMass;
	};

	class PhysicalStateCom : public IComponent
	{
	public:
		PhysicalStateCom() {}
		PhysicalStateCom(const PhysicalStateCreateInfo& createInfo) :
			velocity(createInfo.velocity)
			,speed(createInfo.speed)
			,accel(createInfo.accel)
			,invMass(createInfo.invMass)
		{
		}
		//速度
		vec3 velocity;
		//初始速率(默认方向)
		float speed = 0.0;
		//加速度(单位m/s^2)
		vec3 accel;
		//质量倒数 （单位:kg)
		float invMass =1.0;
	};
}

#endif