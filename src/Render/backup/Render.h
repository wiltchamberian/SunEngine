/**
 * @projectName   Sun
 * @brief         the render
 * @author        acedtang
 * @date          2021/01/21
 */

#ifndef __RENDER_H
#define __RENDER_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Math/Matrix4x4.h"
#include "Render/RenderHandle.h"

namespace Sun
{
    class Shader;
    struct DataHandle
    {
        //from opengl ,may change!
            int vao;
        //    int vbo;
        //    int ebo;
        std::shared_ptr<VertexBuffer> vbo;
        std::shared_ptr<IndexBuffer>  ebo;
    };

    class SRender
    {
    public:
        //加载渲染库dll函数以及着色器,初始化窗口环境等
        //void init(void* hwnd);
        //内部根据宏定义区分平台
        virtual void bindWindow(void* hwnd) = 0;
        //加载渲染库
        virtual void loadRenderLib() = 0;

        virtual void useProgram(unsigned int shaderId) = 0;
        virtual void displayHandle(const RenderHandle& handle) = 0;
        virtual unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false) = 0;
        virtual void setModelMatrix(int shader, const Matrix4x4& mat) = 0;
        virtual void setViewMatrix(int shader, const Matrix4x4& mat) = 0;
        virtual void setProjectionMatrix(int shader, const Matrix4x4& mat) = 0;
        //4*4矩阵，列主元，
        virtual void setMatrix(int shader, const float* mat, int num, const char*) = 0;

        virtual int loadTexture(const std::string& path) = 0;
        virtual int loadCubemap(const std::vector<std::string>& faces) = 0;
        
        virtual void clear(const Vector4D& rgba) = 0;
        virtual void setViewport(int width, int height) = 0;
        virtual void display(int width, int height) = 0;
        virtual void flush() = 0;

        //第一个id是表示纹理句柄，type表示纹理的类型(gl:GL_TEXTURE_2D),
        // index表示纹理索引(gl:GL_TEXTURE0),offsetInShader:位于着色器中第几个，name:纹理在着色器中的名字
        virtual void bindTexture(int textureHandle,int type,int index,int offsetInShader, const char* texureNameInShader) = 0;

        //传递data和index数据到gpu,并返回句柄(gl:vao),调用前需要切换到对应 着色器程序(use Program)
        //virtual std::shared_ptr<VertexArray> setupDatas(void* vertice, long long verticLen, void* indice, long long  indiceSiz) = 0;

        //virtual int setupBufferData(void* data, long long byteLen, unsigned int bufTypeEnum, unsigned int drawEnum) = 0;
        virtual void bindBufferBase(unsigned int bufTypeEnum, unsigned int index, unsigned int hBuf) = 0;
        //传递数据到shader_storage buffer
        virtual int setupShaderStorageBuffer(void* data, long long byteLen, unsigned int drawEnum) = 0;

        //设置gpu内存布局（如何解析数据?)     
        //参数1:对应着色器中的值，参数2:字段个数，参数3:字段类型，参数4:表示是否归一化（无符号映射为
        //[0,1]有符号映射为[-1,1]，参数5:表示该属性相邻两次出现时的偏移间隔字节数,参数6:表示当前属性在属性组中的起始偏移量;
        virtual void setupLayout(int shaderLayoutId, int memberNum, unsigned int memberBasicType, bool normalize, int diff, int offset) = 0;
        virtual void setupLayoutI(int shaderLayoutId, int memberNum, unsigned int memberBasicType, int diff, int offset) = 0;
    };

    extern SRender* g_render;
}


#endif