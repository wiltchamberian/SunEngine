#include "SphSystem.h"
#include "Math/Geometry.h"

namespace Sun {

	SphSystem::SphSystem() {
		init();
	}

	//r=((x-x0)^2+(y-y0)^2+(z-z0)^2)^(1/2),
	//g=kr^n 
	//dr/dx= knr^{n-1}
	void SphSystem::init() {
		//配置默认的核函数
		kernal_general = [](const vec3& v, float h)->float {
			float r = length(v);
			if (r > h) return 0;
			float d = h * h - r * r;
			float res =  315.0 / (64.0 * A_PI * std::powf(h, 9)) * d * d * d;

			return res;
		};

		kernal_viscosity = [](const vec3& v, float h)->float {
			float r = length(v);
			if (r > h) return 0;
			float h3 = h * h * h;
			float res = 15 / (2 * A_PI * h3) * (-r * r * r / (2 * h3) + r * r / (h * h) + h / (2 * r) - 1);
			return res;
		};

		kernal_viscosity_lap = [](const vec3& v, float h)-> float{
			float dx = v.x;
			float r = length(v);
			float h3 = h * h * h;
			
			if (r > h) return 0;
			float h2 = h * h;
			return 45.0 / (A_PI * (h2 * h2 * h2)) * (h - r);
		};

		//debrun's spiky kernel
		kernal_pressure = [](const vec3& v, float h)->float {
			float r = length(v);
			if (r > h) return 0;

			float h2 = h * h;
			return 15.0 * (h - r) * (h - r) * (h - r) / (A_PI * h2 * h2 * h2);
		};

		kernal_pressure_gradient = [](const vec3& v, float h)->vec3 {
			float r = length(v);
			if (r > h) return vec3(0, 0, 0);

			float h2 = h * h;
			float h6 = h2 * h2 * h2;
			float item = (-45)  * (h - r) * (h - r) / r / (A_PI * h6);

			return vec3(v.x * item, v.y * item, v.z * item);
		};
	}

    void SphSystem::prepare() {

    }

	void SphSystem::tick(float dtime, const SEvent& ev) {

		static float totalTime = 0;
		totalTime += dtime;

		//更新密度
		Index3D index;
		int mark[3] = {-1,0,1};

		//更新密度
		for (Particle& particle:particles) {
			index = grid_.getIndex(particle.pos_);
			float rho = 0;
			for (int i = 0; i < 3;++i) {
				for (int j = 0; j < 3; ++j) {
					for (int k = 0; k < 3; ++k) {
						Index3D tmp = index;
						tmp.x += mark[i];
						tmp.y += mark[j];
						tmp.z += mark[k];
						std::list<Particle>* lis = grid_.getData(tmp);
						if (lis) {
							for (auto& it : (*lis)) {
								rho += kernal_general(particle.pos_ - it.pos_, h_) * m_;
							}
						}
					}
				}
			}
			particle.rho_ = rho;
			particle.pressure_ = K_ * (particle.rho_ - rho0_);
		}
		//更新压力
		int id = 0;
		for (Particle& particle : particles) {
			index = grid_.getIndex(particle.pos_);
			vec3 fpressure;
			for (int i = 0; i < 3; ++i) {
				for (int j = 0; j < 3; ++j) {
					for (int k = 0; k < 3; ++k) {
						Index3D tmp = index;
						tmp.x += mark[i];
						tmp.y += mark[j];
						tmp.z += mark[k];
						std::list<Particle>* lis = grid_.getData(tmp);
						if (lis) {
							for (auto& it : (*lis)) {
								fpressure += -kernal_pressure_gradient(particle.pos_- it.pos_, h_) * m_ * (particle.pressure_ + particles[it.id_].pressure_)*0.5 / particles[it.id_].rho_;
							}
						}
					}
				}
			}
			fpressures_[id] = fpressure;
			id += 1;
		}
		//更新粘滞阻力
		id = 0;
		for (Particle& particle : particles) {
			index = grid_.getIndex(particle.pos_);
			vec3 viscosity;
			for (int i = 0; i < 3; ++i) {
				for (int j = 0; j < 3; ++j) {
					for (int k = 0; k < 3; ++k) {
						Index3D tmp = index;
						tmp.x += mark[i];
						tmp.y += mark[j];
						tmp.z += mark[k];
						std::list<Particle>* lis = grid_.getData(tmp);
						if (lis) {
							for (auto& it : (*lis)) {
								viscosity += (it.velocity_ - particle.velocity_) * kernal_viscosity_lap(particle.pos_- it.pos_, h_) * mu_* m_ /it.rho_;
							}
						}
					}
				}
			}
			viscosities_[id] = viscosity;
			id += 1;
		}
		//表面张力
		id = 0;
		for (Particle& particle : particles) {
			index = grid_.getIndex(particle.pos_);
			float cs = 0.0;
			vec3 normal;
			for (int i = 0; i < 3; ++i) {
				for (int j = 0; j < 3; ++j) {
					for (int k = 0; k < 3; ++k) {
						Index3D tmp = index;
						tmp.x += mark[i];
						tmp.y += mark[j];
						tmp.z += mark[k];
						std::list<Particle>* lis = grid_.getData(tmp);
						if (lis) {
							for (auto& it : (*lis)) {
								cs += kernal_general(particle.pos_-it.pos_,h_)*m_*(1.0/it.rho_);
								normal += kernal_general_gradient(particle.pos_- it.pos_, h_) * m_ * (1.0 / it.rho_);
							}
						}
					}
				}
			}
			colorField_[id] = cs;
			normals_[id] = normal;
			id += 1;
		}
		id = 0;
		for (Particle& particle : particles) {
			index = grid_.getIndex(particle.pos_);
			float cs = 0.0;
			vec3 fsurface;
			for (int i = 0; i < 3; ++i) {
				for (int j = 0; j < 3; ++j) {
					for (int k = 0; k < 3; ++k) {
						Index3D tmp = index;
						tmp.x += mark[i];
						tmp.y += mark[j];
						tmp.z += mark[k];
						std::list<Particle>* lis = grid_.getData(tmp);
						if (lis) {
							for (auto& it : (*lis)) {
								fsurface += normals_[id].getNormalized() * (-sigma_) * kernal_general_lap(particle.pos_- it.pos_, h_) * m_ * (1.0 / it.rho_);
							}
						}
					}
				}
			}
			fsurfaces_[id] = fsurface;
			id += 1;
		}
		//外部力

		//计算粒子的加速度,更新粒子状态
		id = 0;
		for (auto& particle : particles) {
			if (visited_[id] == false) {
				Index3D ind = grid_.getIndex(particle.pos_);
				std::list<Particle>* lis = grid_.getData(ind);
				if (lis) {
					for (std::list<Particle>::iterator it = lis->begin(); it != lis->end();) {
						visited_[it->id_] = true;

						//update particle state
						accs_[it->id_] = (fsurfaces_[it->id_] + fpressures_[it->id_] + viscosities_[it->id_] + gravity_) / particles[it->id_].rho_;
						//it->velocity_ += accs_[it->id_] * dt_;
						//it->pos_ += it->velocity_ * dt_;
						vec3 v = it->velocity_ + accs_[it->id_] * dt_;
						vec3 pos = it->pos_ + it->velocity_ * dt_;

						//check if happened collide
						Hint hint = collisionDection(it->pos_, pos, it->radius_);
						if (hint.isHint) {
							//如果发生碰撞，则先把球体移动到碰撞点接触处，再把反射 速度矢量
							it->pos_ = hint.hintPt + hint.normal * it->radius_;
							it->velocity_ = reflect(-it->velocity_, hint.normal);
						}
						else {
							it->velocity_ = v;
							it->pos_ = pos;
						}

						//update state
						particles[it->id_] = *it;

						//erase all
						it = lis->erase(it);
					}
				}
			}
			++id;
		}
		id = 0;
		for (auto& particle : particles) {
			Index3D ind = grid_.getIndex(particle.pos_);
			std::list<Particle>* lis = grid_.getData(ind);
			if (lis) {
				lis->insert(lis->end(), particle);
			}
		}
		
		//reset for next iter
		for (int i = 0; i < visited_.size(); ++i) {
			visited_[i] = false;
		}
	}

