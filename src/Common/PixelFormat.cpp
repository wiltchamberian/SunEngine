#include "PixelFormat.h"

namespace Sun {

	int getChannelNumFromPixelFormat(EPixelFormat format) {
		if (format == EPixelFormat::RGB
			|| format == EPixelFormat::RGB16F
			|| format == EPixelFormat::RGB32F) {
			return 3;
		}
		else if (format == EPixelFormat::RGBA
			|| format == EPixelFormat::RGBA32F
			|| format == EPixelFormat::RGBA16F) {
			return 4;
		}
		assert(false);
		return 4;
	}
}