#include "MeshInterface.h"
#include "Render/GraphicsLib.h"

namespace Sun {

	void MeshInterface::setVertexDataHandle(const GpuVertexDataHandle& handle) {
		gpuVertexDataHandle_ = handle;
	}

	GpuVertexDataHandle MeshInterface::getVertexDataHandle() const {
		return gpuVertexDataHandle_;
	}

	void MeshInterface::render() {
		if (!gpuVertexDataHandle_.isValid()) return;
		bindVertexData();
		drawCall();
		unBindVertexData();
	}

	void MeshInterface::tick(float dtime, const SEvent& ev) {

	}

	void MeshInterface::onAddedToLevel(RenderAbleCom* com) {

	}

	void MeshInterface::bindUniformData(const ShaderProgram& pro) {

	}

	void MeshInterface::unBindUniformData(const ShaderProgram& pro) {

	}

	void MeshInterface::updateVertexData() {

	}

	void MeshInterface::bindVertexData() {
		if (gpuVertexDataHandle_.vao == INVALID_HANDLE) {
			assert(false);
			return;
		}
		g_graphicsLib->bindVertexArray(gpuVertexDataHandle_.vao);
	}

	void MeshInterface::unBindVertexData() {
		g_graphicsLib->bindVertexArray(0);
	}

	void MeshInterface::drawCall() {
		g_graphicsLib->drawCall(gpuVertexDataHandle_);
	}

}