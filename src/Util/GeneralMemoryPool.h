/*****************************************************************************
* @brief :GeneralMemoryPool
* 设计和QuickMemoryPool基本一致，不过更加通用，适合作为一些容器的
* 公共分配器，或者大量经常申请释放小对象的公共分配器
*
*  内存池结构图
内存池有多个Chunk组成，
Chunk0 ,Chunk1, Chunk2,....每个Chunk结构相同

单个Chunk结构由多个block组成,Block里存放着用户数据
Chunk:[Block0,Block1,....                  ]

*
* @author : acedtang
* @date : 2021/7/21
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __GENERAL_MEMORY_POOL_H
#define __GENERAL_MEMORY_POOL_H

#include "Common/Define.h"
#include <vector>
#include <cassert>

namespace Sun {

#define CHUNK_SIZE  (16 * 1024)

#define RESIDUE_SIZE   sizeof(GBlock) //for block head
#define POOL_TABLE_SIZE			37    //must <256
#define MAX_BLOCK_SIZE              1024 - RESIDUE_SIZE

	struct GBlock {
		GBlock* next;
	};

	struct GChunk {
		GBlock* blocks;
		int32 blockSize;
	};

	class GeneralMemoryPool {
	public:
		GeneralMemoryPool();

		template<class _T, class ... _Types>
		_T* malloc(_Types&&... _Args);
		template<class _T>
		void free(_T* ptr);

	protected:
		template <class _Objty, class... _Types>
		void construct(_Objty* const _Ptr, _Types&&... _Args) {
			::new ((_Ptr)) _Objty(_STD forward<_Types>(_Args)...);
		}

		template <class _Uty>
		void destroy(_Uty* const _Ptr) {
			_Ptr->~_Uty();
		}

		template<class _T>
		_T* _malloc();

		template<class _T>
		void _free(_T* ptr);

		GBlock* freeLists_[POOL_TABLE_SIZE];
		std::vector<GChunk> chunks_;

		static int32 blockSizeTable_[POOL_TABLE_SIZE];
		static uint8 blockSizeLookUp_[MAX_BLOCK_SIZE + 1];
		static bool blockSizeLookUpInitialized_;
	};

	template<class _T,class ... _Types>
	_T* GeneralMemoryPool::malloc(_Types&&... _Args) {
		_T* ptr = _malloc<_T>();
		::new (ptr) _T(_STD forward<_Types>(_Args)...);
		return ptr;
	}

	template<class _T>
	void GeneralMemoryPool::free(_T* ptr) {
		ptr->~_T();
		_free(ptr);
	}

	template<class _T>
	_T* GeneralMemoryPool::_malloc() {
		static_assert(sizeof(_T) <= MAX_BLOCK_SIZE);
		std::int32_t typeSiz = sizeof(_T);

		int32 index = blockSizeLookUp_[typeSiz];
		assert(0 <= index && index < POOL_TABLE_SIZE);

		if (freeLists_[index]) {
			GBlock* block = freeLists_[index];
			freeLists_[index] = block->next;
			return (_T*)block;
		}

		chunks_.emplace_back();
		GChunk& chunk = chunks_.back();
		chunk.blocks = (GBlock*)(::malloc(CHUNK_SIZE));

		int32 blockSize = blockSizeTable_[index];
		chunk.blockSize = blockSize;
		std::int32_t blockCount = CHUNK_SIZE / blockSize;
		assert(blockCount * typeSiz <= CHUNK_SIZE);

		//将block串为链表
		GBlock* block = chunk.blocks;
		for (std::int32_t i = 0; i < blockCount - 1; ++i) {
			GBlock* next = (GBlock*)((std::int8_t*)block + blockSize);
			block->next = next;
			block = next;
		}
		block->next = nullptr;

		freeLists_[index] = chunk.blocks->next;

		return (_T*)chunk.blocks;
	}

	template<class _T>
	void GeneralMemoryPool::_free(_T* ptr) {
		static_assert(sizeof(_T) <= MAX_BLOCK_SIZE);
		
		int32 index = blockSizeLookUp_[sizeof(_T)];
		assert(index >= 0 && index < POOL_TABLE_SIZE);

		GBlock* block = (GBlock*)ptr;
		block->next = freeLists_[index];
		freeLists_[index] = block;
	}

}


#endif 