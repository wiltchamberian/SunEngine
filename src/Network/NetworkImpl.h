#ifndef __SUN_NETWORK_IMPL_H
#define __SUN_NETWORK_IMPL_H

#include "ThirdParty/libuv/uv.h"
#include "NetworkEnum.h"

namespace Sun {

	class CommuteNode;

	class NetWorkImpl {

	public:
		NetWorkImpl();
		~NetWorkImpl();

		//初始化网络环境
		void initNetworkEnv();

		//创建通信节点
		CommuteNode* createCommuteNode(EComNodeType nType);

		//销毁通信节点
		void destroyCommuteNode(CommuteNode* node);

		//执行loop
		void run_loop();

		void shutDown();
	protected:
		uv_loop_t* loop_{ nullptr };

	};

	
}

#endif
