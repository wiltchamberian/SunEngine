/*****************************************************************************
* @brief : LightTransformCom
* 支持光照处理的变换组件（包含世界位置信息，法线矩阵信息传递逻辑)
* @author : acedtang
* @date : 2021/7/15
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __LIGHT_TRANSFORM_COM_H
#define __LIGHT_TRANSFORM_COM_H

#include "SimpleTransformCom.h"

namespace Sun {

	struct LightTransformCreateInfo {
		Transform3D relativeTransform_;
	};

	class LightTransformCom : public SimpleTransformCom {
	public:
		LightTransformCom(const LightTransformCreateInfo& createInfo = LightTransformCreateInfo());
		void bind(const ShaderProgram& pro) override;
	};
}


#endif