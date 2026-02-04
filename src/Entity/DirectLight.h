/*****************************************************************************
* @brief : DirectLight
* 定向光源
* @author : acedtang
* @date : 2021/9/2
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __DIRECT_LIGHT_H
#define __DIRECT_LIGHT_H

#include "Light.h"
#include "Math/Vector3D.h"
#include "Component/MeshComponent.h"

namespace Sun {

	class DirectLight : public Light {
	public:
		Matrix4x4 getLookAtMatrix() const override;
		Matrix4x4 getProjMatrix() const override;
		//获取定向光源的方向
		vec3 getDirection();
		void setDirection(const vec3& dir);
		
		SharedPtr<MeshComponent> generateViewVolumeMesh() const;
	protected:
		vec3 dir_;
		float left_ = -10.0;
		float right_ = 10.0;
		float bottom_ = -10.0;
		float top_ = 10.0;
		float near_ = 1.0;
		float far_ = 40.;
	};
}

#endif