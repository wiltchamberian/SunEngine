#include "CommuteNodeImpl.h"
#include "Network.h"
#include <cassert>


namespace Sun {

	inline const struct sockaddr* cast_addr(const struct sockaddr_in* addr) {
		return (const struct sockaddr*)(addr);
	}

	void alloc_buf_cb(uv_handle_t* handle,
		size_t suggested_size,
		uv_buf_t* buf) {
		CommuteNodeImpl* net = (CommuteNodeImpl*)(handle->data);
		buf->base = net->recv_buf_.base;
		buf->len = net->recv_buf_.len;
	}

	void recv_cb(uv_udp_t* handle,
		ssize_t nread,
		const uv_buf_t* buf,
		const struct sockaddr* addr,
		unsigned flags) {
		CommuteNodeImpl* net = (CommuteNodeImpl*)(handle->data);
		net->recv_cb_(handle, nread, buf, addr, flags);
	}

	void send_cb(uv_udp_send_t* req, int status) {
		CommuteNodeImpl* net = (CommuteNodeImpl*)(req->data);
		net->send_cb_(req, status);
		free(req);
	}

	CommuteNodeImpl::CommuteNodeImpl()
	{
		recv_buf_.base = new char[INIT_BUF_LEN];
		recv_buf_.len = INIT_BUF_LEN;
		send_buf_.base = new char[INIT_BUF_LEN];
		send_buf_.len = INIT_BUF_LEN;
	}

	CommuteNodeImpl::~CommuteNodeImpl()
	{
		if (recv_buf_.base != nullptr) {
			delete[] recv_buf_.base;
		}
		if (send_buf_.base != nullptr) {
			delete[] send_buf_.base;
		}
	}

	void CommuteNodeImpl::setLocalNetAddr(const char* ip, int port, ENetWorkType type /*= ENetWorkType::INET*/)
	{
		if (type == ENetWorkType::INET) {
			setNetAddr(localAddr_, ip, port);
		}
		else if (type == ENetWorkType::INET6) {
			setNetAddr6(localAddr6_, ip, port);
		}
		else {
			//TODO
		}
	}

	void CommuteNodeImpl::setRemoteNetAddr(const char* ip, int port, ENetWorkType type /*= ENetWorkType::INET*/)
	{
		if (type == ENetWorkType::INET) {
			setNetAddr(remoteAddr_, ip, port);
		}
		else if (type == ENetWorkType::INET6) {
			setNetAddr6(remoteAddr6_, ip, port);
		}
		else {
			//TODO
		}
	}

	void CommuteNodeImpl::registRecvCallBack(const RecvCallBack& func)
	{
		recv_cb_ = func;
	}

	void CommuteNodeImpl::registSendCallBack(const SendCallBack& func)
	{
		send_cb_ = func;
	}

	void CommuteNodeImpl::recvStart()
	{
		if (nodeType_ == EComNodeType::SERVER) {
			int ret = uv_udp_bind(&socket_, cast_addr(&localAddr_), 0);
			if (ret != 0) {
				assert(false);
			}
		}
		socket_.data = this;
		int ret = uv_udp_recv_start(&socket_, alloc_buf_cb, recv_cb);
		if (ret != 0) {
			assert(false);
		}
	}

	void CommuteNodeImpl::sendMsgToRemote(const char* data, int len)
	{
		uv_udp_send_t* req = (uv_udp_send_t*)malloc(sizeof(*req));
		assert(len <= send_buf_.len);
		req->data = this;

		memcpy(send_buf_.base, data, len);
		send_buf_.len = len;

		int ret = uv_udp_send(req, &socket_, &send_buf_, 1, (const struct sockaddr*)&remoteAddr_, send_cb);
		if (ret != 0) {
			assert(false);
		}
	}

	void CommuteNodeImpl::sendMsgToRemote(const char* data, int len, const struct sockaddr* addr)
	{
		uv_udp_send_t* req = (uv_udp_send_t*)malloc(sizeof(*req));
		assert(len <= send_buf_.len);
		req->data = this;

		memcpy(send_buf_.base, data, len);
		send_buf_.len = len;

		int ret = uv_udp_send(req, &socket_, &send_buf_, 1, addr, send_cb);
		if (ret != 0) {
			assert(false);
		}
	}

	void CommuteNodeImpl::resetSendBuf(unsigned int len)
	{
		if (send_buf_.len == len) {
			return;
		}
		if (send_buf_.base != nullptr) {
			delete[] send_buf_.base;
			send_buf_.base = nullptr;
		}

		send_buf_.base = new char[len];
		send_buf_.len = len;
	}

	void CommuteNodeImpl::resetRecvBuf(unsigned int len)
	{
		if (recv_buf_.len == len) {
			return;
		}
		if (recv_buf_.base != nullptr) {
			delete[] recv_buf_.base;
			recv_buf_.base = nullptr;
		}

		recv_buf_.base = new char[len];
		recv_buf_.len = len;
	}

	void CommuteNodeImpl::broadCastMsg()
	{

	}

	void CommuteNodeImpl::connectToRemote()
	{
		uv_udp_bind(&socket_, cast_addr(&remoteAddr_), 0);
		//uv_udp_set_broadcast(&send_socket_, 1);

		//将发送socket连接到远端网络地址
		int ret = uv_udp_connect(&socket_, (const struct sockaddr*)&remoteAddr_);


		if (ret != 0) {
			assert(false);
		}
	}

	void CommuteNodeImpl::initSocket(uv_loop_t* loop)
	{
		int ret = uv_udp_init(loop, &socket_);
		if (ret != 0) {
			assert(false);
		}
	}

	void CommuteNodeImpl::setComNodeType(EComNodeType type)
	{
		nodeType_ = type;
	}

	void CommuteNodeImpl::setNetAddr(sockaddr_in& addr, const char* ipv4, int port)
	{
		int ret = uv_ip4_addr(ipv4, port, &addr);
		if (ret != 0) {
			assert(false);
		}
	}

	void CommuteNodeImpl::setNetAddr6(sockaddr_in6& addr, const char* ipv6, int port)
	{
		int ret = uv_ip6_addr(ipv6, port, &addr);
		if (ret != 0) {
			assert(false);
		}
	}
}