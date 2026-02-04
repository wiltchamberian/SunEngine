#ifndef __COMPONENT_TYPE_H
#define __COMPONENT_TYPE_H

namespace Sun {

	enum EComType {
		INVALID_COM,
		//mesh
		MESH_COMPONENT,

		//transform
		TRANSFORM_COM,
		SIMPLE_TRANSFORM_COM,
		LIGHT_TRANSFORM_COM,

		//material
		SIMPLE_MATERIAL_COM,
		PBR_MATERIAL_COM,

		//light
		POINT_LIGHT_COM,
	};


}


#endif