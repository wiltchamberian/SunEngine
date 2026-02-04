/*****************************************************************************
* @brief : Particle
* Particle
* @author : acedtang
* @date : 2021/6/18
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __PARTICLE_H
#define __PARTICLE_H

#include "Math/Vector3D.h"

namespace Sun {

	class Particle {
	public:
		Particle();
		vec3 pos_;
		vec3 velocity_;
		vec3 acc_;
		float rho_;
		float pressure_;
		float radius_;
		int id_;
	};
}


#endif