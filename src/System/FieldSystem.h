/*****************************************************************************
* @brief : Field
* 场的基类，场用来表示作用于关卡中一大类物体的实体，举例来说重力场等;
* 其功能是根据物体的初始物理状态计算目标估计物理状态
* @author : acedtang
* @date : 2021/8/7
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __FIELD_H
#define __FIELD_H

#include "Math/Vector3D.h"
#include "System/SystemBase.h"
#include "Util/QuickVector.h"

namespace Sun {

	class GameObject;
	class SLevel;

	using Acceleration = vec3;

	struct FieldCreateInfo {
		Acceleration accel;
		SLevel* level = nullptr;
	};

	class FieldSystem : public SystemBase
	{
	public:
        FieldSystem(const FieldCreateInfo& createInfo);

        void prepare() override;

		virtual void tick(float dtime, const SEvent& ev) override;

		//力量可以随着时间和空间变化
		virtual Acceleration getAcceleration(const vec3& worldPos, float time);
		
		Acceleration accel_;

		SLevel* level_ = nullptr;
	};
}


#endif 