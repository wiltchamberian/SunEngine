#ifndef __SUN_AUDIO_IMPL_H
#define __SUN_AUDIO_IMPL_H

#include "Audio.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "Util/LRUCache.h"
#include "ThirdParty/openAL/al.h"
#include "FrameWork/CWaves.h"
#include "FrameWork/Framework.h"
#include "Platform/PlatformMisc.h"
#include "ThirdParty/json/json.hpp"
#include "FrameWork/aldlist.h"
using json = nlohmann::json;

namespace Sun {

	struct MediaInfo {
		//std::string name;
		std::string type;
		std::string filename;
	};

	class ALBufferDeleter {
	public:
		template<typename _T>
		void operator()(_T& id) {
			alDeleteBuffers(1, &id);
		}
	};

	class AudioImpl {
	public:
		AudioImpl();

		bool initEnvironment();

		//播放一段3d音频,会根据路径搜索缓存,缓存用LRU方式存储
		//如果缓存中有，直接播放；否则读取硬盘，加入缓存并播放，
		//st和ls是音源和监听者的状态信息 (当前只支持wav格式)
		//必须在主线程调用，内部会自动开启子线程播放音乐，之后返回
		void play3DAsync(const std::string& path, const SourceState& ss = SourceState(), const ListenState& ls = ListenState());

		void wait(unsigned int id); //for test
		int test();

	protected:
		//当前使用的deviceInfo
		ALDEVICEINFO deviceInfo_;
		std::string rootDir_;
		void loadConfig();
		void loadLRU();
		void initListener();


		//子线程只读，主线程可读写，但写入时必须保证
		//std::unordered_map<unsigned ,ALuint> sources_;
		std::vector<unsigned int> sources_;
		std::unordered_map<std::string, MediaInfo> configs_;
		TLRUCache<std::string, unsigned int, ALBufferDeleter> lru_{ AUDIO_CACHE_NUM };
	};
}

#endif