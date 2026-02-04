#ifndef __SUN_AUDIO_H
#define __SUN_AUDIO_H

#ifdef AUDIO_EXPORTS
#define AUDIO_API __declspec(dllexport)
#else
#define AUDIO_API __declspec(dllimport)
#endif

#include "Math/Vector3D.h"
#include <string>


namespace Sun {

#define AUDIO_CACHE_NUM 1024

#define AUDIO_RELATIVE_DIR "Pak\\audio\\"

#define AUDIO_MGR_JSON "audioMgr.json"

#define AUDIO_FORMAT_WAV "wav"

#define	TEST_WAVE_FILE		"Footsteps.wav"
#define TEST_WAVE_FILE2     "fiveptone.wav"

	//OpenAL格式
	struct SourceState {
		vec3 position;
		vec3 velocity;
		vec3 direction;
	};

	struct Orientation {
		vec3 forward;
		vec3 up;
		const float* data() const {
			return forward.data();
		}
	};

	struct ListenState {
		vec3 position;
		vec3 velocity;
		Orientation ori;
	};

	class AudioImpl;

	class AUDIO_API Audio {
	public:
		Audio();
		~Audio();

		static Audio* createAudioEngine();

		bool initEnvironment();

		//播放一段3d音频,会根据路径搜索缓存,缓存用LRU方式存储
		//如果缓存中有，直接播放；否则读取硬盘，加入缓存并播放，
		//st和ls是音源和监听者的状态信息 (当前只支持wav格式)
		//必须在主线程调用，内部会自动开启子线程播放音乐，之后返回
		void play3DAsync(const std::string& path, const SourceState& ss = SourceState(),const ListenState& ls= ListenState());

		void wait(unsigned int id); //for test
		int test();

	protected:
		AudioImpl* impl_;
	};

	extern AUDIO_API Audio* g_audioEngine;
}


extern AUDIO_API int nAudio;

AUDIO_API int fnAudio(void);

#endif
