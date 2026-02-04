#include "RenderPassSystem.h"
#include <map>
#include <algorithm>
#include <iostream>
#include "Entity/World.h"
#include "Component/InstancedMeshCom.h"
#include "Render/GraphicsLib.h"
#include "Common/Common.h"
#include "Common/CVarCenter.h"
#include "Entity/GameObject.h"
#include "Util/Clock.h"
#include "Platform/PlatformIO.h"
#include "Common/Define.h"
#include "Render/ShaderManager.h"
#include "Render/RenderAPI.h"
#include "BuildIn/buildin.h"
#include "Math/Vector4D.h"

namespace Sun {

    RenderPassSystem::RenderPassSystem(SWorld* world) {
        world_ = world;
    }

    void RenderPassSystem::prepare() {
        renderPassPrepare();
    }

    void RenderPassSystem::tick(float dtime, const SEvent& ev) {
        renderPass();
    }

    void RenderPassSystem::renderPassPrepare() {

        level_ = world_->getCurLevel();
        //g_graphicsLib->setPolygonMode(ECullMode::FRONT_AND_BACK, EFillMode::LINE);
        if (useFrameBuffer_) {
            createFrameBuffer();

            
        }
        createDepthMap();
        createNormalMap();
    }

    //一次完整的renderPass开始前，需要计算的一些公共cpu全局变量
    void RenderPassSystem::renderPassBegin() {
        Matrix4x4 view = world_->getMainPlayerCameraViewMatrix();
        Matrix4x4 proj = world_->getMainPlayerCameraProjMatrix();
    }


    //简单测试场景
    void RenderPassSystem::renderSceneTest() {
        std::vector<Vertex> vertices;
        std::vector<int> indices;
        vec4 color(1.0, 0.0, 0.0, 1.0);
        buildDefaultCube(vertices, indices, color);

        auto pro =g_shaderManager->getShaderProgram(SP_SIMPLE_PASS); 
        g_graphicsLib->useProgram(pro);
        g_graphicsLib->setFloat(pro.getId(), U_TIME, Sys_Milliseconds() * 0.001);
        Matrix4x4 mat = Matrix4x4::getScale(vec3(1, 1, 1));
        g_graphicsLib->setMat4(pro.getId(), U_MODEL, mat.data(), 1);
        auto vp = world_->getMainPlayerCameraVPMatrix();
        g_graphicsLib->setMat4(pro.getId(), U_VP, vp.data(), 1);
        g_graphicsLib->disable(ECapability::CULL_FACE);

        g_graphicsLib->enable(ECapability::PROGRAM_POINT_SIZE);

        static GpuVertexDataHandle handle;
        handle.mode = EPrimitiveType::TRIANGLES;
        if (!handle.isValid()) {
            g_graphicsLib->applyGpuVertexData(handle, Vertex::getLayout(), vertices.data(), vertices.size(),
                sizeof(Vertex),
                indices.data(), indices.size(), sizeof(int), EDrawMode::STATIC_DRAW);
            
        }
        g_graphicsLib->bindVertexArray(handle.vao);
        g_graphicsLib->drawCall(handle);
        g_graphicsLib->bindVertexArray(0);
    }

    //实例化合批
    GameObject* RenderPassSystem::instanceBatching(std::vector<GameObject*>::iterator first, std::vector<GameObject*>::iterator end) {
        std::shared_ptr<InstancedMeshCom> mesh = meshBatching(first, end);
        if (!mesh) return nullptr;

        GameObject* obj = ActorFactory<GameObject>::get()->malloc();
        obj->setComponent<MeshInterface>(mesh);
        obj->setComponent<MaterialCom>((*first)->getComponent<MaterialCom>());
        obj->setShaderProgram((*first)->getShaderProgram());

        obj->setInstanceDrawing(true);

        return obj;
    }

