/*****************************************************************************
* @brief : GraphicsLib
* 将不同的图形库抽象出来，根据需要可以动态切换(Opengl,Vulkan,D3d,Metal等)
* 采用依赖倒置原理，图形库只依赖抽象组件，而具体组件实现也依赖抽象组件
* 同时，也可看成抽象工厂
* @author : acedtang
* @date : 2021/5/12
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __GRAPHICS_LIB_H
#define __GRAPHICS_LIB_H

#include <memory>
#include <vector>
#include <string>

#include "Math/Vector4D.h"
#include "Math/Vector3D.h"
#include "Math/Vector2D.h"
#include "Math/Matrix4x4.h"
#include "Math/Matrix3x3.h"
#include "Render/Shader.h"
#include "Render/Program.h"
#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Primitive/VertexBufferElement.h"
#include "Primitive/VertexBufferLayout.h"
#include "Primitive/VertexData.h"
#include "Render/RenderHandle.h"
#include "Common/Common.h"
#include "Common/PixelFormat.h"
#include "Common/BasicDataType.h"
#include "Render/RenderInputStruct.h"

namespace Sun{

    /* 封RHI和Vulkan公共的函数接口统计
    
    1, createProgram统一为 createGraphicsPipeline

    2, deleteProgram 统一为 destroyGraphicsPipeline

    3, setLayout 统一为到createGraphicsPipeline中
    
    
    */

    class GraphicsLib {
    public:
        virtual void initEnvironment() = 0;
        //1级接口(所谓1级的含义是最底层的接口，一般都是一个极小粒度的功能，无法再分解，或者只是
        //几个相关极强的图形API函数组合)
        virtual ShaderProgram createProgram(const Shader& shader) { return ShaderProgram(); }
        virtual ShaderProgram createProgram(const std::vector<Shader>& vec) { return ShaderProgram(); }
        virtual void deleteProgram(const ShaderProgram& pro) {}
        virtual Shader createShader(const char* str, EShaderType type, bool pathOrSrc) { return Shader(); }
        virtual void deleteShader(const Shader& shader) {}
        virtual unsigned int genVertexArray() { return 0; }
        virtual void deleteVertexArray(unsigned int) {}
        virtual void bindVertexArray(unsigned int id) {}
        virtual void bindVertexBuffer(unsigned int id) {}
        virtual unsigned int genVertexBuffer(const void* data, long long byteSize, EDrawMode drawType) { return INVALID_HANDLE; }
        virtual unsigned int genIndexBuffer(const void* data, long long byteSize, EDrawMode drawType) { return INVALID_HANDLE; }
        virtual void bindBuffer(unsigned int target, unsigned int) {}
        virtual void enable(ECapability) {}
        virtual void disable(ECapability) {}

        virtual void bindWindow(void* hwnd) {}
        virtual void load() {}
        virtual void useProgram(const ShaderProgram&) {}

        virtual unsigned int  readTexture(const char* path, const std::string& directory, bool gamma = false) { return INVALID_HANDLE; }

        virtual void drawArray(EPrimitiveType drawType, int first, int byteSiz) {}
        virtual void drawArrayInstanced(EPrimitiveType drawType, int first, int byteSiz, int instanceCount) {}
        virtual void drawElements(EPrimitiveType mode, int byteSiz, EBasicDataType type, const void* indices) {}
        virtual void drawElementsInstanced(EPrimitiveType mode, int byteSiz, EBasicDataType type, const void* indices,int instanceCount) {}

        virtual void setBool(unsigned int pro, const std::string& name, bool value) {}
        virtual void setInt(unsigned int pro, const std::string& name, int value) {}
        virtual void setFloat(unsigned int pro, const std::string& name, float value) {}
        // ------------------------------------------------------------------------
        virtual void setVec2(unsigned int pro, const std::string& name, const vec2& value) {}
        virtual void setVec2(unsigned int pro, const std::string& name, float x, float y) {}
        // ------------------------------------------------------------------------
        virtual void setVec3(unsigned int pro, const std::string& name, const vec3& value) {}

        virtual void setVec3(unsigned int pro, const std::string& name, float x, float y, float z) {}
        // ------------------------------------------------------------------------
        virtual void setVec4(unsigned int pro, const std::string& name, const vec4& value) {}

        //virtual void setMat2(const std::string& name, const Matrix2x2& mat) = 0;
 
        virtual void setMat3(unsigned int pro, const char* name, const float* mat ,int count) {}

        //4*4 mat with count row major
        virtual void setMat4(unsigned int pro, const char* name, const float* mat, int count) {}

        virtual int loadTexture(const std::string& path) { return INVALID_HANDLE; }
        //virtual int genTexture(unsigned char* data, int width, int height, int channel) = 0;
        virtual unsigned int genTexture() { return INVALID_HANDLE; }
        virtual int texImage2D(const TexImage2D& input) {
            return INVALID_HANDLE;
        }
        virtual int loadCubemap(const std::vector<std::string>& faces) {
            return INVALID_HANDLE;
        }
        //pro是着色器id,textureHandle是表示纹理句柄，type表示纹理的类型(gl:GL_TEXTURE_2D),
        // index表示纹理索引(gl:GL_TEXTURE0),offsetInShader:位于着色器中第几个，name:纹理在着色器中的名字
        virtual void bindTexture(int pro, int textureHandle, ETextureType type, ETextureUnit index, int offsetInShader, const char* texureNameInShader) {}
        virtual void bindTexture(ETextureType type ,int textureHandle) {}
        //virtual void unBindTexture(ETextureType type, int textureHandle) = 0;
        //从纹理名和着色器获取到纹理单元索引，将纹理绑定到该单元的type类型上
        virtual void bindTexture(ETextureType type, int pro, const char* texureNameInShadadr,int textureHandle) {}

        virtual void unBindTexture(int pro, int textureHandle, ETextureType type, ETextureUnit index, int offsetInShader, const char* texureNameInShader) {}
        virtual void deleteTexture(int textureHandle) {}

        virtual void genFrameBuffers(int, unsigned int* ids) {}
        virtual void bindFrameBuffer(unsigned int frambuffer) {}

        virtual void framebufferTexture2D(const FrameBufferTexture2D& input) {}

        virtual void setDrawBuffer(EDrawBuffer drawBuffer) {}
        virtual void setReadBuffer(EDrawBuffer drawBuffer) {}

        virtual void drawBuffers(int n,
            const unsigned int* bufs) {}

        virtual void genRenderbuffers(int n,
            unsigned int* renderbuffers) {}

        virtual void bindRenderbuffer(unsigned int renderbuffer) {}

        virtual void renderbufferStorage(
            EPixelFormat internalformat,
            int width,
            int height) {}

        virtual void framebufferRenderbuffer(EFrameBufferType target,
            EAttachmentType attachment,
            unsigned int renderbuffer) {}

        virtual EFrameBufferState checkFramebufferStatus(EFrameBufferType target) {
            return EFrameBufferState::FRAMEBUFFER_UNDEFINED;
        }

        virtual std::shared_ptr<VertexArray> setupDatas(void* vertice, long long verticByteLen, void* indice, long long  indiceByteSiz) {
            return nullptr;
        }

        virtual void setupLayout(int shaderLayoutId, int memberNum, unsigned int memberBasicType, bool normalize, int diff, int offset) {}

        virtual void setupLayoutI(int shaderLayoutId, int memberNum, unsigned int memberBasicType, int diff, int offset) {}

        virtual unsigned int genBufferData(void* data, long long byteLen, EBufferType bufTypeEnum, EDrawMode drawMode) {
            return INVALID_HANDLE;
        }
        virtual void deleteBufferData(unsigned int h) {}
        virtual void bufferSubData(EBufferType targe, int offset, int siz, const void* data) {}
        virtual void bindBufferBase(EBufferType bufTypeEnum, unsigned int index, unsigned int hBuf) {}
        //传递数据到shader_storage buffer
        virtual int setupShaderStorageBuffer(void* data, long long byteLen, unsigned int drawEnum) { return INVALID_HANDLE; }

        virtual unsigned int genStorageTexture(ETextureType target, int levels, ETextureUnit unit, EPixelFormat rgbaFormat, int width, int height) { return INVALID_HANDLE; }
        virtual unsigned int genTexture(ETextureType target, int levels, ETextureType textureType, EPixelFormat rgbaFormat, int width, int height) { return INVALID_HANDLE; }

        virtual void clearColor(float r,float g,float b,float a) {}
        virtual void clear(EBitField field) {}
        virtual void setViewport(int x,int y, int width, int height) {}
        virtual void display(int width, int height) {}
        virtual void flush() {}

        virtual void setVertexBufferLayout(const VertexBufferLayout& layout) {}

        virtual void setPolygonMode(ECullMode cullMode,EFillMode fillMode) {}

        //2级接口（在1级接口基础上组装而成的接口，完成稍微独立的一个子功能，2级接口没有严格评判标准
        //只能靠实现者自己把握，这样分层导出接口是为了方便以后扩展多图形API支持，也是为了方便使用

        virtual VboEboHandle applyForVboEbo(const VboEboCreateInfo&) { return VboEboHandle(); }

        virtual void bindVboEboToVao(VboEboHandle handle, unsigned int vao,const VertexBufferLayout& layout) {}

        virtual void drawCall(const GpuVertexDataHandle& handle) {}
        //handle为输出
        virtual void applyGpuVertexData(GpuVertexDataHandle& handle, const VertexBufferLayout& layout, 
            void* vertice, int verticeCount, int verticeSize, void* indice, int indexCount, int indexSize, EDrawMode usage) {
            ;
        }

        virtual void applyGpuVertexDataArray(GpuVertexDataHandle& handle, const VertexDataArray& vertexData) {}
        virtual ShaderProgram createShaderProgram(const ProgramCreateInfo& createInfo) { return ShaderProgram(); }

    public:
        /**************************非虚函数******************************/
        

    protected:

    };
    
    extern GraphicsLib* g_graphicsLib;
}


#endif 