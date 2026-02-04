/*****************************************************************************
* @brief : State
* @author : acedtang
* 物体的物理状态信息结构
* @date : 2021/8/6
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __STATE_H
#define __STATE_H

#include "Math/Vector3D.h"

namespace Sun {

	struct State {
		//位置
		pt3 pos;
		//速度
		vec3 velocity;
		//初始速率(默认方向)
		float speed;
		//加速度(单位m/s^2)
		vec3 accel;
		//质量倒数 （单位:kg)
		float invMass;
	};



}

#endif