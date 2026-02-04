#include "Network/NetworkImpl.h"
#include "Network/CommuteNode.h"
#include "Network/CommuteNodeImpl.h"
#include "Util/exception.h"
#include <cassert>

namespace Sun {

	NetWorkImpl::NetWorkImpl()
	{
	}

	NetWorkImpl::~NetWorkImpl()
	{
	}

	//初始化网络环境
	void NetWorkImpl::initNetworkEnv() {
		loop_ = uv_default_loop();
		loop_->data = this;
	}

	CommuteNode* NetWorkImpl::createCommuteNode(EComNodeType nType)
	{
		CommuteNode* node = new CommuteNode();
		node->impl_->initSocket(loop_);
		node->impl_->setComNodeType(nType);
		return node;
	}

	void NetWorkImpl::destroyCommuteNode(CommuteNode* node)
	{
		delete node;
	}

	void NetWorkImpl::run_loop()
	{
		int ret =  uv_run(loop_, UV_RUN_DEFAULT);
		if (ret != 0) {
			assert(false);
		}

	}

	static void close_walk_cb(uv_handle_t* handle, void* arg) {
		if (!uv_is_closing(handle))
			uv_close(handle, NULL);
	}

	void NetWorkImpl::shutDown()
	{
		uv_walk(loop_, close_walk_cb, NULL);
		uv_run(loop_, UV_RUN_DEFAULT);
		ASSERT(0 == uv_loop_close(uv_default_loop()));  
		uv_library_shutdown();                          
	}

	/////////////////////////////////////////////////////////////////////////////

	

}