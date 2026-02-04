/*****************************************************************************
* @brief :VideoMemoryPool
显存管理内存池，其实并非真正的内存池，只是类似内存池起到管理显存的目的,
基本思想是分配一大块显存，然后在cpu端像内存池一样管理这块显存
*
* @author : acedtang
* @date : 2021/9/25
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __VIDEO_MEMORY_POOL_H
#define __VIDEO_MEMORY_POOL_H

namespace Sun {

	//_T表示显存句柄
	template<typename _T>
	class VideoMemoryPool {
	public:


	};



}

#endif