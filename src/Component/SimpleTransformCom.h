/*****************************************************************************
* @brief : SimpleTransformCom
* 简单变换组件，只包含模型矩阵
* @author : acedtang
* @date : 2021/7/15
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SIMPLE_TRANSFORM_COM_H
#define __SIMPLE_TRANSFORM_COM_H

#include "TransformCom.h"
#include "Math/Transform3D.h"

namespace Sun {

	struct SimpleTransformCreateInfo {
		Transform3D relativeTransform_;
	};

	class SimpleTransformCom : public TransformCom
	{
	public:
		SimpleTransformCom();
		SimpleTransformCom(const SimpleTransformCreateInfo& createInfo);
		void bind(const ShaderProgram& pro) override;
		void setWorldTransform(const Transform3D& trans) override;
		Transform3D getWorldTransform() const override;
		void setRelativeTransform(const Transform3D& trans) override;
		Transform3D getRelativeTransform() const override;
		void setRelativeTranslation(const vec3& v) override;
		Matrix4x4 getWorldMatrix() const override;
		void scaleRelativeTransform(const vec3& v) override;
		void rotateRelativeTransform(const vec3& axis, float radians) override;
		void translateRelativeTransform(const vec3& v) override;
		Transform3D relativeTransform_;
		Transform3D worldTransform_;
		vec3 velocity_;
	};


}


#endif