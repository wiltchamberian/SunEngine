/*****************************************************************************
* @brief : SystemMgr
* ECS架构中的系统管理
* @author : acedtang
* @date : 2021/8/29
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SUN_ECS_SYSTEMMGR_H
#define __SUN_ECS_SYSTEMMGR_H

#include <functional>
#include "Util/Traits/Traits.h"
#include "ECSGlobal.h"

namespace Sun {

	template<typename... _ComType>
	using FuncArchetype = std::function<void(_ComType*...)>;

	template<typename ... _FuncArchetype>
	class SystemMgr {
	public:
		using FuncArchetypeList = TypeList<_FuncArchetype...>;

		template<typename ... _ComType>
		void addSystem(std::function<void(_ComType...)>&& func);
	protected:
		std::tuple<SVector<_FuncArchetype>...> functions_;
	};
}


#endif