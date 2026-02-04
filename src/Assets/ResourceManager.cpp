#include "ResourceManager.h"
#include <cassert>

namespace Sun {

	ResourceMgr* getResourceMgr() {
		return Singleton<ResourceMgr>::get();
	}
}