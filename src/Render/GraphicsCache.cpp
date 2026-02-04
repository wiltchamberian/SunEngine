#include "GraphicsCache.h"
#include "Primitive/ImageCache.h"
#include "GraphicsLib.h"
#include "Common/Define.h"

namespace Sun {

	bool operator == (const GPUTexture& t1, const GPUTexture& t2) {
		return t1.source == t2.source;
	}

	GraphicsCache* g_graphicsCache = new GraphicsCache();

	unsigned int GraphicsCache::applyForTexture() {
		if (!unUseHandles_.empty()) {
			unsigned int handle = unUseHandles_.back();
			unUseHandles_.pop_back();
			texHandles_.insert(handle);
			return handle;
		}
		unsigned int handle  = g_graphicsLib->genTexture();
		texHandles_.insert(handle);
		return handle;
	}

	void GraphicsCache::unUseTexture(unsigned int handle) {
		texHandles_.erase(handle);
		unUseHandles_.push_back(handle);
	}

	void GraphicsCache::applyForTexture(const std::string& source, Texture& texture) {
		if (source.empty())
			return;
		SharedPtr<Image> buf = g_imageCache_->loadImage(source.c_str());
		texture.source_ = source;
		texture.width_ = buf->width_;
		texture.height_ = buf->height_;
		texture.textureType_ = buf->type_;
		texture.pixelFormat_ = buf->pixelFormat_;

		//
		auto iter = gpuTextures_.find(source);
		if (iter != gpuTextures_.end()) {
			g_graphicsLib->bindTexture(iter->second.textureType_, iter->second.handle_);

			//return iter->second.handle_;
			texture = iter->second;
		}
		else {
			iter = unuseTextures_.find(source);
			if (iter != unuseTextures_.end()) {
				g_graphicsLib->bindTexture(iter->second.textureType_, iter->second.handle_);

				//return iter->second.handle_;
				texture = iter->second;
			}
			else {
				unsigned int handle = -1;
				handle = g_graphicsLib->genTexture();

				TexImage2D input;
				input.handle = handle;
				input.data = buf->data_.data();
				input.width = buf->width_;
				input.height = buf->height_;
				input.pixelDataType = EBasicDataType::UNSIGNED_BYTE;
				input.inputPixelFormat = buf->pixelFormat_;
				input.internalPixelFormat = EPixelFormat::RGBA;
				input.wrapperMode = ETextureWrapper::REPEAT;
				input.filter = ETextureMinMagFilter::LINEAR;
				g_graphicsLib->texImage2D(input);

				texture.handle_ = handle;
				gpuTextures_.insert(std::make_pair(source, texture));
				return;
			}
		}
		return;
	}

	void GraphicsCache::applyForReleaseTexture(const std::string& source) {
		auto iter = gpuTextures_.find(source);
		if (iter != gpuTextures_.end()) {
			unuseTextures_.insert(*iter);
			gpuTextures_.erase(iter);
		}
		else {
			return;
		}
	}

}