    std::shared_ptr<InstancedMeshCom> RenderPassSystem::meshBatching(std::vector<GameObject*>::iterator first, std::vector<GameObject*>::iterator end) {
#ifdef _DEBUG
        Clock clock;
#endif

        std::shared_ptr<MeshComponent> mesh = (*first)->getComponent<MeshComponent>();
        if (!mesh) {
            return nullptr;
        }
        std::shared_ptr<InstancedMeshCom> imesh(new InstancedMeshCom());
        //imesh->vertices_ = mesh->vertices_;
        //imesh->indices_ = mesh->indices_;

        GpuVertexDataHandle hd;
        hd.vao = g_graphicsLib->genVertexArray();
       
        GpuVertexDataHandle old = mesh->getVertexDataHandle();
        //FIXME 
        g_graphicsLib->bindVboEboToVao({ old.vec[0].vbo,old.vec[0].ebo }, hd.vao, Vertex::getLayout());
        hd.indexCount = old.indexCount;
        hd.vertexCount = old.indexCount;
        hd.mode = old.mode;
        
        imesh->setVertexDataHandle(hd);
        InstancedVertex tmp;
        imesh->instancedVertices_.reserve(std::distance(first, end));
        for (auto iter = first; iter != end;++iter) {
            tmp.model = (*iter)->getWorldMatrix().getTranspose();
            //tmp.tf = (*iter)->getWorldTransform();
            imesh->instancedVertices_.push_back(tmp);
        }
        imesh->allocInstanceGpu(EDrawMode::STATIC_DRAW);

#ifdef _DEBUG
        printf("meshBatching:%d\n", (int)clock.getMicroDuration());
#endif
  
        return imesh;
    }

    void RenderPassSystem::addGameObject(GameObject* obj) {
        needRenderObjs_.insert(obj);
    }

    void RenderPassSystem::eraseGameObject(GameObject* obj) {
        needRenderObjs_.erase(obj);
    }

    void RenderPassSystem::createFrameBuffer() {
        //创建frameBuffer
        unsigned int framebuf = 0;
        g_graphicsLib->genFrameBuffers(1,&framebuf);
        g_graphicsLib->bindFrameBuffer(framebuf);

        //创建颜色附件buffer
        hColorBuf_ = g_graphicsLib->genTexture();
        Extend extend = g_platformIO->getFrameBufferSize();

        TexImage2D input;
        input.handle = hColorBuf_;
        input.data = nullptr;
        input.width = extend.width;
        input.height = extend.height;
        input.pixelDataType = EBasicDataType::UNSIGNED_BYTE;
        input.inputPixelFormat = EPixelFormat::RGB;
        input.internalPixelFormat = EPixelFormat::RGB;
        input.wrapperMode = ETextureWrapper::REPEAT;
        input.filter = ETextureMinMagFilter::LINEAR;

        g_graphicsLib->texImage2D(input);

        FrameBufferTexture2D fb;
        fb.target = EFrameBufferType::FRAME_BUFFER;
        fb.attachment = EAttachmentType::COLOR_ATTACHMENT0;
        fb.textarget = ETextureType::TEXTURE_2D;
        fb.hTex = hColorBuf_;
        fb.level = 0;
        g_graphicsLib->framebufferTexture2D(fb);

        //创建深度附件buffer
        g_graphicsLib->genRenderbuffers(1, &hDepthBuf_);
        g_graphicsLib->bindRenderbuffer(hDepthBuf_);
        g_graphicsLib->renderbufferStorage(EPixelFormat::DEPTH24_STENCIL8, extend.width, extend.height);
        g_graphicsLib->framebufferRenderbuffer(EFrameBufferType::FRAME_BUFFER,
            EAttachmentType::DEPTH_STENCIL_ATTACHMENT, hDepthBuf_);

        EFrameBufferState state = g_graphicsLib->checkFramebufferStatus(EFrameBufferType::FRAME_BUFFER);
        if (state != EFrameBufferState::FRAMEBUFFER_COMPLETE) {
            assert(false);
        }

        g_graphicsLib->bindFrameBuffer(0);

        hFrameBuf_ = framebuf;

        RenderAPI::cmdQuad(frameBufVertexDataHandle_);
    }

    void RenderPassSystem::bindFrameBuffer() {
        g_graphicsLib->bindFrameBuffer(hFrameBuf_);
        g_graphicsLib->enable(ECapability::DEPTH_TEST);
    }

    void RenderPassSystem::flushFrameBuffer() {
        g_graphicsLib->bindFrameBuffer(0);
        g_graphicsLib->disable(ECapability::DEPTH_TEST);

        auto pro = g_shaderManager->getShaderProgram(SP_FRAME_BUFFER);
        g_graphicsLib->useProgram(pro);
        g_graphicsLib->bindVertexArray(frameBufVertexDataHandle_.vao);
        g_graphicsLib->bindTexture(ETextureType::TEXTURE_2D, hColorBuf_);
        g_graphicsLib->drawArray(EPrimitiveType::TRIANGLES, 0, 6);
    }

