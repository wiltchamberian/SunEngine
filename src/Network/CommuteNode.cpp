#include "CommuteNode.h"
#include "CommuteNodeImpl.h"

namespace Sun {

	CommuteNode::CommuteNode()
	{
		impl_.reset(new CommuteNodeImpl);
	}

	CommuteNode::~CommuteNode()
	{

	}

	void CommuteNode::setLocalNetAddr(const char* ip, int port, ENetWorkType type /*= ENetWorkType::INET*/)
	{
		impl_->setLocalNetAddr(ip, port, type);
	}

	void CommuteNode::setRemoteNetAddr(const char* ip, int port, ENetWorkType type /*= ENetWorkType::INET*/)
	{
		impl_->setRemoteNetAddr(ip, port, type);
	}

	void CommuteNode::registRecvCallBack(const RecvCallBack& func)
	{
		impl_->registRecvCallBack(func);
	}

	void CommuteNode::registSendCallBack(const SendCallBack& func)
	{
		impl_->registSendCallBack(func);
	}

	void CommuteNode::recvStart()
	{
		impl_->recvStart();
	}

	void CommuteNode::sendMsgToRemote(const char* data, int len)
	{
		impl_->sendMsgToRemote(data, len);
	}

	void CommuteNode::sendMsgToRemote(const char* data, int len, const struct sockaddr* addr)
	{
		impl_->sendMsgToRemote(data, len, addr);
	}

	void CommuteNode::resetSendBuf(unsigned int len)
	{
		impl_->resetSendBuf(len);
	}

	void CommuteNode::resetRecvBuf(unsigned int len)
	{
		impl_->resetRecvBuf(len);
	}

	void CommuteNode::broadCastMsg()
	{
		impl_->broadCastMsg();
	}

	void CommuteNode::connectToRemote()
	{
		impl_->connectToRemote();
	}
}