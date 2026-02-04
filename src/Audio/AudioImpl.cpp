#include "AudioImpl.h"
#include <fstream>

namespace Sun {



	// 这是已导出类的构造函数。
	AudioImpl::AudioImpl()
	{
		return;
	}

	bool AudioImpl::initEnvironment() {
		//加载配置文件
		loadConfig();

		//创建全局CWaves
		ALFWInit();
		//列出设备列表，让用户选择一个，之后创建设备和对应context,再清空设备列表
		if (!ALFWInitOpenAL(&deviceInfo_))
		{
			ALFWprintf("Failed to initialize OpenAL\n");
			ALFWShutdown();
			return false;
		}

		//根据配置文件加载一部分音频数据到lru缓存
		loadLRU();

		//初始化listener
		initListener();
	}

	void AudioImpl::play3DAsync(const std::string& name, const SourceState& state, const ListenState& ls) {
		bool ok = true;
		ALuint hBuffer;
		auto it = lru_.find(name);
		if (it != lru_.end()) {
			hBuffer = lru_.get(name);
		}
		else {
			if (configs_.find(name) != configs_.end()) {
				MediaInfo& info = configs_.find(name)->second;
				if (info.type == AUDIO_FORMAT_WAV) {
					alGenBuffers(1, &hBuffer);
					std::string path = rootDir_ + AUDIO_RELATIVE_DIR + info.filename;
					bool ok = ALFWLoadWaveToBuffer(path.c_str(), hBuffer);
					if (ok) {
						lru_.put(name, hBuffer);
					}
					else {
						alDeleteBuffers(1, &hBuffer);
						return;
					}
				}
				else {
					return;
				}
			}
		}

		ALuint hSource;
		ALint iState;
		bool bingo = false;
		for (auto& it : sources_) {
			alGetSourcei(it, AL_SOURCE_STATE, &iState);
			if (iState != AL_PLAYING) {
				hSource = it;
				bingo = true;
				break;
			}
		}
		if (bingo == false) {
			alGetError();
			alGenSources(1, &hSource);
			if (alGetError() != AL_NO_ERROR) {
				//走到这里，说明没有可用的source;直接返回
				return;
			}
			else {
				sources_.emplace_back(hSource);
			}
		}

		if (ok) {
			alSourcei(hSource, AL_BUFFER, hBuffer);

			alGetError();

			alSource3f(hSource, AL_POSITION, state.position.x, state.position.y, state.position.z);
			alSource3f(hSource, AL_VELOCITY, state.velocity.x, state.velocity.y, state.velocity.z);
			alSource3f(hSource, AL_DIRECTION, state.direction.x, state.direction.y, state.direction.z);

			auto er = alGetError();
			if (er != AL_NO_ERROR) {
				//printf("alListenerfv POSITION : ", er);
				return;
			}

			alListener3f(AL_POSITION, ls.position.x, ls.position.y, ls.position.z);
			alListener3f(AL_VELOCITY, ls.velocity.x, ls.velocity.y, ls.velocity.z);
			alListenerfv(AL_ORIENTATION, ls.ori.data());

			er = alGetError();
			if (er != AL_NO_ERROR) {
				//printf("alListenerfv POSITION : ", er);
				return;
			}

			alSourcePlay(hSource);
		}

	}

	void AudioImpl::wait(unsigned int id) {
		ALuint uiSource = sources_[id];
		ALint iState;
		do
		{
			Sleep(100);
			ALFWprintf(".");
			// Get Source State
			alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
		} while (iState == AL_PLAYING);

	}

