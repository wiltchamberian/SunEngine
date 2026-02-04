#include "ParticleComponent.h"
#include "Render/GraphicsLib.h"
#include "Render/ShaderManager.h"
#include "Math/Random.h"
#include "Render/GLFunctions.h" //FIXME
#include "Engine.h"
#include "Common/Define.h"

namespace Sun {
#define PARTICLE_COUNT 128*100
#define PARTICLE_GROUP_COUNT 100
    void ParticleComponent::prepareRender() {
        randomSeed();

        //pro_ = g_renderManager->particleProgram_->getId();
        //proShow_ = g_renderManager->particleShowProgram_->getId();
        g_graphicsLib->useProgram(g_shaderManager->getShaderProgram(SP_PARTICLE_SHADER));

        vao_ = g_graphicsLib->genVertexArray();
        g_graphicsLib->bindVertexArray(vao_);

        positionBuf_ = g_graphicsLib->genBufferData(nullptr, PARTICLE_COUNT * sizeof(vec4), EBufferType::ARRAY_BUFFER, EDrawMode::DYNAMIC_COPY);
        vec4* positions = (vec4*)
            glMapBufferRange(GL_ARRAY_BUFFER,
                0,
                PARTICLE_COUNT * sizeof(vec4),
                GL_MAP_WRITE_BIT |
                GL_MAP_INVALIDATE_BUFFER_BIT);
        for (int i = 0; i < PARTICLE_COUNT; i++)
        {
            vec3 ran = randomVector3D(-10.0f, 10.0f, -10., 10., -10., 10.);
            positions[i] = vec4(ran.x,ran.y,ran.z,rand48());
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), (void*)(0));

        //velocity
        //glEnableVertexAttribArray(1);
        //glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);

        velBuf_ = g_graphicsLib->genBufferData(nullptr, PARTICLE_COUNT * sizeof(vec4), EBufferType::ARRAY_BUFFER, EDrawMode::DYNAMIC_COPY);
        vec4* velocities = (vec4*)
            glMapBufferRange(GL_ARRAY_BUFFER,
                0,
                PARTICLE_COUNT * sizeof(vec4),
                GL_MAP_WRITE_BIT |
                GL_MAP_INVALIDATE_BUFFER_BIT);
        for (int i = 0; i < PARTICLE_COUNT; i++)
        {
            vec3 ran = randomVector3D(-0.1f, 0.1f, -0.1, 0.1, -0.1, 0.1);
            velocities[i] = vec4(ran.x, ran.y, ran.z, 0.0);
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);

        // ---------------------
        // 创建 TBO
        glGenTextures(2, tbo_);

        glBindTexture(GL_TEXTURE_BUFFER, tbo_[0]);
        glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, positionBuf_);
        glBindTexture(GL_TEXTURE_BUFFER, tbo_[1]);
        glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, velBuf_);

        // ---------------------
        // 创建 UBO
        glGenBuffers(1, &attractor_buffer_);
        glBindBuffer(GL_UNIFORM_BUFFER, attractor_buffer_);
        glBufferData(GL_UNIFORM_BUFFER, 32 * sizeof(vec4), NULL, GL_STATIC_DRAW);	// 仅初始化，静态拷贝

        // attractor 的最后一位，表示粒子质量
        for (int i = 0; i < 64; ++i) {
            attractor_masses_[i] = rand48() * 0.5 + 0.5;
        }

        glBindBufferBase(GL_UNIFORM_BUFFER, 0, attractor_buffer_);

        // --------------------------------------


    }

    void ParticleComponent::tick(float dtime, const SEvent& ev) {
        // Update the buffer containing the attractor positions and masses
        vec4* attractors =
            (vec4*)glMapBufferRange(GL_UNIFORM_BUFFER,
                0,
                32 * sizeof(vec4),
                GL_MAP_WRITE_BIT |
                GL_MAP_INVALIDATE_BUFFER_BIT);
        int i;
        static float time = 0.0;
        time += dtime;
        for (i = 0; i < 32; i++)
        {
            attractors[i] =
                vec4(sinf(time * (float)(i + 4) * 7.5f * 20.0f) * 50.0f,
                    cosf(time * (float)(i + 7) * 3.9f * 20.0f) * 50.0f,
                    sinf(time * (float)(i + 3) * 5.3f * 20.0f) *
                    cosf(time * (float)(i + 5) * 9.1f) * 100.0f,
                    attractor_masses_[i]);
        }
        glUnmapBuffer(GL_UNIFORM_BUFFER);
        dtime_ = dtime;
    }

    void ParticleComponent::beginRender() {

    }

    void ParticleComponent::render() {
        
        // Activate the compute program and bind the position
        // and velocity buffers
        g_graphicsLib->useProgram(g_shaderManager->getShaderProgram(SP_PARTICLE_SHADER));
        glBindImageTexture(0, tbo_[0], 0,
            GL_FALSE, 0,
            GL_READ_WRITE, GL_RGBA32F);
        glBindImageTexture(1, tbo_[1], 0,
            GL_FALSE, 0,
            GL_READ_WRITE, GL_RGBA32F);
        // Set delta time
        g_graphicsLib->setFloat(g_shaderManager->getShaderProgram(SP_PARTICLE_SHADER).getId(), "dt", dtime_);
        //glUniform1f(dt_location, info.dtime);
        // Dispatch the compute shader
        glDispatchCompute(PARTICLE_GROUP_COUNT, 1, 1);
        // Ensure that writes by the compute shader have completed
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        // Set up our mvp matrix for viewing
        /*vmath::mat4 mvp = vmath::perspective(45.0f, aspect_ratio,
            0.1f, 1000.0f) *
            Examples 641
            ptg9898810
            vmath::translate(0.0f, 0.0f, -60.0f) *
            vmath::rotate(time * 1000.0f,
                vmath::vec3(0.0f, 1.0f, 0.0f));*/
        // Clear, select the rendering program and draw a full screen quad
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glUseProgram(proShow_);
        g_graphicsLib->useProgram(g_shaderManager->getShaderProgram(SP_PARTICLE_SHOW_SHADER));
        //glUniformMatrix4fv(0, 1, GL_FALSE, mvp);
        /*glBindVertexArray(render_vao);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);*/

        SharedPtr<SCameraComponent> camera = SUN->getWorld()->getCameraCom();
        Matrix4x4 view = camera->getLookAtMatrix();
        Matrix4x4 proj = camera->getProjectionMatrix();
        //Matrix4x4 model = getWorldMatrix(); FIX_ME
        Matrix4x4 model;
        Matrix4x4 mvp = proj * view * model;
        Matrix4x4 vp = proj * view;

        //glUseProgram(proShow_);
        g_graphicsLib->setMat4(g_shaderManager->getShaderProgram(SP_PARTICLE_SHOW_SHADER).getId(), U_VP, vp.data(),1);
        g_graphicsLib->bindVertexArray(vao_);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glDisable(GL_DEPTH_TEST);

        //glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glPointSize(2.0f);
        glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);
    }

    void ParticleComponent::endRender() {

    }

}