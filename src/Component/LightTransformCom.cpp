#include "LightTransformCom.h"
#include "Render/GraphicsLib.h"

namespace Sun {

	LightTransformCom::LightTransformCom(const LightTransformCreateInfo& createInfo) 
	{
		relativeTransform_ = createInfo.relativeTransform_;
		worldTransform_ = createInfo.relativeTransform_;
	}

	void LightTransformCom::bind(/*const Matrix4x4& view,const Matrix4x4& proj, const Matrix4x4& vp, const vec3& cameraPos, */const ShaderProgram& pro) {
		unsigned int id = pro.getId();
		Matrix4x4 model = getWorldMatrix();

		//g_graphicsLib->setMat4(id, U_VP, vp.data(), 1);

		g_graphicsLib->setMat4(id, U_MODEL, model.data(), 1);
		Transform3D tf = getWorldTransform();
		g_graphicsLib->setVec4(id, U_TMODEL_TRANSLATE, tf.translation_);
		g_graphicsLib->setVec4(id, U_TMODEL_QUA, (vec4&)tf.rotation_);
		g_graphicsLib->setVec4(id, U_TMODEL_SCALE, tf.scale_);


		//g_graphicsLib->setVec3(pro.getId(), U_VIEWPOS, cameraPos);

		
	}

}