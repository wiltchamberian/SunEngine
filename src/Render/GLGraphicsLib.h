#ifndef __GL_GRAPHICS_LIB_H
#define __GL_GRAPHICS_LIB_H

#include "GraphicsLib.h"

namespace Sun {

    struct UniformKey {
        unsigned int pro = -1;
        std::string name;
    };

    class GLGraphicsLib : public GraphicsLib {
    public:
        //1级接口
        void initEnvironment();
        ShaderProgram createProgram(const Shader& shader) override;
        ShaderProgram createProgram(const std::vector<Shader>& vec) override;
        void deleteProgram(const ShaderProgram& pro) override;
        Shader createShader(const char* str, EShaderType type, bool pathOrSrc) override;
        void deleteShader(const Shader& shader) override;
        unsigned int genVertexArray() override;
        void deleteVertexArray(unsigned int id) override;
        void bindVertexArray(unsigned int id)override;
        void bindVertexBuffer(unsigned int id) override;
        unsigned int genVertexBuffer(const void* data, long long byteSize, EDrawMode drawType) override;
        unsigned int genIndexBuffer(const void* data, long long byteSize, EDrawMode drawType) override;
        void bindBuffer(unsigned int target, unsigned int) override;
        void enable(ECapability) override;
        void disable(ECapability) override;

        //belongs to initEnvironment
        void bindWindow(void* hwnd) override;
        //belongs to initEnvironment
        void load() override;
        //bindPipeline
        void useProgram(const ShaderProgram&)override;

        //not used
        unsigned int  readTexture(const char* path, const std::string& directory, bool gamma = false) override;

        //belongs to drawCall all
        void drawArray(EPrimitiveType mode, int first, int count) override;
        void drawArrayInstanced(EPrimitiveType drawType, int first, int count, int instanceCount) override;
        void drawElements(EPrimitiveType mode, int byteSiz, EBasicDataType type, const void* indices) override;
        void drawElementsInstanced(EPrimitiveType mode, int byteSiz, EBasicDataType type, const void* indices, int instanceCount) override;

        //belongs to descriptor,internal
        void setBool(unsigned int pro, const std::string& name, bool value) override;
        void setInt(unsigned int pro, const std::string& name, int value) override;
        void setFloat(unsigned int pro, const std::string& name, float value) override;
        void setVec2(unsigned int pro, const std::string& name, const vec2& value) override;
        void setVec2(unsigned int pro, const std::string& name, float x, float y) override;
        void setVec3(unsigned int pro, const std::string& name, const vec3& value) override;
        void setVec3(unsigned int pro, const std::string& name, float x, float y, float z) override;
        void setVec4(unsigned int pro, const std::string& name, const vec4& value) override;
        void setMat3(unsigned int pro, const char* name, const float* mat , int count) override;
        void setMat4(unsigned int pro, const char* name, const float* mat , int count) override;

        void genFrameBuffers(int, unsigned int* ids) override;
        void bindFrameBuffer(unsigned int frambuffer) override;

        void framebufferTexture2D(const FrameBufferTexture2D&) override;

        //not used
        void setDrawBuffer(EDrawBuffer drawBuffer) override;
        void setReadBuffer(EDrawBuffer drawBuffer) override;

        //not used
        void drawBuffers(int n,
            const unsigned int* bufs) override;

        void genRenderbuffers(int n,
            unsigned int* renderbuffers) override;

        void bindRenderbuffer(unsigned int renderbuffer) override;

        void renderbufferStorage(
            EPixelFormat internalFormat,
            int width,
            int height) override;

        void framebufferRenderbuffer(EFrameBufferType target,
            EAttachmentType attachment,
            unsigned int renderbuffer) override;

        EFrameBufferState checkFramebufferStatus(EFrameBufferType type) override;

        int loadTexture(const std::string& path) override;
        unsigned int genTexture() override;
        int texImage2D(const TexImage2D& input) override;
        int loadCubemap(const std::vector<std::string>& faces) override;

        //第一个id是表示纹理句柄，type表示纹理的类型(gl:GL_TEXTURE_2D),
        // index表示纹理索引(gl:GL_TEXTURE0),offsetInShader:位于着色器中第几个，name:纹理在着色器中的名字
        void bindTexture(int pro, int textureHandle, ETextureType type, ETextureUnit index, int offsetInShader, const char* texureNameInShader) override;
        void bindTexture(ETextureType type ,int textureHandle) override;
        void bindTexture(ETextureType type, int pro, const char* texureNameInShadadr, int textureHandle) override;

        void unBindTexture(int pro, int textureHandle, ETextureType type, ETextureUnit index, int offsetInShader, const char* texureNameInShader) override;
        void deleteTexture(int textureHandle) override;

        //internal
        void setupLayout(int shaderLayoutId, int memberNum, unsigned int memberBasicType, bool normalize, int diff, int offset) override;
        //internal
        void setupLayoutI(int shaderLayoutId, int memberNum, unsigned int memberBasicType, int diff, int offset) override;

        //belongs to applyForBuffer
        unsigned int genBufferData(void* data, long long byteLen, EBufferType bufTypeEnum, EDrawMode drawEnum) override;

        void deleteBufferData(unsigned int h) override;

        //belongs to updateBuffer
        void bufferSubData(EBufferType targe, int offset, int siz, const void* data) override;

        void bindBufferBase(EBufferType bufTypeEnum, unsigned int index, unsigned int hBuf) override;
        //传递数据到shader_storage buffer
        int setupShaderStorageBuffer(void* data, long long byteLen, unsigned int drawEnum) override;

        //生成storage纹理
        unsigned int genStorageTexture(ETextureType target, int levels, ETextureUnit numTexture, EPixelFormat rgbaFormat, int width, int height) override;
        unsigned int genTexture(ETextureType target, int levels, ETextureType numTexture, EPixelFormat rgbaFormat, int width, int height) override;

        //
        void clearColor(float r,float g,float b,float a) override;
        void clear(EBitField field) override;
        //GraphicsPipelineBuilder::setVertexState
        void setViewport(int x,int y,int width, int height) override;
        void display(int width, int height) override;
        void flush() override;

        //GraphicsPipelineBuilder::setVertexLayout
        void setVertexBufferLayout(const VertexBufferLayout& layout) override;

        //belongs to GraphicsPipelineBuilder
        void setPolygonMode(ECullMode cullMode ,EFillMode fillMode) override;

        /*********************************2级接口***************************/
        VboEboHandle applyForVboEbo(const VboEboCreateInfo& createInfo) override;

        void bindVboEboToVao(VboEboHandle handle, unsigned int vao, const VertexBufferLayout& layout) override;

        //belongs to apply for buffer
        void applyGpuVertexData(GpuVertexDataHandle& handle, const VertexBufferLayout& layout,
            void* vertice, int verticeCount, int verticeSize, void* indice, int indexCount, int indexSize, EDrawMode usage) override;
        void applyGpuVertexDataArray(GpuVertexDataHandle& handle, const VertexDataArray& vertexData) override;

        //belongs to GraphicsPipelineBuilder::setShaderProgram
        ShaderProgram createShaderProgram(const ProgramCreateInfo& info) override;
        //belongs to drawCall
        virtual void drawCall(const GpuVertexDataHandle& handle) override;

    protected:
        
        int createShaderBySrc(const char* src , EShaderType type);
        int createShaderByPath(const char* path , EShaderType type);

    };

    extern void* GetProcAddress(const char* name);
}


#endif