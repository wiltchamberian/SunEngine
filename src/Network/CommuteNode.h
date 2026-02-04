#ifndef __SUN_COMMUTE_NODE_H
#define __SUN_COMMUTE_NODE_H

#include <functional>
#include <memory>
#include "NetworkEnum.h"
#include "Export.h"
#include "ThirdParty/libuv/uv.h"

namespace Sun {

	using RecvSignature = void(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags);
	using RecvCallBack = std::function<void(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags)>;
	using SendCallBack = std::function<void(uv_udp_send_t* req, int status)>;

	class Network;
	class CommuteNodeImpl;

	class NETWORK_API CommuteNode {
	public:
		friend class NetWorkImpl;
		CommuteNode();
		~CommuteNode();

		//设置本地网络地址
		void setLocalNetAddr(const char* ip, int port, ENetWorkType type = ENetWorkType::INET);

		//设置远端网络地址
		void setRemoteNetAddr(const char* ip, int port, ENetWorkType type = ENetWorkType::INET);

		//注册接收回调
		void registRecvCallBack(const RecvCallBack& func);

		//注册发送回调
		void registSendCallBack(const SendCallBack& func);

		//开始接收远端消息
		void recvStart();

		//向远端发送消息
		void sendMsgToRemote(const char* data, int buf);

		void sendMsgToRemote(const char* data, int len, const struct sockaddr* addr);

		//重载发送缓冲区的大小
		void resetSendBuf(unsigned int len);

		//重置接收缓冲区的大小
		void resetRecvBuf(unsigned int len);

		//广播消息，用于服务器
		void broadCastMsg();

		void connectToRemote();

	protected:
		std::unique_ptr<CommuteNodeImpl> impl_;

		Network* network_ = nullptr;
	};

}


#endif