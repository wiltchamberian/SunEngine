/*****************************************************************************
* @brief :ComTypeId
* 组件类型id,通过它可以唯一索引到组件信息
* @author : acedtang
* @date : 2021/8/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __SUN_ECS_COM_TYPE_ID_H
#define __SUN_ECS_COM_TYPE_ID_H

#include "ECSGlobal.h"
#include <cstdio>
#include "Util/base/containers/flat_set.h"

namespace Sun {

#if 0
	class ComTypeId {
	public:
		ComTypeId() :ptr_(nullptr) {}
		ComTypeId(void* ptr) :ptr_(ptr) {}
		//for hash
		size_t getValue() const {
			return size_t(ptr_);
		}
		friend bool operator < (const ComTypeId& id1, const ComTypeId& id2);
		friend bool operator == (const ComTypeId& id,const ComTypeId& id2) {
			return id.ptr_ == id2.ptr_;
		}
		template<class _ComType>
		static ComTypeId buildComTypeId() {
			ComTypeId id;
			id.ptr_ = &(ComTypeIndex<_ComType>::index_);
			return id;
		}
		size_t getIndex() const {
			return *((size_t*)(ptr_));
		}
	private:
		void* ptr_ = nullptr;
	};

	inline bool operator < (const ComTypeId& id1, const ComTypeId& id2) {
		return id1.ptr_ < id2.ptr_;
	}
#endif 


	using ComTypeId = size_t;

	

	//原型唯一标记
	using ArchetypeId = base::flat_set<ComTypeId>;

	//原型过滤器
	/*
	* 过滤准则:
	* 过滤出同时满足以下三个条件的原型
	* 1,包含all中所有ComType
	* 2,至少包含any中一个ComType
	* 3,不包含none中任何一个ComType
	*/
	struct ArchetypeFilter {
		ArchetypeId all;
		ArchetypeId any;
		ArchetypeId none;
	};
}

#endif