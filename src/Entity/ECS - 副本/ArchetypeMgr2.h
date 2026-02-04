/*****************************************************************************
* @brief : ArchetypeMgr
* 对单个原型以及相关数据进行管理的类
* @author : acedtang
* @date : 2021/8/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SUN_ECS_ARCHETYPE_MGR2_H
#define __SUN_ECS_ARCHETYPE_MGR2_H

#include <vector>
#include "Chunk.h"
//#include "Entity.h"
#include "ComTypeId.h"
#include "ComAccessPtr.h"
#include "Util/base/containers/flat_map.h"

namespace Sun {

	class Entity;

	class ArchetypeMgr2 {
	public:
		template<typename ... _Archetypes>
		friend class ECSMgr2;

		//接收entity标签，创建object
		[[deprecated]] EntityAddress createObject(Entity entity);

		[[deprecated]] size_t getEntityNum() const;

		//删除object
		[[deprecated]] size_t erase(const EntityAddress& addr);

		[[deprecated]] EntityAddress requestEntry();

		ArchetypeId getArchetypeId() const;

		ComAccessPtr getAccessPtr(const ComTypeId& id, const EntityAddress& addr);
		template<class _ComType>
		ComAccessPtr getAccessPtr(const EntityAddress& addr);

		ComAccessPtr getAccessPtr(const ComTypeId& id, Chunk* chunk, size_t idxInChunk);
		template<class _ComType>
		ComAccessPtr getAccessPtr(Chunk* chunk, size_t idxInChunk);

		template<typename ... _ComType>
		EntityAccessPtr getChunkHeadEntityAccessPtr(Chunk* chunk);

		template<class _ComType>
		void addComponent(const _ComType& com, const EntityAddress& addr);

		template<class _T>
		void setComponent(const EntityAddress& info);

		[[deprecated]] ComTypeInfo getComponentInfo(const ComTypeId& id);

		[[deprecated]] SVector<Chunk*>& getChunks();
	protected:
		[[deprecated]] void assignObject(const EntityAddress& addr);
		[[deprecated]] inline bool validation(const EntityAddress& addr) const;
		[[deprecated]]inline Chunk* applyChunk();
		[[deprecated]] inline void moveEntity(void* dst, void* src);

		//ECSMgr2* mgr_;

		//base::flat_map<size_t, size_t> index_to_offset_;
		

		////该原型对应的组件在mgr_中的索引集合
		//SVector<size_t> comTypeIndexes_;
		// 
		////组件在object内部的偏移量，长度与types_相同
		//SVector<size_t> offsets_;

		//当前原型中所有entity的个数
		int numEntity_ = 0;

		SVector<Chunk*> chunks_;
		//始终指向最小索引的未满chunk，如果不存在，则为-1
		int indexToChunk_ = -1;
		
		
		//SVector<size_t> freeChunks_;

		/*********这里记录索引组件需要的信息***********/
		//该原型中任意chunk所能容纳的最大object个数
		size_t capacity_ =0;
		//该原型中单个object占用的字节数(含align)
		size_t objectSize_ =0;
		//entity 偏移量始终为0
		const size_t entityOffset_ = 0;
		//eneity align size
		size_t entityAlignSiz_ = 0;

		ArchetypeId id_;
		//和ArchetypeId一一对应，每个组件起始位置在chunk中的偏移量
		base::flat_map<ComTypeId, size_t> offsets_;
	};

}

#endif