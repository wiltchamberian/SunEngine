#include "SystemMgr.h"

template<typename ... _FuncArchetype>
template<typename ... _ComType>
void Sun::SystemMgr< _FuncArchetype...>::addSystem(std::function<void(_ComType...)> && func) {
	using MyFuncArchetype = FuncArchetype<_ComType...>;
	static_assert(TContain_v < FuncArchetypeList, MyFuncArchetype > ==  true);

	std::get<SVector<MyFuncArchetype>>(functions_).emplace_back(func);
}