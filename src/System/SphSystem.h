/*****************************************************************************
* @brief : SphSystem
* Smoothed Particle Hydrodynamics 平滑粒子系统（流体仿真)
* @author : acedtang
* @date : 2021/6/18
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SPH_SYSTEM_H
#define __SPH_SYSTEM_H

#include <vector>
#include <list>
#include <functional>
#include "Physics/Particle.h"
#include "Physics/Grid3D.h"
#include "Physics/Physics.h"
#include "Common/Events.h"
#include "System/SystemBase.h"

namespace Sun {

	class SphSystem : public SystemBase
	{
	public:
		SphSystem();
        void prepare() override;
		void tick(float dtime, const SEvent& ev) override;

	public:
		Grid3D<std::list<Particle>> grid_;
		std::vector<Particle> particles;
		std::vector<bool> visited_;
		float h_; //should be the same in grid;
		float m_; //the same for all particle
		float K_;
		float rho0_;
		float mu_; //粘稠度
		float sigma_;
		vec3 gravity_;
		float dt_;

		//常规核函数
		std::function<float(const vec3& v, float h)> kernal_general;
		std::function < vec3(const vec3& v, float h)> kernal_general_gradient;
		std::function<float(const vec3& v, float)> kernal_general_lap;

		//粘滞力核函数
		std::function<float(const vec3& v, float h)> kernal_viscosity;
		std::function < vec3(const vec3& v, float h)> kernal_viscosity_gradient;
		std::function<float(const vec3& v, float)> kernal_viscosity_lap;

		//压力核函数
		std::function<float(const vec3& dr, float h)> kernal_pressure;
		std::function < vec3(const vec3& r, float h)> kernal_pressure_gradient;
		std::function<float(const vec3& dr, float)> kernal_pressure_lap;

		//derived
		std::vector<float> pressures_;
		std::vector<vec3> fpressures_;
		std::vector<vec3> viscosities_;
		std::vector<float> colorField_;
		std::vector<vec3> normals_;
		std::vector<vec3> fsurfaces_;
		std::vector<vec3> accs_;
	protected:
		void init();
		void updateRho(float& output ,Particle& particle, const Particle& particle2);
		Hint collisionDection(const vec3& start, const vec3& end ,float r);
	};

}


#endif