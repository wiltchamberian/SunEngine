#include "RayTracingComponent.h"
#include <thread>
#include <chrono>
#include <mutex>
#include "Render/GLFunctions.h"
#include "Render/ShaderManager.h"
#include "Math/Random.h"
#include "Primitive/VertexBufferLayout.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/GraphicsLib.h"
#include "Common/BasicDataType.h"

namespace Sun {

    // 
        //glTexImage3D(GL_TEXTURE_2D, 0, GL_RGB, 100,
        //    100, 0, GL_RGB,
        //    GL_UNSIGNED_BYTE, noise3DTexPtr);
        //GLuint adsIndex = :glGetSubroutineIndex(g_shaderManager->rayTracingProgram_, GL_COMPUTE_SHADER, "phongModel");
        //GLuint diffuseIndex = glGetSubroutineIndex(g_shaderManager->rayTracingProgram_, GL_COMPUTE_SHADER, "diffuseOnly");
        //glUniformSubroutinesuiv(GL_COMPUTE_SHADER, 1, &adsIndex);
        //glUniformSubroutinesuiv(GL_COMPUTE_SHADER, 1, &diffuseIndex);

    void RayTracingComponent::prepareRender() {
#if 0
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
#endif
        //prepareGenRay();

        int work_grp_cnt[3];
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

        int work_grp_size[3];
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);

        int work_grp_inv;
        glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);


        prepareRayTracing();

        prepareRayShow();
    }

    void RayTracingComponent::prepareGenRay() {
        //g_render->useProgram(g_shaderManager->rayGenProgram_);

        //计算与生成着色器已合并
        g_graphicsLib->useProgram(g_shaderManager->rayTracingProgram_);
        int loc = 0;
        RCamera camera;
        camera.origin = { 0.f,0.f,0.f };
        camera.lower_left_corner = { -2,-1,-1 };
        camera.horizontal = { 4.f,0.f,0.f };
        camera.vertical = { 0.f,2.f,0.f };
        loc = glGetUniformLocation(g_shaderManager->rayGenProgram_.getId(), "camera.origin");
        glUniform3f(loc, camera.origin.x, camera.origin.y, camera.origin.z);
        loc = glGetUniformLocation(g_shaderManager->rayGenProgram_.getId(), "camera.lower_left_corner");
        glUniform3f(loc, camera.lower_left_corner.x, camera.lower_left_corner.y, camera.lower_left_corner.z);
        loc = glGetUniformLocation(g_shaderManager->rayGenProgram_.getId(), "camera.horizontal");
        glUniform3f(loc, camera.horizontal.x, camera.horizontal.y, camera.horizontal.z);
        loc = glGetUniformLocation(g_shaderManager->rayGenProgram_.getId(), "camera.vertical");
        glUniform3f(loc, camera.vertical.x, camera.vertical.y, camera.vertical.z);
        //ray

        hRayBuf_ =g_graphicsLib->genBufferData
        (nullptr, W_ * H_ * N_ * N_ * sizeof(RRay), EBufferType::SHADER_STORAGE_BUFFER, EDrawMode::STATIC_DRAW);
        g_graphicsLib->bindBufferBase(EBufferType::SHADER_STORAGE_BUFFER, 1, hRayBuf_);
        //生成射线
        //glDispatchCompute(W_, H_, 1);

        //glFlush();

        GLenum er = glGetError();
    }

    void RayTracingComponent::prepareRayTracing() {
        g_graphicsLib->useProgram(g_shaderManager->rayTracingProgram_);
        //glGetIntegerv()并设置pname为GL_MAX_COMPUTE_SHARED_MEMORY_SIZE
        // 
#if 0
        //设置噪声
        unsigned int hNoiseTex = 0;
        //随机生成噪声纹理
        const int len = 10000;
        std::vector<vec3> vec(len);
        randomSeed();
        vec3 v;
        for (int i = 0; i < len; ++i) {
            vec[i] = randomPointInUnitSphere();
        }
        //生成噪声纹理，并存入（用于随机数)
        glGenTextures(1, &hNoiseTex);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, hNoiseTex);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 100, 100, 0, GL_RGB, GL_FLOAT, vec.data());
        GLint t1 = glGetUniformLocation(g_renderManager->rayTracingProgram_->getId(), "Noise");
        glUniform1i(t1, 0);