    void RenderPassSystem::createDepthMap() {
        g_graphicsLib->genFrameBuffers(1, &hShadowFBO_);
        g_graphicsLib->bindFrameBuffer(hShadowFBO_);

        hShadowDepthTex_ = g_graphicsLib->genTexture();

        TexImage2D input;
        input.handle = hShadowDepthTex_;
        input.data = nullptr;
        input.width = shadowWidth_;
        input.height = shadowHeight_;
        input.pixelDataType = EBasicDataType::FLOAT;
        input.inputPixelFormat = EPixelFormat::DEPTH_COMPONENT;
        input.internalPixelFormat = EPixelFormat::DEPTH_COMPONENT;
        input.wrapperMode = ETextureWrapper::REPEAT;
        input.filter = ETextureMinMagFilter::NEAREST;
        g_graphicsLib->texImage2D(input);

        FrameBufferTexture2D fb;
        fb.target = EFrameBufferType::FRAME_BUFFER;
        fb.attachment = EAttachmentType::DEPTH_ATTACHMENT;
        fb.textarget = ETextureType::TEXTURE_2D;
        fb.hTex = hShadowDepthTex_;
        fb.level = 0;
        g_graphicsLib->framebufferTexture2D(fb);

        g_graphicsLib->setDrawBuffer(EDrawBuffer::NONE);
        g_graphicsLib->setReadBuffer(EDrawBuffer::NONE);

        EFrameBufferState state = g_graphicsLib->checkFramebufferStatus(EFrameBufferType::FRAME_BUFFER);
        if (state != EFrameBufferState::FRAMEBUFFER_COMPLETE) {
            assert(false);
        }

        g_graphicsLib->bindFrameBuffer(0);

        RenderAPI::cmdQuad(shadowVertexDataHandle_);
    }

    void RenderPassSystem::renderDepthMap(bool toScreen) {
        if (hShadowFBO_ == -1) return;
        SLevel* level = world_->getCurLevel();
        DirectLight* dl = level->getDirectLight();
        if (dl == nullptr) return;

        g_graphicsLib->setViewport(0,0,shadowWidth_, shadowHeight_);

        g_graphicsLib->bindFrameBuffer(hShadowFBO_);
        g_graphicsLib->enable(ECapability::DEPTH_TEST);
        g_graphicsLib->clear(EBitField::DEPTH_BUFFER_BIT);


        Matrix4x4 vp = dl->getProjMatrix() * dl->getLookAtMatrix();
        //Matrix4x4 vp = world_->getMainPlayerCameraVPMatrix();

        auto pro = g_shaderManager->getShaderProgram(SP_SIMPLE_SHADOW);
        unsigned int proId = pro.getId();
        g_graphicsLib->useProgram(pro);
        g_graphicsLib->setMat4(proId, U_LIGHT_SPACE_MAT, vp.data(), 1);

        RenderScene input;
        input.useGlobalShader = true;
        input.globalShader = pro;
        sceneRendering(input);

        g_graphicsLib->bindFrameBuffer(0);
        if (toScreen) {
            flushDepthMap();
        }
        
    }

    void RenderPassSystem::createNormalMap() {
        g_graphicsLib->genFrameBuffers(1, &hNormalFBO_);
        g_graphicsLib->bindFrameBuffer(hNormalFBO_);

        hNormalTex_ = g_graphicsLib->genTexture();

        TexImage2D input;
        input.handle = hNormalTex_;
        input.data = nullptr;
        input.width = normalWidth_;
        input.height = normalHeight_;
        input.pixelDataType = EBasicDataType::UNSIGNED_BYTE;
        input.inputPixelFormat = EPixelFormat::RGB;
        input.internalPixelFormat = EPixelFormat::RGB;
        input.wrapperMode = ETextureWrapper::REPEAT;
        input.filter = ETextureMinMagFilter::NEAREST;
        g_graphicsLib->texImage2D(input);

        FrameBufferTexture2D fb;
        fb.target = EFrameBufferType::FRAME_BUFFER;
        fb.attachment = EAttachmentType::COLOR_ATTACHMENT0;
        fb.textarget = ETextureType::TEXTURE_2D;
        fb.hTex = hNormalTex_;
        fb.level = 0;
        g_graphicsLib->framebufferTexture2D(fb);

        //创建深度附件buffer
        g_graphicsLib->genRenderbuffers(1, &hNormalDepthTex_);
        g_graphicsLib->bindRenderbuffer(hNormalDepthTex_);
        g_graphicsLib->renderbufferStorage(EPixelFormat::DEPTH24_STENCIL8, normalWidth_, normalHeight_);
        g_graphicsLib->framebufferRenderbuffer(EFrameBufferType::FRAME_BUFFER,
            EAttachmentType::DEPTH_STENCIL_ATTACHMENT, hNormalDepthTex_);

        EFrameBufferState state = g_graphicsLib->checkFramebufferStatus(EFrameBufferType::FRAME_BUFFER);
        if (state != EFrameBufferState::FRAMEBUFFER_COMPLETE) {
            assert(false);
        }

        g_graphicsLib->bindFrameBuffer(0);

        RenderAPI::cmdQuad(hNormalDataHandle_);
    }

