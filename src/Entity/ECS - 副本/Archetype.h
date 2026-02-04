/*****************************************************************************
* @brief : Archetype
* 表示ECS中的原型
* @author : acedtang
* @date : 2021/8/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SUN_ECS_ARCHETYPE_H
#define __SUN_ECS_ARCHETYPE_H

#include <functional>
#include "ComTypeId.h"
#include "Util/Traits/Traits.h"

namespace Sun {

	template <typename...>
	struct TypeTrait {};

	// 因为ClassType是一个模板类，所以用模板的模板参数表示它，即
	// template <typename...> class ClassType 表示ClassType是一个模板类
	// 这样才可以在特化的时候为它添加模板参数Args...
	template <template <typename...> class ClassType, typename... Args>
	struct TypeTrait<ClassType<Args...>>
	{
		template <std::size_t N>
		using type = std::tuple_element_t<N, std::tuple<Args...>>;
	};

	//模板静态for循环
	template<class _Func,std::size_t N>
	struct for_loop {
		for_loop(_Func f) {
			func = f;
		}
		void operator()() {
			for_loop<N - 1> loop(func);
			loop.operator();
			func(N);
		}
		_Func func;
	};
	template<class _Func>
	struct for_loop<_Func, 0>
	{
		for_loop(_Func f) {
			func = f;
		}
		void operator()() {
			return;
		}
		_Func func;
	};

	/// <summary>
	/// //////////
	/// </summary>

	using Archetype_t = size_t;

	template<typename Arg>
	void add(size_t& input) {
		++input;
	}


	template<typename ... _Args>
	class Archetype {
	public:
		friend class ECSMgr;

		template<std::size_t N>
		using type = typename std::tuple_element_t<N, std::tuple<_Args...>>;
		using tuple_type = std::tuple<_Args...>;
		static constexpr size_t size = sizeof...(_Args);

		template<typename _Arg>
		static constexpr size_t typeIndex_v = TGetIndex_v<_Arg, _Args...>;

		static uint32_t getIndex() {
			return index;
		}
		static bool isRegist() {
			return index != -1;
		}
	private:
		static void setIndex(size_t ind) {
			index = ind;
		}
		
		static size_t index;
	};

	template <typename ... Args>
	size_t Archetype<Args... >::index = -1;

	
}


#endif