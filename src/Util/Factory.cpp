#include "Factory.h"

namespace Sun {

	GeneralMemoryPool* getGlobalComponentMememoryPool() {
		static GeneralMemoryPool pool;
		return &pool;
	}
}