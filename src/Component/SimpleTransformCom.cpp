#include "SimpleTransformCom.h"
#include "Render/GraphicsLib.h"

namespace Sun {

	SimpleTransformCom::SimpleTransformCom() {

	}

	SimpleTransformCom::SimpleTransformCom(const SimpleTransformCreateInfo& createInfo) {
		relativeTransform_ = createInfo.relativeTransform_;
		worldTransform_ = createInfo.relativeTransform_;
	}

	void SimpleTransformCom::bind(/*const Matrix4x4& view, const Matrix4x4& proj,const Matrix4x4& vp, const vec3& cameraPos, */const ShaderProgram& pro) {
		unsigned int id = pro.getId();
		//g_graphicsLib->setMat4(id, U_VP, vp.data(), 1);
		g_graphicsLib->setMat4(id, U_MODEL, getWorldMatrix().data(), 1);
		
		Transform3D tf = getWorldTransform();
		g_graphicsLib->setVec4(id, U_TMODEL_TRANSLATE, tf.translation_);
		g_graphicsLib->setVec4(id, U_TMODEL_QUA, (vec4&)tf.rotation_);
		g_graphicsLib->setVec4(id, U_TMODEL_SCALE, tf.scale_);
	}

	void SimpleTransformCom::setWorldTransform(const Transform3D& trans) {
		worldTransform_ = trans;
	}

	Transform3D SimpleTransformCom::getWorldTransform() const {
		return worldTransform_;
	}

	void SimpleTransformCom::setRelativeTransform(const Transform3D& trans) {
		relativeTransform_ = trans;
	}

	Transform3D SimpleTransformCom::getRelativeTransform() const {
		return relativeTransform_;
	}

	void SimpleTransformCom::setRelativeTranslation(const vec3& v) {
		relativeTransform_.setTranslation(v);
	}

	Matrix4x4 SimpleTransformCom::getWorldMatrix() const {
		return worldTransform_.toMatrix();
	}

	void SimpleTransformCom::scaleRelativeTransform(const vec3& v) {
		relativeTransform_.scale(v);
	}

	void SimpleTransformCom::rotateRelativeTransform(const vec3& axis, float radians) {
		relativeTransform_.rotate(axis, radians);
	}

	void SimpleTransformCom::translateRelativeTransform(const vec3& v) {
		relativeTransform_.translate(v);
	}
}