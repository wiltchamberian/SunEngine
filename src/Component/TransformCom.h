/*****************************************************************************
* @brief : TransformCom
* 变换组件接口 ,主要处理 矩阵变换相关和gpu通信部分
* @author : acedtang
* @date : 2021/7/15
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __TRANSFORM_COM_H
#define __TRANSFORM_COM_H

#include "Render/Program.h"
#include "Math/Matrix4x4.h"
#include "Math/Transform3D.h"
#include "Component/IComponent.h"

namespace Sun {

	class TransformCom : public IComponent
	{
	public:
		virtual void bind(/*const Matrix4x4& view, const Matrix4x4& proj, const Matrix4x4& vp, const vec3& cameraPos, */const ShaderProgram& pro) = 0;		
		virtual void setWorldTransform(const Transform3D& trans) = 0;
		virtual Transform3D getWorldTransform() const = 0;
		virtual void setRelativeTransform(const Transform3D& trans) = 0;
		virtual Transform3D getRelativeTransform() const = 0;
		virtual void setRelativeTranslation(const vec3& v) = 0;


		virtual Matrix4x4 getWorldMatrix() const = 0;

		virtual void scaleRelativeTransform(const vec3& v) = 0;
		virtual void rotateRelativeTransform(const vec3& axis, float radians) = 0;
		virtual void translateRelativeTransform(const vec3& v) = 0;
	};
}


#endif