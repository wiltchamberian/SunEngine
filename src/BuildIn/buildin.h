#ifndef __BUILD_IN_H
#define __BUILD_IN_H

#include "Common/Define.h"
#include "Component/PbrMaterialCom.h"
#include "Component/SimpleMaterialCom.h"
#include "Primitive/StaticMeshPrimitive.h"
#include "Component/MeshInterface.h"
#include "Component/MeshComponent.h"
#include "Component/SimpleTransformCom.h"
#include "Component/LightTransformCom.h"
#include "Util/GeneralMemoryPool.h"
#include "Math/Box.h"

#ifndef OUT
#define OUT
#endif

namespace Sun {

	enum class EGameObjectFlag : unsigned int {
		DEFAULT_MESH = 1 << 0,
		ALLOC_MESH = 1 << 1,
		DEFAULT_MATERIAL = 1 << 2,
		ALLOC_MATERIAL = 1 << 3,
		DEFAULT_TRANSFORM = 1<<4,
	};
	inline EGameObjectFlag operator | (const EGameObjectFlag& b1, const EGameObjectFlag& b2) {
		return EGameObjectFlag((unsigned int)b1 | (unsigned int)b2);
	}
	using EGOF = EGameObjectFlag;

	struct TriangleCreateInfo {
		vec3 v0;
		vec3 v1;
		vec3 v2;
		vec4 color0;
		vec4 color1;
		vec4 color2;
		vec2 tex0;
		vec2 tex1;
		vec2 tex2;
	};

	class SWorld;
	class GameObject;
	class Missile;
	struct GameObjectCreateInfo;
	struct MissileCreateInfo;

	/*******************************变换创建函数***************************/
	SharedPtr<SimpleTransformCom> buildSimpleTransform(const SimpleTransformCreateInfo& createInfo = SimpleTransformCreateInfo());
	SharedPtr<LightTransformCom> buildLightTransform(const LightTransformCreateInfo& createInfo = LightTransformCreateInfo());

	/********************************材质创建函数*******************************************/
	SharedPtr<SimpleMaterialCom> buildSimpleMaterial(const SimpleMaterialCreateInfo& createInfo);
	SharedPtr<PbrMaterialCom> buildPbrMaterial(const PbrMaterialCreateInfo& createInfo);

	SharedPtr<PbrMaterialCom> buildDefaultPbrMaterial(int id = 0);

	
	/********************************顶点信息创建函数*********************************************/
	//以AABB为盒子构建立方体
	void buildCube(std::vector<Vertex>& vertices, std::vector<int>& indices, const AABB& box, const vec4& color = g_defaultColor);
	//构建边长为1m,中心为(0,0,0)的立方体
	void buildDefaultCube(std::vector<Vertex>& vertices, std::vector<int>& indices ,const vec4& color = g_defaultColor);
	//构建三角形面片(自动生成法线)
	void buildTriangle(std::vector<Vertex>& IN_OUT vertices, std::vector<int>& IN_OUT indices, const TriangleCreateInfo& createInfo);

	//构建起始点在原点的三角形面片
	void buildTriangleAutoNormal(std::vector<Vertex>& IN_OUT vertices, std::vector<int>& IN_OUT indices, const Vertex& v1, const Vertex& v2);

	//构建三角形面片，自定义全部信息
	void buildTriangle(std::vector<Vertex>& IN_OUT vertices, std::vector<int>& IN_OUT indices, const Vertex& v0, const Vertex& v1, const Vertex& v2);
	//构建平行四边形面片(自动生成法线),以v0v1,v0v2为矢量，构建平行四边形
	void buildParallelogramAutoNormal(std::vector<Vertex>& vertices, std::vector<int>& indices, const Vertex& v0, const Vertex& v1, const Vertex& v2);
	
	/*********************************网格创建函数*******************************************/
	SharedPtr<MeshComponent> loadDefaultMeshComponent();
	SharedPtr<MeshComponent> buildTriangleMeshComponent(const Vertex& v0, const Vertex& v1, const Vertex& v2);

	SharedPtr<MeshComponent> buildParallelogramMeshComponent(const Vertex& v0, const Vertex& v1, const Vertex& v2);

	//长方体网格  (len,z axis , width: x axis, h y axis)
	SharedPtr<MeshComponent> buildCuboid(float len, float width, float h);

	//球体网格
	SharedPtr<MeshComponent> buildSphereMesh();
	/*********************************渲染组件创建函数********************************/
	GameObject* createGameObject(const SharedPtr<MeshInterface>& mesh, const SharedPtr<MaterialCom>& material);

	/**********************************GameObject创建函数************************************/
	GameObject* createGameObject(const GameObjectCreateInfo& createInfo);

	GameObject* createGround(const std::string& path,int defaultMaterialNo = -1);

	//创建一个缺省网格，缺省材质，缺省变换的gameobject
	GameObject* buildDefaultGameObject(EGameObjectFlag flags);

	//创建子弹
	Missile* createMissile(const MissileCreateInfo& createInfo);

	//创建灯光
	extern GameObject* buildPointLight(const vec3& color, const vec3& translate);

	/***********************************world创建函数**********************************/
	SWorld* loadDefaultWorld();

	//just for test,no use
	void test(MissileCreateInfo createInfo);
}



#endif