    void RenderPassSystem::renderNormalMap(bool toScreen) {
        if (hNormalFBO_ == -1) return;
        SLevel* level = world_->getCurLevel();
        DirectLight* dl = level->getDirectLight();
        if (dl == nullptr) return;

        int width = 0;
        int height = 0;

 
        g_graphicsLib->setViewport(0,0,normalWidth_, normalHeight_);

        g_graphicsLib->bindFrameBuffer(hNormalFBO_);
        g_graphicsLib->enable(ECapability::DEPTH_TEST);

        vec4 clearColor = g_cvarCenter->getCVar<vec4>(CVAR_CLEAR_COLOR);
        g_graphicsLib->clearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        g_graphicsLib->clear(EBitField::COLOR_BUFFER_BIT | EBitField::DEPTH_BUFFER_BIT);


        Matrix4x4 vp = world_->getMainPlayerCameraVPMatrix();

        auto pro = g_shaderManager->getShaderProgram(SP_NORMAL_MAP);
        unsigned int proId = pro.getId();
        g_graphicsLib->useProgram(pro);
        g_graphicsLib->setMat4(proId, U_VP, vp.data(), 1);

        RenderScene input;
        input.useGlobalShader = true;
        input.globalShader = pro;
        sceneRendering(input);

        g_graphicsLib->bindFrameBuffer(0);
        if (toScreen) {
            flushNormalMap();
        }

    }

    void RenderPassSystem::flushNormalMap() {
        g_graphicsLib->disable(ECapability::DEPTH_TEST);

        // reset viewport
        Rect rc = g_platformIO->getViewportGeometry();
        g_graphicsLib->setViewport(rc.x,rc.y, rc.w, rc.h);

        vec4 clearColor = g_cvarCenter->getCVar<vec4>(CVAR_CLEAR_COLOR);
        g_graphicsLib->clearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        g_graphicsLib->clear(EBitField::COLOR_BUFFER_BIT | EBitField::DEPTH_BUFFER_BIT);

        auto pro = g_shaderManager->getShaderProgram(SP_NORMAL_MAP_FLUSH);
        g_graphicsLib->useProgram(pro);
        g_graphicsLib->bindVertexArray(hNormalDataHandle_.vao);
        g_graphicsLib->bindTexture(ETextureType::TEXTURE_2D, pro.getId(), U_SCREEN_TEXTURE, hNormalTex_);

        g_graphicsLib->drawArray(EPrimitiveType::TRIANGLES, 0, 6);
        g_graphicsLib->bindVertexArray(0);
    }

    void RenderPassSystem::flushDepthMap() {
        g_graphicsLib->disable(ECapability::DEPTH_TEST);

        // reset viewport
        setViewPort();

        vec4& clearColor = g_cvarCenter->getCVar<vec4>(CVAR_CLEAR_COLOR);
        g_graphicsLib->clearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        g_graphicsLib->clear(EBitField::COLOR_BUFFER_BIT | EBitField::DEPTH_BUFFER_BIT);

        auto pro = g_shaderManager->getShaderProgram(SP_SIMPLE_SHADOW_FLUSH);
        g_graphicsLib->useProgram(pro);
        g_graphicsLib->bindVertexArray(shadowVertexDataHandle_.vao);
        g_graphicsLib->bindTexture(ETextureType::TEXTURE_2D, pro.getId(), U_SHADOW_MAP, hShadowDepthTex_);

        g_graphicsLib->drawArray(EPrimitiveType::TRIANGLES, 0, 6);
        g_graphicsLib->bindVertexArray(0);
    }

    Matrix4x4 RenderPassSystem::getDLightVPMat() {
        DirectLight* dl = level_->getDirectLight();
        if (!dl) {
            return Matrix4x4();
        }
        Matrix4x4 vp = dl->getProjMatrix() * dl->getLookAtMatrix();
        return vp;
    }

