#include "Network.h"
#include "Network/NetworkImpl.h"
#include "CommuteNode.h"

namespace Sun {

	Network::Network()
	{
		impl_.reset(new NetWorkImpl());
	}

	Network::~Network()
	{
	}

	void Network::initNetworkEnv()
	{
		impl_->initNetworkEnv();
	}

	CommuteNode* Network::createCommuteNode(EComNodeType nType)
	{
		return impl_->createCommuteNode(nType);
	}

	void Network::destroyCommuteNode(CommuteNode* node)
	{
		impl_->destroyCommuteNode(node);
	}

	void Network::run_loop()
	{
		impl_->run_loop();
	}


	void Network::shutDown()
	{
		impl_->shutDown();
	}

}

