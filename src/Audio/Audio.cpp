// Audio.cpp : 定义 DLL 的导出函数。
//

#include "./pch.h"
#include "framework.h"
#include "Audio.h"
#include "AudioImpl.h"


// 这是导出变量的一个示例
AUDIO_API int nAudio=0;

// 这是导出函数的一个示例。
AUDIO_API int fnAudio(void)
{
    return 0;
}

namespace Sun {

	AUDIO_API Audio* g_audioEngine = nullptr;

	Audio* Audio::createAudioEngine() {
		return new Audio();
	}

    // 这是已导出类的构造函数。
    Audio::Audio()
    {
		impl_ = new AudioImpl();
        return;
    }

	Audio::~Audio()
	{
		delete impl_;
	}

	bool Audio::initEnvironment() {
		return impl_->initEnvironment();
	}

	void Audio::play3DAsync(const std::string& name, const SourceState& state, const ListenState& ls) {
		impl_->play3DAsync(name, state, ls);
	}

	void Audio::wait(unsigned int id) {
		impl_->wait(id);
	}

    int Audio::test() {
		return impl_->test();
    }
}
