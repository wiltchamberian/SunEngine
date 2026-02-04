/*****************************************************************************
* @brief :ECSGlobal
* ECS一些公共全局变量
* @author : acedtang
* @date : 2021/8/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SUN_ECS_GLOBAL_H
#define __SUN_ECS_GLOBAL_H

#include <cstdint>
#include <vector>
#include <functional>
#include <atomic>
#include "Util/QuickVector.h"

namespace Sun {

	class Chunk;

	template<class _T>
	using SVector = std::vector<_T>;

	struct TypeIndex_t {
		uint32_t index = -1;
	};


	struct TypeExtraction {
		using type = size_t;
		template<class _T>
		static type extract() {
			return typeid(_T).hash_code();
		}
	};

	using ComTypeIndex_t = size_t;
	template<typename _Com>
	class ComTypeIndex {
	public:
		friend class ECSMgr;
		//friend class ComTypeId;
		static ComTypeIndex_t getIndex() {
			return index_;
		}
		static void setIndex(ComTypeIndex_t index) {
			index_ = index;
		}
		static bool isRegist() {
			return index_ != -1;
		}
	private:
		static ComTypeIndex_t index_;
	};
	template <typename _Com>
	ComTypeIndex_t ComTypeIndex<_Com>::index_ = -1;

	//组件类型id，唯一标记一种组件类型
	//using ComTypeId = TypeExtraction::type;

	using ObjTypeId = SVector<TypeExtraction::type>;

	class ArchetypeMgr;
	struct EntityAddress {
		std::size_t chunkIdx{ static_cast<std::size_t>(-1) };
		//Chunk* chunk{ nullptr };
		std::size_t idxInChunk{ static_cast<std::size_t>(-1) };
	};
	struct EntityInfo : public EntityAddress{
		ArchetypeMgr* archetypeMgr{ nullptr };
		std::uint32_t version{ 0 }; // version
	};

	struct MemberFunctions {

		//缺省placement构造函数
		std::function<void(void*)> default_constructor;

		//拷贝构造函数
		std::function<void(void*, const void*)> copy_constructor;

		//移动构造函数
		std::function<void(void*, void*)> move_constructor;

		//移动赋值
		std::function<void(void*, void*)> move_assignment;

		//析构函数
		std::function<void(void*)> destructor;
	};
	
	struct ComTypeInfo : public MemberFunctions {
		size_t siz = 0 ;
		size_t align_siz = 0;
	};

	
#define SUN_ECS_ALIGNMENT  4

	
}


#endif