	int AudioImpl::test() {
		ALuint      uiBuffer;
		ALuint      uiSource;
		ALuint      uiSource2;
		ALint       iState;

		//创建全局CWaves
		ALFWInit();

		ALFWprintf("PlayStatic Test Application\n");

		//列出设备列表，让用户选择一个，之后创建设备和对应context,再清空设备列表
		if (!ALFWInitOpenAL(nullptr))
		{
			ALFWprintf("Failed to initialize OpenAL\n");
			ALFWShutdown();
			return 0;
		}

		// Generate an AL Buffer
		ALuint uiBuffer2;
		alGenBuffers(1, &uiBuffer);
		alGenBuffers(1, &uiBuffer2);

		// Load Wave file into OpenAL Buffer
		//把路径指向的wave数据加载到指定的alBuffer里
		if (!ALFWLoadWaveToBuffer((char*)ALFWaddMediaPath(TEST_WAVE_FILE), uiBuffer))
		{
			ALFWprintf("Failed to load %s\n", ALFWaddMediaPath(TEST_WAVE_FILE));
		}
		if (!ALFWLoadWaveToBuffer((char*)ALFWaddMediaPath(TEST_WAVE_FILE2), uiBuffer2))
		{
			ALFWprintf("Failed to load %s\n", ALFWaddMediaPath(TEST_WAVE_FILE2));
		}

		// Generate a Source to playback the Buffer
		alGenSources(1, &uiSource);
		alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
		assert(iState == AL_INITIAL);
		// Generate a Source to playback the Buffer
		alGenSources(1, &uiSource2);

		// Attach Source to Buffer
		alSourcei(uiSource, AL_BUFFER, uiBuffer);
		alSourcei(uiSource2, AL_BUFFER, uiBuffer2);

		alGetError();
		alSource3f(uiSource, AL_POSITION, 0.0, 0.0, 0.0);
		alSourcei(uiSource, AL_LOOPING, AL_FALSE);
		vec3 pos;
		alGetSource3f(uiSource, AL_POSITION, &pos[0], &pos[1], &pos[2]);
		vec3 vel;
		alGetSourcefv(uiSource, AL_VELOCITY, &vel[0]);
		vec3 dir;
		alGetSource3f(uiSource, AL_DIRECTION, &dir[0], &dir[1], &dir[2]);

		alListener3f(AL_POSITION, 5, 0, 0);
		vec3 lpos;
		alGetListener3f(AL_POSITION, &lpos[0], &lpos[1], &lpos[2]);
		vec3 lvel;
		alGetListener3f(AL_VELOCITY, &lvel[0], &lvel[1], &lvel[2]);
		Orientation lori;
		alGetListenerfv(AL_ORIENTATION, (ALfloat*)(&lori));

		auto er = alGetError();

		// Play Source
		alSourcei(uiSource, AL_BUFFER, uiBuffer);
		alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
		assert(iState == AL_INITIAL);
		alSourcePlay(uiSource);
		alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
		assert(iState == AL_PLAYING);

		ALFWprintf("Playing Source ");

		float x = 5.0;
		do
		{
			Sleep(100);
			x -= 1.0;
			if (x == -6.0) {
				x = 5.0;
			}
			alListener3f(AL_POSITION, x, 0, 0);
			ALFWprintf(".");
			// Get Source State
			alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
		} while (iState == AL_PLAYING);
		assert(iState == AL_STOPPED);

		alSourcei(uiSource, AL_BUFFER, uiBuffer2);
		if (alGetError() != AL_NO_ERROR) {
			int x = 0; int y = x;
		}
		alSourcePlay(uiSource);
		do
		{
			Sleep(100);
			x -= 1.0;
			if (x == -6.0) {
				x = 5.0;
			}
			alListener3f(AL_POSITION, x, 0, 0);
			ALFWprintf(".");
			// Get Source State
			alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
		} while (iState == AL_PLAYING);


		ALFWprintf("\n");

		// Clean up by deleting Source(s) and Buffer(s)
		alSourceStop(uiSource);
		alDeleteSources(1, &uiSource);
		alDeleteBuffers(1, &uiBuffer);

		ALFWShutdownOpenAL();

		ALFWShutdown();

		return 0;


	}

	void AudioImpl::loadConfig() {
		rootDir_ = getGlobalPath();
		std::string audioMgrPath = rootDir_;
		audioMgrPath += AUDIO_RELATIVE_DIR;
		audioMgrPath += AUDIO_MGR_JSON;

		json js;
		std::ifstream fstream(audioMgrPath);
		if (!fstream.is_open()) {
			assert(false);
			return;
		}
		try {
			fstream >> js;
		}
		catch (...) {
			assert(false);
			return;
		}


		if (js.count("media") == 0) {
			assert(false);
			return;
		}
		json& media = js["media"];
		assert(media.is_array());
		size_t numMedia = media.size();

		if (js.count("RelativeDir") == 0) {
			assert(false);
			return;
		}

		std::string filename;
		std::string name;
		std::string type;
		for (auto& audio : media) {
			filename = audio["filename"].get<std::string>();
			name = audio["name"].get<std::string>();
			type = audio["type"].get<std::string>();

			if (type == AUDIO_FORMAT_WAV) {
				configs_.insert(std::make_pair(name, MediaInfo{ type,filename }));
			}
		}
		return;
	}

	void AudioImpl::loadLRU() {
		int num = std::min<int>(lru_.maxSize(), configs_.size());
		int i = 0;
		std::string prefix = rootDir_ + AUDIO_RELATIVE_DIR;
		for (auto& it : configs_) {
			if (i >= num) break;
			if (it.second.type == AUDIO_FORMAT_WAV) {
				ALuint hBuffer;
				alGetError();
				alGenBuffers(1, &hBuffer);
				if (alGetError() != AL_NO_ERROR) {
					break;
				}
				if (!ALFWLoadWaveToBuffer((prefix + it.second.filename).c_str(), hBuffer))
				{
					ALFWprintf("Failed to load %s\n", (prefix + it.second.filename).c_str());
					alDeleteBuffers(1, &hBuffer);
					continue;
				}
				lru_.put(it.first, hBuffer);

				i += 1;
			}
		}
	}

	void AudioImpl::initListener() {


	}
}