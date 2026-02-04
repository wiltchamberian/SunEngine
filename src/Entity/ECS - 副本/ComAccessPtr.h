/*****************************************************************************
* @brief : ComAccessPtr
* 提供一种方式从原型管理器管理的内存中访问组件的方法
* @author : acedtang
* @date : 2021/8/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SUN_ECS_COM_ACCESS_PTR_H
#define __SUN_ECS_COM_ACCESS_PTR_H

#include <array>
#include "ComTypeId.h"
#include "Util/base/containers/flat_map.h"
#include "Entity.h"
#include "Util/Traits/Traits.h"

namespace Sun {

	class ECSMgr;

	class ComAccessPtr {
	public:
		friend class EntityAccessPtr;
		ComAccessPtr():id_(-1){

		}
		ComAccessPtr(ComTypeId id, size_t align_siz, std::uint8_t * p) noexcept
			: id_{ id }, p_{ p }, align_siz_(align_siz){}

		void* get() const noexcept { return p_; }

		ComTypeId getComTypeId() const noexcept { return id_; }

		bool valid() const noexcept { return p_ != nullptr; }

		size_t getAlignSize() const { return align_siz_; }

		template<typename _ComType>
		_ComType* as() const noexcept { return reinterpret_cast<_ComType*>(p_); }

		ComAccessPtr& operator++() {
			p_ += align_siz_;
			return *this;
		}

		ComAccessPtr& operator++(int) {
			p_ += align_siz_;
			return *this;
		}
	protected:
		ComTypeId id_;
		size_t align_siz_ = 0;
		std::uint8_t* p_ = nullptr;
	};

	class EntityAccessPtr {
	public:
		void addComponentPtr(ComAccessPtr ptr) {
			ptrs_.insert(std::make_pair(ptr.id_, ptr));
		}
		template<class _ComType>
		ComAccessPtr getComponentPtr(ECSMgr* mgr) const;
		EntityAccessPtr& operator++() {
			for (auto& it : ptrs_) {
				++it.second;
			}
			return *this;
		}
		template<size_t I>
		ComAccessPtr getComAccessPtr() const {
			return (ptrs_.begin() + I)->second;
		}
		EntityAccessPtr& operator++(int) {
			for (auto& it : ptrs_) {
				++it.second;
			}
			return *this;
		}
	protected:
		base::flat_map<ComTypeId, ComAccessPtr> ptrs_;
	};

	template<class _ComType>
	ComAccessPtr getEntityAccessPtrComponent(const EntityAccessPtr& ptr, ECSMgr* mgr);

	template<class _ComType>
	_ComType* getEntityAccessPtrComPtr(const EntityAccessPtr& ptr, ECSMgr* mgr);

	template<size_t I>
	ComAccessPtr getComAccessPtr(const EntityAccessPtr& ptr, ECSMgr* mgr);

	template<typename _ComType, size_t I>
	_ComType* getComPtr(const EntityAccessPtr& ptr);
#if 0
	template<class _T>
	struct TComAccessPtr {
		_T* p_ = nullptr;
		size_t align_siz_ = 0;
		TComAccessPtr& operator++() {
			auto t = (std::uint8_t*)p_;
			t += align_siz_;
			p_ = (_T*)(t);
		}
	};

	template<typename _T, typename _SrcTuple, size_t K>
	void each_get(_T& tr, _SrcTuple& vec) {
		std::get<K>(tr) = (vec[K]).p_;
	}
	template<typename _Tuple, typename _SrcTuple, typename _T, size_t ... N>
	void for_each_seq(_Tuple& tr, _SrcTuple& vec, std::integer_sequence<_T, N...> seq) {
		(each_get<_Tuple, _SrcTuple, N>(tr, vec), ...);
	}

	template<typename _SrcTuple, size_t K>
	void each_add(_SrcTuple& vec) {
		++vec[K];
	}
	template<typename _SrcTuple, typename _T, size_t ... N>
	void for_each_add(_SrcTuple& vec, std::integer_sequence<_T, N...> seq) {
		(each_add<_SrcTuple, N>(vec), ...);
	}

	template<typename ... _ComType>
	class TEntityAccessPtr {
	public:
		constexpr // since C++14
			std::tuple<_ComType&...> tie(_ComType&... args) noexcept {
			return { args... };
		}
		std::tuple<_ComType&...> get() {
			std::tuple<_ComType&...> res;
			for_each_seq(res, ptrs_, std::tuple_size_v< std::tuple<TComAccessPtr...>>);
			return res;
		}

		TEntityAccessPtr<_ComType...> operator++ (int){
			for_each_add(ptrs_, std::tuple_size_v< std::tuple<TComAccessPtr...>>);
			return *this;
		}


	protected:
		std::tuple<TComAccessPtr<_ComType>...> ptrs_;
	};
#endif

#if 0
	//原型迭代器，通过该迭代器可以高效率的迭代某个原型中的所有entity的所有组件
	template<typename ... _ComType>
	class ArchetypeIter
	{
	public:
		using _This = ArchetypeIter<_ComType...>;

		std::tuple<_ComType*...> pointers;
		std::array<size_t, sizeof...(_ComType)> sizes;
		std::array<size_t, sizeof...(_ComType)> offsets;

		//标记目标entity在原型中的总实体偏移量
		size_t offset = 0;
		//标记原型的单个chunk的容量
		static constexpr size_t capacity = 0;
		static constexpr size_t blockSiz = 1<<14;

		Chunk** chunks = nullptr;

		template<class _Arg>
		_Arg& get() {
			size_t chunkIdx = offset / capacity; //TODO:this may optimize to 2^n or by some other strategy
			size_t idxInChunk = offset % capacity;
			constexpr size_t comIndex = GetIndex_v<_Arg, _ComType...>;
			return *(_Arg*)(chunks[chunkIdx]->data_ + offsets[comIndex] + sizes[comIndex] * idxInChunk);
		}

		template<typename ... _Args>
		friend bool operator == (const ArchetypeIter<_Args...>& my, const ArchetypeIter<_Args...>& other) const;
		
		_This& operator++ () {
			++offset;
			return *this;
		}
		_This& operator++ (int) {
			ArchetypeIter tmp = *this;
			++offset;
			return tmp;
		}
		_This& operator-- () {
			--offset;
			return *this;
		}
		_This& operator-- (int) {
			ArchetypeIter tmp = *this;
			--offset;
			return tmp;
		}
	};

	template<typename ... _Args>
	inline bool operator == (const ArchetypeIter<_Args...>& my, const  ArchetypeIter<_Args...>& other) const {
		return (my.chunks == other.chunks) && (my.offset == other.offset);
	}

#endif 

	
}

#endif