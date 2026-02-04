/*****************************************************************************
* @brief : Resource
* 资源类，组件继承该类，从而可以支持被资源管理器管理
* @author : acedtang
* @date : 2021/8/13
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SUN_RESOURCE_H
#define __SUN_RESOURCE_H

#include "Common/Define.h"

namespace Sun {

	class IResource {
	public:
		virtual Hash getHashValue() {
			return hash_;
		}
		virtual void setHashValue(Hash hash) {
			hash_ = hash;
		}
		Hash hash_;
	};
}

#endif