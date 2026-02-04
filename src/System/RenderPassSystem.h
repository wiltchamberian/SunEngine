/*****************************************************************************
* @brief :RasterRenderPassSystem
* 试图给出渲染管线在cpu层面操作的完整定义
* 代码解说严格区分cpu对象和gpu对象以避免歧义
* 这里一次renderPass指的就是从一帧开始到一帧结束的整个流程
* 这个过程可能会涉及到多个subPass
* 一个subPass通常对应一个对象的完整渲染，这个过程一般至少关联一个着色器程序
* 在延迟着色或者等情形下，可能会级联处理，这里对应关系有点复杂；
* 区分如下:
* 1,一个对象，走完一个shaderPass，完成一次渲染
* 2，一个对象，走完若干级联shaderPass,完成一次渲染（比如光追)
* 3,若干对象，分别走完一个shaderPass;之后这些对象再分别走第二个shaderPass(延迟着色)
* 
* @author : acedtang
* @date : 2021/6/26
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __RENDER_PASS_SYSTEM_H
#define __RENDER_PASS_SYSTEM_H

#include "Common/Common.h"
#include "Entity/GameObject.h"
#include "System/SystemBase.h"
#include <memory>
#include <set>
#include <vector>

namespace Sun {

	struct RenderScene {
		//表示所有对象都使用统一的全局着色器渲染，常用于光照贴图
		bool useGlobalShader = false;
		ShaderProgram globalShader;
	};

	class SWorld;
	//using GameObjectContainer=std::vector<GameObject*>;
	class InstancedMeshCom;
	class RenderPassSystem : public SystemBase {
	public:
		RenderPassSystem(SWorld* world);

        void prepare() override;
        void tick(float dtime, const SEvent& ev) override;

		//简单测试场景
		void renderSceneTest();
		
		void addGameObject(GameObject* obj);
		void eraseGameObject(GameObject* obj);
	protected:
        //渲染系统的初始化配置，在系统启之后调用1次
        void renderPassPrepare();

        //一次完整的renderPass开始前，需要计算的一些公共cpu全局变量
        void renderPassBegin();

        //[main]按照涉及到的着色器进行遍历,this is the Main function!
        void renderPass();

		//帧缓冲渲染相关
		void createFrameBuffer();
		void bindFrameBuffer();
		void flushFrameBuffer();
		bool useFrameBuffer_ = true;
		//法线可视化
		bool normalVisual_ = false;
		unsigned int hFrameBuf_ = 0;
		unsigned int hColorBuf_ = 0;
		unsigned int hDepthBuf_ = 0;
		GpuVertexDataHandle frameBufVertexDataHandle_;

		//阴影贴图相关
		void createDepthMap();//创建深度（阴影)贴图	
		void renderDepthMap(bool toScreen = false);//从平行光源视角渲染深度贴图（目前假设场景中只有最多一个平行光源
		void flushDepthMap();
		Matrix4x4 getDLightVPMat();
		unsigned int shadowWidth_ = 800;
		unsigned int shadowHeight_ = 600;
		unsigned int hShadowFBO_ = -1;
		unsigned int hShadowDepthTex_ = -1;
		GpuVertexDataHandle shadowVertexDataHandle_;

		//法线贴图
		void createNormalMap();
		void renderNormalMap(bool toScreen = false);
		void flushNormalMap();
		unsigned int normalWidth_ = 800;
		unsigned int normalHeight_ = 600;
		unsigned int hNormalFBO_ = -1;
		unsigned int hNormalTex_ = -1;
		unsigned int hNormalDepthTex_ = -1;//no use ,just interface need 
		GpuVertexDataHandle hNormalDataHandle_; //quad 句柄

		//背景(天空盒或者其他)
		void createBackGroundImage();
		void flushBackGround();
		GpuVertexDataHandle hBackgroundHandle_;

		//场景对象剔除
		void sceneCulling();

		//场景对象排序
		void sceneSort();

		//场景实例化合批
		void sceneInstanceBatching();

		//场景常规合批
		void sceneNormalBatching();

		//场景渲染
		void sceneRendering(const RenderScene& param);

		//善后处理
		void sceneEndProcess();

		//上传视口的 视图，投影，vp矩阵
		void uploadViewTransform(const ShaderProgram& pro);

		void setViewPort();

		//实例化合批
		static GameObject* instanceBatching(std::vector<GameObject*>::iterator first, std::vector<GameObject*>::iterator end);
		static std::shared_ptr<InstancedMeshCom> meshBatching(std::vector<GameObject*>::iterator first, std::vector<GameObject*>::iterator end);
		SWorld* world_ = nullptr;
		SLevel* level_ = nullptr;
		std::set<GameObject*, GameObjectCompare> needRenderObjs_;

		/*********************临时变量************************/
		//剔除后对象
		std::vector<GameObject*> objsAfterCulled_;
		std::vector<GameObject*> createdObjs_; //合批过程中新创建的gameobjects
		std::vector<GameObject*> renderObjs_; //包含所有需要渲染的object,含createdObjs;
	};

}



#endif