	void SphSystem::updateRho(float& output, Particle& particle, const Particle& particle2){
		
	}

	Hint SphSystem::collisionDection(const vec3& start, const vec3& end, float r) {
		Hint hint;
		hint.t = 1;
		hint.isHint = false;

		vec3 d = end - start;
		//目前用最简单的碰撞
		//底面检测
		if (end.y - r <= 0) {
			float dy = end.y - r;
			hint.t = (r-start.y) / (end.y-start.y);
			hint.hintPt = start + d * hint.t + vec3(0,-1,0)*r;
			hint.normal = vec3(0, 1, 0);
			hint.isHint = true;
		}
		
		float xlen = grid_.getXLen();
		float zlen = grid_.getZLen();
		//四个面检测
		if (end.x - r <= 0) {
			float t = (r -start.x) / (end.x - start.x);
			if (t < hint.t) {
				hint.t = t;
				hint.hintPt = start + d * hint.t + vec3(-1, 0, 0) * r;
				hint.normal = vec3(1, 0, 0);
				hint.isHint = true;
			}
		}
		if (end.z - r <= 0) {
			float t = (r - start.z) / (end.z - start.z);
			if (t < hint.t) {
				hint.t = t;
				hint.hintPt = start + d * hint.t + vec3(0, 0, -1) * r;
				hint.normal = vec3(0, 0, 1);
				hint.isHint = true;
			}
		}
		if (end.x + r >= xlen) {
			float t = (xlen - r - start.x) / (end.x - start.x);
			if (t < hint.t) {
				hint.t = t;
				hint.hintPt = start + d * hint.t + vec3(1, 0, 0) * r;
				hint.normal = vec3(-1, 0, 0);
				hint.isHint = true;
			}
		}
		if (end.z + r >= zlen) {
			float t = (zlen - r - start.x) / (end.x - start.x);
			if (t < hint.t) {
				hint.t = t;
				hint.hintPt = start + d * hint.t + vec3(0, 0, 1) * r;
				hint.normal = vec3(0, 0, -1);
				hint.isHint = true;
			}
		}

		return hint;
	}
}