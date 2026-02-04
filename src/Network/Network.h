#ifndef __SUN_NETWORK_H
#define __SUN_NETWORK_H


#include "Export.h"
#include "NetworkEnum.h"
#include <memory>

namespace Sun
{
#define TEST_LOCAL_PORT 68
#define TEST_PORT 9123
#define TEST_PORT_2 9124
#define TEST_PORT_3 9125

#define INIT_BUF_LEN 65536

    class NetWorkImpl;
    class CommuteNode;

    
    class NETWORK_API Network
    {
    public:
        Network();
        ~Network();
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
        std::unique_ptr<NetWorkImpl> impl_;
    };
   
}



#endif