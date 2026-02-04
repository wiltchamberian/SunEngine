#include "StaticMeshRasterDefferedLightingSystem.h"
#include "ThirdParty/glfw/glad/glad.h"
#include "Render/ShaderManager.h"
#include "Render/GraphicsProxy.h"
#include "Render/GraphicsLib.h"
#include "Entity/World.h"

namespace Sun {

	StaticMeshRasterDefferedLightingRenderSystem::StaticMeshRasterDefferedLightingRenderSystem(SWorld* world) {
		world_ = world;
	}

	//will be called on game start
	void StaticMeshRasterDefferedLightingRenderSystem::prepareRender() {

	}

	//will be called every frame before all render
	void StaticMeshRasterDefferedLightingRenderSystem::beginRender() {
		GRAPHICS_API_TYPE api = globalGetGraphics_API_Type();
		if (api == GRAPHICS_API_TYPE::G_OPENGL) {
			openglBeginRender();
		}
	}

	//will be called every frame between beginRender and endRender
	void StaticMeshRasterDefferedLightingRenderSystem::render() {
		GRAPHICS_API_TYPE api = globalGetGraphics_API_Type();
		if (api == GRAPHICS_API_TYPE::G_OPENGL) {
			openglrender();
		}
	}

	//will be called every frame after render
	void StaticMeshRasterDefferedLightingRenderSystem::endRender() {

	}

	void StaticMeshRasterDefferedLightingRenderSystem::openglBeginRender() {
		if (quadVAO_ == 0)
		{
			GLfloat quadVertices[] = {
				// Positions        // Texture Coords
				-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
				1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			glUseProgram(g_shaderManager->getShaderProgram(SP_STATIC_MESH_RENDER_DEFFERED_SHADER).getId());

			// Setup plane VAO
			glGenVertexArrays(1, &quadVAO_);
			glGenBuffers(1, &quadVBO_);
			glBindVertexArray(quadVAO_);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO_);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		}

		std::shared_ptr<SCameraComponent> camera = world_->getCameraCom();
		viewPos_ = camera->getWorldPosition();
		
	}

	void StaticMeshRasterDefferedLightingRenderSystem::openglrender() {
		glUseProgram(g_shaderManager->getShaderProgram(SP_STATIC_MESH_RENDER_DEFFERED_SHADER).getId());

		g_graphicsLib->setVec3(g_shaderManager->getShaderProgram(SP_STATIC_MESH_RENDER_DEFFERED_SHADER).getId(), U_VIEWPOS, viewPos_);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_graphicsProxy->gbuf_.gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, g_graphicsProxy->gbuf_.gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, g_graphicsProxy->gbuf_.gAlbedo);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, g_graphicsProxy->gbuf_.gMRA);

		glBindVertexArray(quadVAO_);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void StaticMeshRasterDefferedLightingRenderSystem::vulkanBeginRender() {

	}
}