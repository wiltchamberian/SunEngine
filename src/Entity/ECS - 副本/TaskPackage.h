#ifndef __SUN_ECS_PACKAGE_H
#define __SUN_ECS_PACKAGE_H

#include "ComAccessPtr.h"
#include "Chunk.h"

namespace Sun {

	struct TaskPackage_deprecated {
		const Chunk* chunk = nullptr;
		EntityAccessPtr input;
		std::function<void(EntityAccessPtr)> func;
		size_t len = 0;
		//make it be a function
		void operator()() {
			EntityAccessPtr it = input;
			for (size_t i=0;i<len;++i){
				func(it);
				++it;
			}
		}
	};

	struct TaskPackage {
		const Chunk* chunk = nullptr;
		std::function<void()> func;
		void operator()() {
			func();
		}
	};
}

#endif