    //背景天空盒
    void RenderPassSystem::createBackGroundImage() {

    }

    void RenderPassSystem::flushBackGround() {


    }

    void RenderPassSystem::sceneCulling() {
#ifdef PRINT_OPEN
        Clock clock;
#endif
        assert(level_ != nullptr);
        auto& actors = level_->getActors();

        objsAfterCulled_.clear();
        for (auto& it : actors) {
            travel<GameObject*>(it.second, [this](GameObject* gameobj) mutable {
                //去掉剔除的和不需要渲染的
                if (gameobj->isNeedDisplay_ && !gameobj->culled_) {
                    objsAfterCulled_.push_back(gameobj);
                }
                });
        }
#ifdef PRINT_OPEN
        printf("%s:%d\n",__FUNCTION__, (int)clock.getMicroDuration());
        clock.reset();
#endif
    }

    //场景对象排序
    void RenderPassSystem::sceneSort() {
#ifdef PRINT_OPEN
        Clock clock;
#endif

        //先按照shader排序，再按照材质排序
        std::sort(objsAfterCulled_.begin(), objsAfterCulled_.end(), GameObjectCompare());

#ifdef PRINT_OPEN
        printf("renderPass_sort_time:%d\n", (int)clock.getMicroDuration());
        clock.reset();
#endif
    }

    void RenderPassSystem::sceneInstanceBatching() {
#ifdef PRINT_OPEN
        Clock clock;
#endif
        createdObjs_.clear();
        renderObjs_.clear();

        //instance batching ，对shaderPro相同且材质相同且组件相同的合并为相同instance     
        int start = 0;
        auto start_iter = objsAfterCulled_.begin();
        GameObjectCompare compare;
        for (auto iter = objsAfterCulled_.begin(); iter != objsAfterCulled_.end(); ++iter) {
            auto iter_next = iter; ++iter_next;
            if (iter_next != objsAfterCulled_.end() && compare.equal(*iter, *iter_next)) {
                continue;
            }
            else {
                //单独一个物体，直接加入渲染队列
                if (iter == start_iter) {
                    renderObjs_.push_back(*start_iter);
                    start_iter = iter + 1;
                    continue;
                }
                GameObject* obj = instanceBatching(start_iter, iter);
                //合批失败，则单独渲染
                if (obj == nullptr) {
                    auto end = iter + 1;
                    for (auto k = start_iter; k != end; ++k) {
                        renderObjs_.push_back(*k);
                    }
                    start_iter = iter + 1;
                    continue;
                }
                createdObjs_.push_back(obj);
                renderObjs_.push_back(obj);
                start_iter = iter + 1;
            }

        }
#ifdef PRINT_OPEN
        printf("%s:%d\n",__FUNCTION__, (int)clock.getMicroDuration());
        clock.reset();
#endif
    }

    void RenderPassSystem::sceneNormalBatching(){




    }

    void RenderPassSystem::sceneRendering(const RenderScene& param) {
        //renderSceneTest(); return;

        Matrix4x4 view = world_->getMainPlayerCameraViewMatrix();
        Matrix4x4 proj = world_->getMainPlayerCameraProjMatrix();
        Matrix4x4 vp = proj * view;
        vec3 viewPos = world_->getMainPlayerViewPos();

        for (int i = 0; i < renderObjs_.size(); ++i) {
            //objPass
            GameObject* cur = renderObjs_[i];
            GameObject* pre = (i == 0) ? nullptr : renderObjs_[i - 1];

            //对于多shaderPass的先特化处理
            if (cur->hasMultiShaderPass()) {
                cur->render();
                continue;
            }

            if (i != 0) {
                assert(!(cur->getShaderProgram() < pre->getShaderProgram()));
            }
            if ((param.useGlobalShader == false) &&
                (i == 0 || pre->getShaderProgram() < cur->getShaderProgram())) {
                g_graphicsLib->useProgram(cur->getShaderProgram());
                g_graphicsLib->setMat4(cur->getShaderProgram().getId(), U_VP, vp.data(), 1);
                g_graphicsLib->setVec3(cur->getShaderProgram().getId(), U_VIEWPOS, viewPos);
            }
            if (i == 0 || pre->getComponent<MaterialCom>() < cur->getComponent<MaterialCom>()) {
                //材质更新
                cur->bindUniformData_Material();
            }
            if (cur->getCulled()) {
                g_graphicsLib->enable(ECapability::CULL_FACE);
            }
            else {
                g_graphicsLib->disable(ECapability::CULL_FACE);
            }

            cur->bindTransform(param.globalShader);
            //cur->bindUniformData_Material();
            cur->bindUniformData();
            cur->bindVertexData();
            cur->updateVertexData();
            cur->drawCall();
            cur->unBindVertexData();
            cur->unBindUniformData();
            //cur->unBindUniformData_Material();

        }

    }

