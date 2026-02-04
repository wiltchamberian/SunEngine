#ifndef __SIMPLE_MATERIAL_COM_H
#define __SIMPLE_MATERIAL_COM_H

#include "Component/MaterialCom.h"
#include "Common/Common.h"
#include "Primitive/Texture.h"

namespace Sun {

	struct SimpleMaterialCreateInfo {
		std::string texture_path;
	};

	class SimpleMaterialCom : public MaterialCom
	{
	public:
		SimpleMaterialCom();
		SimpleMaterialCom(const SimpleMaterialCreateInfo& createInfo);

		void bind(const ShaderProgram& pro) override;
		void unBind(const ShaderProgram& pro) override;

		void allocGpu() override;
		void releaseGpu() override;

		Texture texture;
	protected:
		void init(const SimpleMaterialCreateInfo& createInfo);
		
	};


}


#endif