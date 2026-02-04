/*****************************************************************************
* @brief : graphicsCache
* 正如cpu会有资源管理缓冲一样，gpu有时候也需要缓冲区管理
* 重复分配的资源id无需释放，下一回合可以复用；避免重复申请释放的开销
* 该类负责这些事
* @author : acedtang
* @date : 2021/6/7
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __GRAPHICS_CACHE_H
#define __GRAPHICS_CACHE_H

#include "Common/Macros.h"
#include "Common/Common.h"
#include "Primitive/Texture.h"
#include "Primitive/VertexBufferLayout.h"
#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace Sun {

	struct GPUTexture {
		std::string source;
		unsigned int handle;
		unsigned int width;
		unsigned int height;
		unsigned int channel;
		ETextureType type;
		friend bool operator == (const GPUTexture& t1, const GPUTexture& t2);
	};
	bool operator == (const GPUTexture& t1, const GPUTexture& t2);

	class GraphicsCache {
	public:
		unsigned int applyForTexture();
		void unUseTexture(unsigned int );

		void applyForTexture(const std::string& source, Texture& texture);
		void applyForReleaseTexture(const std::string& tex);

	protected:
		std::unordered_set<unsigned int> texHandles_;
		std::vector<unsigned int> unUseHandles_;

		std::unordered_map<std::string,Texture> gpuTextures_;	
		std::unordered_map<std::string,Texture> unuseTextures_;

		//key为着色器名，值为数组，数组中存储着若干chunk,
		//每个chunk表示一块较大的gpu内存（实际是两块，一块vertex,一块index）
		//一个chunk由一个内存池管理
		std::unordered_map<std::string, std::vector<GpuVertexDataHandle>> chunksMap_;
	};

	extern GraphicsCache* g_graphicsCache;
}

#endif