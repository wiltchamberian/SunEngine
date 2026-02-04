#include "InstancedMeshCom.h"
#include "Primitive/VertexData.h"
#include "Render/GraphicsLib.h"
#include <cassert>

namespace Sun {

	InstancedMeshCom::~InstancedMeshCom() {
		releaseGpu();
	}

	void InstancedMeshCom::allocGpu(EDrawMode type) {
		if (!gpuVertexDataHandle_.isValid()) {
			VertexDataArray vec;
			VertexData d0;
			d0.vertice = (void*)getVerticesData();
			d0.vertexCount = getVerticesSize();
			d0.vertexSiz = sizeof(Vertex);
			d0.indice = (void*)getIndicesData();
			d0.indiceCount = getIndicesSize();
			d0.indiceSiz = sizeof(int);
			d0.usage = type;
			d0.layout = Vertex::getLayout();

			VertexData d1;
			d1.vertice = instancedVertices_.data();
			d1.vertexCount = instancedVertices_.size();
			d1.vertexSiz = sizeof(decltype(instancedVertices_)::value_type);
			d1.usage = type;
			d1.layout = InstancedVertex::getLayout();

			vec.emplace_back(d0);
			vec.emplace_back(d1);

			g_graphicsLib->applyGpuVertexDataArray(gpuVertexDataHandle_, vec);
			gpuVertexDataHandle_.numInstance = instancedVertices_.size();
			gpuVertexDataHandle_.mode = triangleType_;
		}
		else {
			assert(false);
		}
	}

	void InstancedMeshCom::bindUniformData(const ShaderProgram& pro) {
		if (!pro.isValid()) {
			assert(false);
			return;
		}
		MeshComponent::bindUniformData(pro);
		g_graphicsLib->setInt(pro.getId(), U_INSTANCE, true);
	}

	void InstancedMeshCom::unBindUniformData(const ShaderProgram& pro) {
		MeshComponent::unBindUniformData(pro);
		g_graphicsLib->setBool(pro.getId(), U_INSTANCE, false);
	}

	void InstancedMeshCom::allocInstanceGpu(EDrawMode type) {
		VboEboCreateInfo ci;
		ci.vertices = instancedVertices_.data();
		ci.verticeCount = instancedVertices_.size();
		ci.verticeSiz = sizeof(decltype(instancedVertices_)::value_type);
		ci.drawMode = type;

		transformsHandle_ = g_graphicsLib->applyForVboEbo(ci);
		g_graphicsLib->bindVboEboToVao(transformsHandle_, gpuVertexDataHandle_.vao, InstancedVertex::getLayout());

		gpuVertexDataHandle_.numInstance = instancedVertices_.size();
	}

	void InstancedMeshCom::releaseGpu() {
		if(gpuVertexDataHandle_.vao!= INVALID_HANDLE)
			g_graphicsLib->deleteVertexArray(gpuVertexDataHandle_.vao);
		if(transformsHandle_.vbo!= INVALID_HANDLE)
			g_graphicsLib->deleteBufferData(transformsHandle_.vbo);
		if(transformsHandle_.ebo !=INVALID_HANDLE)
			g_graphicsLib->deleteBufferData(transformsHandle_.ebo);
		gpuVertexDataHandle_.vao = INVALID_HANDLE;
		transformsHandle_.vbo = INVALID_HANDLE;
		transformsHandle_.ebo = INVALID_HANDLE;
	}

	//////////////////////////////////////////SimpleInstancedMeshCom/////////////////////////////////////////////////////
	SimpleInstancedMeshCom::~SimpleInstancedMeshCom() {
		releaseGpu();
	}

	//for test
	void SimpleInstancedMeshCom::allocGpu(EDrawMode type) {
		VertexDataArray vec;

		vec.indiceCount = getIndicesSize();
		vec.vertexCount = getVerticesSize();

		VertexData d0;
		d0.vertice = (void*)getVerticesData();
		d0.vertexCount = getVerticesSize();
		d0.vertexSiz = sizeof(Vertex);
		d0.indice = (void*)getIndicesData();
		d0.indiceCount = getIndicesSize();
		d0.indiceSiz = sizeof(int);
		d0.usage = type;
		d0.layout = Vertex::getLayout();

		VertexData d1;
		d1.vertice = instancedVertices_.data();
		d1.vertexCount = instancedVertices_.size();
		d1.vertexSiz = sizeof(decltype(instancedVertices_)::value_type);
		d1.usage = type;
		d1.layout = SimpleInstancedVertex::getLayout();

		vec.emplace_back(d0);
		vec.emplace_back(d1);

		g_graphicsLib->applyGpuVertexDataArray(gpuVertexDataHandle_, vec);
		gpuVertexDataHandle_.numInstance = instancedVertices_.size();
	}

	void SimpleInstancedMeshCom::releaseGpu() {
		//TODO makesure this will release the vertexBuffer!
		g_graphicsLib->deleteVertexArray(gpuVertexDataHandle_.vao);
		gpuVertexDataHandle_.vao = INVALID_HANDLE;
	}
}