#endif
        //输入相机
        RCamera camera;
        camera.origin = vec3(0, 0, 2);
        camera.horizontal = vec3(2, 0, 0);
        camera.vertical = vec3(0, 2, 0);
        camera.lower_left_corner = vec3(-1, -1, 1);
        int loc = -1;
        int proId = g_shaderManager->rayGenProgram_.getId();
        loc = glGetUniformLocation(proId, "camera.origin");
        glUniform3f(loc, camera.origin.x, camera.origin.y, camera.origin.z);
        loc = glGetUniformLocation(proId, "camera.lower_left_corner");
        glUniform3f(loc, camera.lower_left_corner.x, camera.lower_left_corner.y, camera.lower_left_corner.z);
        loc = glGetUniformLocation(proId, "camera.horizontal");
        glUniform3f(loc, camera.horizontal.x, camera.horizontal.y, camera.horizontal.z);
        loc = glGetUniformLocation(g_shaderManager->rayGenProgram_.getId(), "camera.vertical");
        glUniform3f(loc, camera.vertical.x, camera.vertical.y, camera.vertical.z);
        

        //生成Entity
        const int numEnt = 3;
        std::vector<Entity> entities(numEnt);
        for (int i = 0; i < numEnt; ++i) {
            entities[i].offset = i;
            entities[i].hitFuncIndex = 0; //0:sphere 1:bvh
            entities[i].len = sizeof(RSphere);
            entities[i].type = 0;
            entities[i].color = { 0.2,0.2,0.2 };
        }
        entities[0].color = { 0.1,0.2,0.5 };
        entities[1].color = { 0.8,0.8,0 };
        entities[2].color = { 0.8, 0.6, 0.2 };
        //生成一些球体
        std::vector<RSphere> spheres(numEnt);
        spheres[0].center_ = { 0,0,-1 }; spheres[0].radius_ = 0.5;
        spheres[1].center_ = { 0,-100.5,-1 }; spheres[1].radius_ = 100;
        spheres[2].center_ = { 1,0,-1 }; spheres[2].radius_ = 0.5;
        spheres[0].color_ = { 0.1,0.2,0.5 };
        spheres[1].color_ = { 0.8,0.8,0 };
        spheres[2].color_ = { 0.8, 0.6, 0.2 };
        //生成bvh (TODO)

        //gpu球体数组生成
        unsigned int hShperes = g_graphicsLib->genBufferData
        (spheres.data(), spheres.size() * sizeof(RSphere), EBufferType::SHADER_STORAGE_BUFFER, EDrawMode::STATIC_DRAW);
        g_graphicsLib->bindBufferBase(EBufferType::SHADER_STORAGE_BUFFER, 2, hShperes);

        //gpu颜色数组生成
        //unsigned int colorBuf = g_graphicsLib->genBufferData
        //(nullptr, N_ * N_ * W_ * H_ * sizeof(vec3), EBufferType::SHADER_STORAGE_BUFFER, EDrawMode::STATIC_DRAW);
        //g_graphicsLib->bindBufferBase(EBufferType::SHADER_STORAGE_BUFFER, 10, colorBuf);

        //生成输出纹理
        hImgOutput_ = g_graphicsLib->genStorageTexture(ETextureType::TEXTURE_2D, 1, ETextureUnit::TEXTURE0, EPixelFormat::RGBA32F, W_, H_);


        GLenum err1 = glGetError();
    }

    void RayTracingComponent::prepareRayShow() {
#if 0
        g_graphicsLib->useProgram(g_renderManager->rayTracingShowProgram_);
        //g_graphicsLib->useProgram(g_renderManager->rayTracingProgram_);

        GLenum err = glGetError();
        std::vector<vec3> datas = { {-1,-1,0},{1,-1,0},{1,1,0},{-1,1,0} };
        std::vector<int> index = { 0,1,3,2,3,1 };
        //dh_ = g_render->setupDatas(datas.data(), datas.size() * sizeof(vec3), index.data(), index.size() * sizeof(int));

        dh_.vao = g_graphicsLib->createVertexArray();
        dh_.vao->bind();
        SharedPtr<VertexBuffer> vb = g_graphicsLib->createVertexBuffer(datas.data(), datas.size() * sizeof(vec3));
        std::shared_ptr<IndexBuffer> eb = g_graphicsLib->createIndexBuffer((const unsigned int*)index.data(), index.size());
        VertexBufferLayout layout;
        layout.push_back({ 0, 3, EBasicDataType::FLOAT,false, sizeof(Vector3D),0 });
        dh_.vao->addVertexBuffer(vb);
        dh_.vao->addIndexBuffer(eb);
        dh_.vao->addVertexBufferLayout(layout);

        GLenum err1 = glGetError();

        _render();
#endif
    }

    void RayTracingComponent::_render() {
        ;
    }

    void RayTracingComponent::render() {
        
        //光线追踪
        g_graphicsLib->useProgram(g_shaderManager->rayTracingProgram_);

        //射线数据绑定
        //g_render->bindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, hRayBuf_);

        GLenum err = glGetError();
 
        glBindImageTexture(0, hImgOutput_, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glDispatchCompute(W_, H_, 1);

        GLenum err1 = glGetError();

        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        //检测计算结果
#if 0
        GLint bufMask = GL_MAP_READ_BIT;
        Ray* ptr = (Ray*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, rayNum * sizeof(Ray), bufMask);
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
#endif

        //光栅化显示
        g_graphicsLib->useProgram(g_shaderManager->rayTracingShowProgram_);
        //glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        //glBindVertexArray();
        glActiveTexture(GL_TEXTURE0);


        glBindTexture(GL_TEXTURE_2D, hImgOutput_);

        glBindVertexArray(dh_.vao->getId());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    }


}
