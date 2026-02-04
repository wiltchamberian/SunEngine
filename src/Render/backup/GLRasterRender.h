/**
 * @projectName   Sun
 * @brief         the render engine
 * @author        acedtang
 * @date          2021/01/21
 */

#ifndef __GL_RENDER_H
#define __GL_RENDER_H

#include <string>
#include <vector>
#include "windows.h"
#include "gl/gl.h"
#include "GLMacros.h"
#include "Math/Matrix4x4.h"
#include "Render/RenderHandle.h"
#include "Render/Render.h"
#include "Render/VertexArray.h"

namespace Sun
{
    class SGLRasterRender:public SRender
    {
    public:
        void bindWindow(void* hwnd) override;
        void loadRenderLib() override;
        void displayHandle(const RenderHandle& handle) override;
        void useProgram(unsigned int shaderId) override;
        unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false) override;
        void setModelMatrix(int shader, const Matrix4x4& mat) override;
        void setViewMatrix(int shader, const Matrix4x4& mat) override;
        void setProjectionMatrix(int shader, const Matrix4x4& mat) override;
        void setMatrix(int shader, const float* , int num, const char*) override;
        int loadTexture(const std::string& path) override;
        int loadCubemap(const std::vector<std::string>& faces) override;
        void clear(const Vector4D& rgba) override;
        void setViewport(int width, int height) override;
        void display(int width, int height) override;
        void bindTexture(int texHandle, int texType, int texIndex, int offsetInShader, const char* texNameInShader) override;
        void flush() override;
        std::shared_ptr<VertexArray> setupDatas(void* vertice, long long verticByteLen, void* indice, long long  indiceByteSiz) override;
        int setupBufferData(void* data, long long byteLen, unsigned int bufTypeEnum, unsigned int drawEnum) override;
        void bindBufferBase(unsigned int bufTypeEnum, unsigned int index, unsigned int hBuf) override;
        int setupShaderStorageBuffer(void* data, long long byteLen, unsigned int drawEnum) override;
        void setupLayout(int shaderLayoutId, int memberNum, unsigned int memberBasicType, bool normalize,int diff, int offset) override;
        void setupLayoutI(int shaderLayoutId, int memberNum, unsigned int memberBasicType, int diff, int offset) override;
        
    protected:     
        void* GetProcAddress(const char* name);
    };
}


#endif

