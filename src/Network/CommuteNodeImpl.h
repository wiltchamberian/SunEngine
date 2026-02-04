#ifndef __COMMUTE_NODE_IMPL_H
#define __COMMUTE_NODE_IMPL_H

#include "ThirdParty/libuv/uv.h"
#include "NetworkEnum.h"
#include "CommuteNode.h"

namespace Sun {

	class CommuteNodeImpl {
	public:
		friend class CommuteNode;
		friend void recv_cb(uv_udp_t* handle,
			ssize_t nread,
			const uv_buf_t* buf,
			const struct sockaddr* addr,
			unsigned flags);
		friend void alloc_buf_cb(uv_handle_t* handle,
			size_t suggested_size,
			uv_buf_t* buf);
		friend void send_cb(uv_udp_send_t* req, int status);

		CommuteNodeImpl();
		~CommuteNodeImpl();

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

		void initSocket(uv_loop_t* loop);

		void setComNodeType(EComNodeType type);
	protected:
		void setNetAddr(sockaddr_in& addr, const char* ipv4, int port);
		void setNetAddr6(sockaddr_in6& addr, const char* ipv6, int port);

		EComNodeType nodeType_;

		//本地网络地址
		sockaddr_in localAddr_;
		sockaddr_in6 localAddr6_;
		int localPort_;

		//远端网络地址v4
		sockaddr_in remoteAddr_;
		sockaddr_in6 remoteAddr6_;
		int remotePort_;

		//通信socket
		uv_udp_t socket_;

		//接收回调
		RecvCallBack recv_cb_;
		//发送成功回调
		SendCallBack send_cb_;

		//发送缓冲区
		uv_buf_t send_buf_;
		//接收缓冲区
		uv_buf_t recv_buf_;
	};
}


#endif