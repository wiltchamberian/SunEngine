#ifndef __SUN_ECS_LIGHT_H
#define __SUN_ECS_LIGHT_H

#include "GameObject.h"
#include "Math/Matrix4x4.h"

namespace Sun {

	class Light  : public GameObject {
	public:
		virtual Matrix4x4 getLookAtMatrix() const { return Matrix4x4(); }
		virtual Matrix4x4 getProjMatrix() const { return Matrix4x4(); }
	};

}

#endif