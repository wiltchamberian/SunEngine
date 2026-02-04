#include "SimpleMaterialCom.h"
#include "Render/GraphicsLib.h"
#include "Render/GraphicsCache.h"

namespace Sun {

	SimpleMaterialCom::SimpleMaterialCom() {
		SimpleMaterialCreateInfo createInfo;
		init(createInfo);
	}

	SimpleMaterialCom::SimpleMaterialCom(const SimpleMaterialCreateInfo& createInfo) {
		init(createInfo);
	}

	void SimpleMaterialCom::init(const SimpleMaterialCreateInfo& createInfo) {
		texture.source_ = createInfo.texture_path;
	}

	void SimpleMaterialCom::bind(const ShaderProgram& pro) {
		g_graphicsLib->bindTexture(pro.getId(), texture.handle_, ETextureType::TEXTURE_2D,
			ETextureUnit::TEXTURE0, 0, "texSampler");
	}

	void SimpleMaterialCom::unBind(const ShaderProgram& pro) {
		g_graphicsLib->unBindTexture(pro.getId(), texture.handle_, ETextureType::TEXTURE_2D,
			ETextureUnit::TEXTURE0, 0, "texSampler");
	}

	void SimpleMaterialCom::allocGpu() {
		g_graphicsCache->applyForTexture(texture.source_, texture);
	}

	void SimpleMaterialCom::releaseGpu() {
		g_graphicsCache->applyForReleaseTexture(texture.source_);
	}

}