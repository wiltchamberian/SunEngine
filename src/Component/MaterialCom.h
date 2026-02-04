#ifndef __MATERIAL_COM_H
#define __MATERIAL_COM_H

#include "Render/Program.h"
#include "Component/ComponentType.h"
#include "Component/IComponent.h"

namespace Sun {

	struct MaterialCreateInfo {
		EComType materialType;
		void* createInfo = nullptr;
	};

	class MaterialCom:public IComponent {
	public:
		virtual void bind(const ShaderProgram& pro) = 0;
		virtual void unBind(const ShaderProgram& pro) = 0;

		virtual void allocGpu() = 0;
		virtual void releaseGpu() = 0;
	};

}

#endif