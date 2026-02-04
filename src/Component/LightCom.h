#ifndef __LIGHT_COM_H
#define __LIGHT_COM_H

#include "Render/Program.h"
#include "Common/Events.h"
#include "Component/MeshInterface.h"

namespace Sun {

	class LightCom : public MeshInterface
	{
	public:
		void allocGpu(EDrawMode drawTYpe) override {}
		void releaseGpu() override {}
	};

}


#endif