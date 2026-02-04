#include "DirectLight.h"
#include "BuildIn/buildin.h"

namespace Sun {

	Matrix4x4 DirectLight::getLookAtMatrix() const {
		vec3 pos = getPosition();
		return Matrix4x4::getLookAt(pos, pos + dir_, vec3(0, 1, 0));
	}

	Matrix4x4 DirectLight::getProjMatrix() const {
		return Matrix4x4::getOrtho(left_, right_, bottom_, top_, near_, far_);
	}

	vec3 DirectLight::getDirection() {
		return dir_;
	}

	void DirectLight::setDirection(const vec3& dir) {
		dir_ = dir;
	}

	SharedPtr<MeshComponent> DirectLight::generateViewVolumeMesh() const {
		SharedPtr<MeshComponent> ptr(new MeshComponent);
		AABB box;
		box.mins_ = vec3(left_, bottom_, -far_);
		box.maxs_ = vec3(right_, top_, -near_);
		//box.mins_ = vec3(1, 0, -2);
		//box.maxs_ = vec3(3, 2, 0);
		buildCube(ptr->getVertices(), ptr->getIndices(), box, vec4(1.0, 0.0, 0.0, 1.0));
		auto& vertices = ptr->getVertices();
		Matrix4x4 mat = getLookAtMatrix().getInverseTransform();
		//从视图空间转为世界空间
		for (auto& it : vertices) {
			vec4 tmp = mat * vec4(it.pos.x, it.pos.y, it.pos.z, 1.0);
			it.pos = vec3(tmp.x, tmp.y, tmp.z);
		}
		return ptr;
	}
}