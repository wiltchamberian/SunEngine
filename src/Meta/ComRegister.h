/*****************************************************************************
* @brief : ComRegister
* 组件注册器
* 以静态方式注册GameObject中所可能用到的组件，标签唯一标记该组件
* 不同类型的组件可以标记相同标签，相同标签的组件在GameObject中同一时刻最多只能存在一份实例
* 如果GameObject添加了未注册的组件会在编译器给出错误提示
* @author : acedtang
* @date : 2021/8/14
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/


#ifndef __COM_REGISTER_H
#define __COM_REGISTER_H

#include "Common/Define.h"
#include "Component/MeshComponent.h"
#include "Component/PbrMaterialCom.h"
#include "Component/AABBCom.h"
#include "Component/PhysicalStateCom.h"
#include "Component/InstancedMeshCom.h"
#include "Component/SkeletalMeshComponent.h"
#include "Component/BVSphereCom.h"


namespace Sun {

	template<class _T>
	struct ComTag {
	public:
		inline static constexpr uint16 tag = -1;
	};

	//resister tag
	template<>
	struct ComTag< MeshInterface>{
		inline static uint16 constexpr tag = static_cast<uint16>(0);
	};
	template<>
	struct ComTag< MeshComponent> {
		inline static uint16 constexpr tag = static_cast<uint16>(0);
	};
	template<>
	struct ComTag< SkeletalMeshComponent> {
		inline static uint16 constexpr tag = static_cast<uint16>(0);
	};

	template<>
	struct ComTag< MaterialCom> {
		inline static uint16 constexpr tag = static_cast<uint16>(1);
	};
	template<>
	struct ComTag< PbrMaterialCom> {
		inline static uint16 constexpr tag = static_cast<uint16>(1);
	};

	template<>
	struct ComTag< BoundingVolumeCom> {
		inline static uint16 constexpr tag = static_cast<uint16>(2);
	};
	template<>
	struct ComTag< AABBCom> {
		inline static uint16 constexpr tag = static_cast<uint16>(2);
	};
	template<>
	struct ComTag<BVSphereCom> {
		inline static uint16 constexpr tag = static_cast<uint16>(2);
	};

	template<>
	struct ComTag< PhysicalStateCom> {
		inline static uint16 constexpr tag = static_cast<uint16>(3);
	};

	template<>
	struct ComTag< TransformCom> {
		inline static uint16 constexpr tag = static_cast<uint16>(4);
	};
	template<>
	struct ComTag< SimpleTransformCom> {
		inline static uint16 constexpr tag = static_cast<uint16>(4);
	};
	template<>
	struct ComTag< LightTransformCom> {
		inline static uint16 constexpr tag = static_cast<uint16>(4);
	};

	template<>
	struct ComTag<InstancedMeshCom> {
		inline static uint16 constexpr tag = static_cast<uint16>(5);
	};
	
}


#endif