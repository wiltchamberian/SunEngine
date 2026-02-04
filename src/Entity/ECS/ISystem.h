/*****************************************************************************
* @brief : ISystem
* ECS中的S,表示系统，提供所有系统的基础功能
* @author : acedtang
* @date : 2021/8/21
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __SUN_ECS_ISYSTEM_H
#define __SUN_ECS_ISYSTEM_H

#include <functional>
#include <tuple>
#include "Archetype.h"
#include "TaskPackage.h"
#include "Util/Traits/Traits.h"

namespace Sun {

	class ECSMgrBase;

	class ISystem {
	public: 
		virtual void tick(float dtime) {
			/* this is demo for how subclass implement this function
			registSystemFunc([](int* a,float* b){
				printf("ISystem::tick:%d\n",*a);
			});
			*/
		}
	protected:
		template<typename ... _ComType>
		void registSystemFunc_deprecated(std::function<void(_ComType*...)>&& func);

		template<typename ... _ComType>
		void registSystemFunc(std::function<void(_ComType*...)>&& func);

		ECSMgrBase* mgr_ = nullptr;
	};

	template<typename _TypeInTuple>
	void extractInner(_TypeInTuple& ele, EntityAccessPtr ptr, ECSMgrBase* mgr) {
		ele = ptr.getComponent<_TypeInTuple>(mgr);
	}

	template<typename Tuple, typename _T, size_t... N>
	void extract(Tuple& tp, EntityAccessPtr ptr, ECSMgrBase* mgr, std::integer_sequence<_T, N...> seq) {
		(extractInner<std::tuple_element_t<N, Tuple>>(std::get<N>(tp), ptr,mgr), ...);
	}

	template<typename ... _ComType>
	void ISystem::registSystemFunc_deprecated(std::function<void(_ComType*...)>&& func) {
		
		using tuple_type = typename Archetype<_ComType...>::tuple_type;
		constexpr size_t siz = std::tuple_size_v<tuple_type>;
		auto seq = std::make_integer_sequence<size_t, siz>();

		std::function<void(EntityAccessPtr)> wrapperfun = [&func](EntityAccessPtr ptr) {
#if 0
			//从ptr中提取信息存入inpuptList
			std::tuple<_ComType*...> inputList;
			constexpr size_t siz = std::tuple_size_v<std::tuple<_ComType*...>>;
			auto seq = std::make_integer_sequence<size_t, siz>();
			extract(inputList, ptr,mgr_, seq);

			func(std::get<_ComType*>(inputList)...);
#endif
			constexpr size_t siz = std::tuple_size_v<std::tuple<_ComType...>>;
			auto seq = std::make_integer_sequence<size_t, siz>();
			func(getEntityAccessPtrComPtr<_ComType>(ptr)...);
			//func(getEntityAccessPtrCom<)
		};

		ArchetypeId archetypeId = mgr_->calArchetypeId<_ComType...>();
		SVector<ArchetypeMgr*> vec = mgr_->filterArchetype(archetypeId);
		for (int i = 0; i < vec.size(); ++i) {
			ArchetypeMgr* amgr = vec[i];
			SVector<Chunk*>& chunks = amgr->getChunks();
			for (const Chunk* chunk : chunks) {
				EntityAccessPtr ptr = amgr->getChunkHeadEntityAccessPtr<_ComType...>(chunk);

				TaskPackage package;
				package.chunk = chunk;
				package.input = ptr;
				package.func = wrapperfun;
				package.len = chunk->numEntity_;
			}
		}
	}

	template<typename ... _ComType>
	void ISystem::registSystemFunc(std::function<void(_ComType*...)>&& func) {
		constexpr size_t siz = sizeof...(_ComType);
		ArchetypeId archetypeId = mgr_->calArchetypeId<_ComType...>();
		SVector<ArchetypeMgr*> vec = mgr_->filterArchetype(archetypeId);

		for (int i = 0; i < vec.size(); ++i) {
			ArchetypeMgr* amgr = vec[i];
			SVector<Chunk*>& chunks = amgr->getChunks();
			for (const Chunk* chunk : chunks) {
				EntityAccessPtr ptr = amgr->getChunkHeadEntityAccessPtr<_ComType...>(chunk);
				std::tuple<_ComType*...> inputList;
				auto seq = std::make_integer_sequence<size_t, siz>();
				extract(inputList, ptr, mgr_, seq);

				size_t numEntity = chunk->numEntity_;
				std::function<void()>  wrapperfunc = [inputList, numEntity, func]() {
					constexpr size_t siz = GetLength_v< _ComType...>;
					for (size_t i = 0; i < numEntity; ++i) {
						func(std::get<_ComType*>(inputList)...);
						std::get<_ComType*>(inputList) += sizeof(_ComType)...;
					}
				}

				TaskPackage package;
				package.chunk = chunk;
				package.func = wrapperfun;

				mgr_->addPackage(package);
			}
		}
	}

}


#endif