    void RenderPassSystem::sceneEndProcess() {
        //释放内存
        for (auto& it : createdObjs_) {
            //how to release a subclass of GameObject? FIXME!may use a memroy Pool like box2d
            ActorFactory<GameObject>::get()->free(it);
        }
        createdObjs_.clear();
    }

    //上传视口的 视图，投影，vp矩阵
    void RenderPassSystem::uploadViewTransform(const ShaderProgram& pro) {
        auto id = pro.getId();
        auto vp = world_->getMainPlayerCameraVPMatrix();
        g_graphicsLib->setMat4(id, U_VP, vp.data(), 1);
        auto proj = world_->getMainPlayerCameraProjMatrix();
        g_graphicsLib->setMat4(id, U_PROJ, proj.data(), 1);
        auto view = world_->getMainPlayerCameraViewMatrix();
        g_graphicsLib->setMat4(id, U_VIEW, view.data(), 1);
    }

    void RenderPassSystem::setViewPort() {
        //Extend ext = g_platformIO->getFrameBufferSize();
        //ViewPortRatio ratio = g_cvarCenter->getViewPortRatio();

        //g_graphicsLib->setViewport(ext.width * ratio.x, ext.height * ratio.y,
        //    ext.width * ratio.w, ext.height * ratio.h);

        Rect rc = g_platformIO->getViewportGeometry();
        g_graphicsLib->setViewport(rc.x, rc.y, rc.w, rc.h);
    }

    //按照涉及到的着色器进行遍历
    void RenderPassSystem::renderPass() {
        sceneCulling();
        sceneSort();
        sceneInstanceBatching();

        //阴影贴图pass
        bool useShadowMap = g_cvarCenter->getCVar<bool>(CVAR_OPEN_SHADOW_MAP);
        bool to_screen = g_cvarCenter->getCVar<bool>(CVAR_SHOW_SCREEN_SHADOW_MAP);
        to_screen = false;
        if (useShadowMap) {
            renderDepthMap(to_screen);
            if(to_screen)
                return;
        }

        //法线贴图
        bool useNormalMap = g_cvarCenter->getCVar<bool>(CVAR_OPEN_NORMAL_MAP);
        bool normalToScreen = g_cvarCenter->getCVar<bool>(CVAR_SHOW_SCREEN_NORMAL_MAP);
        if (useNormalMap) {
            renderNormalMap(normalToScreen);
            if (normalToScreen) return;
        }
        

        //主渲染pass
        useFrameBuffer_ = false;
        if (useFrameBuffer_) {
            bindFrameBuffer();
        }
        setViewPort();

        //清frameBuffer
        vec4& clearColor = g_cvarCenter->getCVar<vec4>(CVAR_CLEAR_COLOR);
        g_graphicsLib->clearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        g_graphicsLib->clear(EBitField::COLOR_BUFFER_BIT | EBitField::DEPTH_BUFFER_BIT);
        
        
        RenderScene input;
        input.useGlobalShader = false;

        //绑定阴影贴图
        if (useShadowMap){
            auto pro = g_shaderManager->getShaderProgram(SP_NORMAL_PASS);
            g_graphicsLib->useProgram(pro);
            g_graphicsLib->bindTexture(ETextureType::TEXTURE_2D, pro.getId(), U_SHADOW_MAP, hShadowDepthTex_);
            auto mat = getDLightVPMat();
            g_graphicsLib->setMat4(pro.getId(), U_LIGHT_SPACE_MAT, mat.data(), 1);
        }
        

        sceneRendering(input);
        if (useFrameBuffer_) {
            flushFrameBuffer();
        }

        normalVisual_ = true;
        if (normalVisual_) {
            RenderScene input;
            input.useGlobalShader = true;
            input.globalShader = g_shaderManager->getShaderProgram(SP_NORMAL_VISUAL);
            g_graphicsLib->useProgram(input.globalShader);
            uploadViewTransform(input.globalShader);

            sceneRendering(input);
        }

        sceneEndProcess();
    }
}