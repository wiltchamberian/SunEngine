#ifndef __RESOURCE_MANAGER_H
#define __RESOURCE_MANAGER_H

#include "Common/Define.h"
#include "Util/Singleton.h"
#include "Util/GeneralMemoryPool.h"
#include "Component/Resource.h"
#include <unordered_map>

namespace Sun {

	class ResourceMgr{
	public:
		void insert(Hash hash , SharedPtr<IResource> ele) {
			elements.insert(std::make_pair(hash, ele));
		}
		bool hasElement(Hash hash) {
			return elements.find(hash) != elements.end();
		}
		template<class _T>
		SharedPtr<_T> getElement(Hash hash) {
			return std::dynamic_pointer_cast<_T>(elements[hash]);
		}
		template<class _T ,class ... _Args>
		SharedPtr<_T> createElement(Hash hash , _Args&&... args) {
			//SharedPtr<_T> ptr(getGlobalComponentMememoryPool()->malloc<_T>(_STD forward<_Args>(args)...));
			SharedPtr<_T> ptr(new _T(_STD forward<_Args>(args)...));
			ptr->setHashValue(hash);
			elements.emplace(std::make_pair(hash, ptr));
			return ptr;
		}

		std::unordered_map<Hash, SharedPtr<IResource>> elements;
	};

	using GResourceMgr = Singleton<ResourceMgr>;

	extern ResourceMgr* getResourceMgr();
}

#endif