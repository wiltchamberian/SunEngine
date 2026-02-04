#include "GLRasterRender.h"
#include <map>
#include <string>
#include <memory>
#include "GLFunctions.h"
#include "Util/Util.h"
#include "Util/exception.h"
#include "PCH.h"

//this thirdparty lib should be remove
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Sun
{
    void SGLRasterRender::bindWindow(void* hwnd) {
#ifdef _WIN32
        HWND hWnd = HWND(hwnd);

        //获取HDC，并随意设置一个空像素格式
        HGLRC       hRC;
        HDC hDC = GetDC(hWnd);
        PIXELFORMATDESCRIPTOR pfd;
        SetPixelFormat(hDC, 1, &pfd);

        hRC = wglCreateContext(hDC);

        wglMakeCurrent(hDC, hRC);
#endif
#ifdef _LINUX

#endif
    }

    void SGLRasterRender::loadRenderLib() {
        GLFunctions::glGetString = (PFN_glGetString)GetProcAddress("glGetString");
        GLFunctions::glCreateShader = (PFN_glCreateShader)GetProcAddress("glCreateShader");
        GLFunctions::glShaderSource = (PFN_glShaderSource)GetProcAddress("glShaderSource");
        GLFunctions::glCompileShader = (PFN_glCompileShader)GetProcAddress("glCompileShader");
        GLFunctions::glGetShaderiv = (PFN_glGetShaderiv)GetProcAddress("glGetShaderiv");
        GLFunctions::glGetIntegeri_v = (PFN_glGetIntegeri_v)GetProcAddress("glGetIntegeri_v");
        GLFunctions::glGetIntegerv = (PFN_glGetIntegerv)(GetProcAddress("glGetIntegerv"));
        GLFunctions::glGetShaderInfoLog = (PFN_glGetShaderInfoLog)GetProcAddress("glGetShaderInfoLog");
        GLFunctions::glCreateProgram = (PFN_glCreateProgram)GetProcAddress("glCreateProgram");
        GLFunctions::glAttachShader = (PFN_glAttachShader)GetProcAddress("glAttachShader");
        GLFunctions::glLinkProgram = (PFN_glLinkProgram)GetProcAddress("glLinkProgram");
        GLFunctions::glGetProgramiv = (PFN_glGetProgramiv)GetProcAddress("glGetProgramiv");
        GLFunctions::glGetProgramInfoLog = (PFN_glGetProgramInfoLog)GetProcAddress("glGetProgramInfoLog");
        GLFunctions::glDeleteShader = (PFN_glDeleteShader)GetProcAddress("glDeleteShader");
        GLFunctions::glUseProgram = (PFN_glUseProgram)GetProcAddress("glUseProgram");
        GLFunctions::glDispatchCompute = (PFN_glDispatchCompute)GetProcAddress("glDispatchCompute");
        GLFunctions::glMemoryBarrier = (PFN_glMemoryBarrier)GetProcAddress("glMemoryBarrier");
        GLFunctions::glGetUniformLocation = (PFN_glGetUniformLocation)GetProcAddress("glGetUniformLocation");
        GLFunctions::glUniformMatrix4fv = (PFN_glUniformMatrix4fv)GetProcAddress("glUniformMatrix4fv");
        GLFunctions::glUniform1iv = (PFN_glUniform1iv)GetProcAddress("glUniform1iv");
        GLFunctions::glUniform1i = (PFN_glUniform1i)GetProcAddress("glUniform1i");
        GLFunctions::glUniform3f = (PFN_glUniform3f)GetProcAddress("glUniform3f");
        GLFunctions::glUniform1f = (PFN_glUniform1f)GetProcAddress("glUniform1f");
        GLFunctions::glGetSubroutineIndex = (PFN_glGetSubroutineIndex)GetProcAddress("glGetSubroutineIndex");
        GLFunctions::glUniformSubroutinesuiv = (PFN_glUniformSubroutinesuiv)GetProcAddress("glUniformSubroutinesuiv");
        GLFunctions::glGenVertexArrays = (PFN_glGenVertexArrays)GetProcAddress("glGenVertexArrays");
        GLFunctions::glDeleteVertexArrays = (PFN_glDeleteVertexArrays)GetProcAddress("glDeleteVertexArrays");
        GLFunctions::glGenBuffers = (PFN_glGenBuffers)GetProcAddress("glGenBuffers");
        GLFunctions::glBindVertexArray = (PFN_glBindVertexArray)GetProcAddress("glBindVertexArray");
        GLFunctions::glBindBuffer = (PFN_glBindBuffer)GetProcAddress("glBindBuffer");
        GLFunctions::glBindBufferBase = (PFN_glBindBufferBase)GetProcAddress("glBindBufferBase");
        GLFunctions::glBufferData = (PFN_glBufferData)GetProcAddress("glBufferData");
        GLFunctions::glBufferSubData = (PFN_glBufferSubData)GetProcAddress("glBufferSubData");
        GLFunctions::glDeleteBuffers = (PFN_glDeleteBuffers)GetProcAddress("glDeleteBuffers");
        GLFunctions::glMapBufferRange = (PFN_glMapBufferRange)GetProcAddress("glMapBufferRange");
        GLFunctions::glUnmapBuffer = (PFN_glUnmapBuffer)GetProcAddress("glUnmapBuffer");
        GLFunctions::glVertexAttribPointer = (PFN_glVertexAttribPointer)GetProcAddress("glVertexAttribPointer");
        GLFunctions::glVertexAttribIPointer = (PFN_glVertexAttribIPointer)GetProcAddress("glVertexAttribIPointer");
        GLFunctions::glEnableVertexAttribArray = (PFN_glEnableVertexAttribArray)GetProcAddress("glEnableVertexAttribArray");
        GLFunctions::glActiveTexture = (PFN_glActiveTexture)GetProcAddress("glActiveTexture");
        GLFunctions::glBindTexture = (PFN_glBindTexture)GetProcAddress("glBindTexture");
        GLFunctions::glGenTextures = (PFN_glGenTextures)GetProcAddress("glGenTextures");
        GLFunctions::glTexImage2D = (PFN_glTexImage2D)GetProcAddress("glTexImage2D");
        GLFunctions::glTexImage3D = (PFN_glTexImage3D)GetProcAddress("glTexImage3D");
        GLFunctions::glTexStorage2D = (PFN_glTexStorage2D)GetProcAddress("glTexStorage2D");
        GLFunctions::glTexParameterf = (PFN_glTexParameterf)GetProcAddress("glTexParameterf");
        GLFunctions::glBindImageTexture = (PFN_glBindImageTexture)GetProcAddress("glBindImageTexture");
        GLFunctions::glGenerateMipmap = (PFN_glGenerateMipmap)GetProcAddress("glGenerateMipmap");
        GLFunctions::glTexParameteri = (PFN_glTexParameteri)GetProcAddress("glTexParameteri");
        GLFunctions::glDrawElements = (PFN_glDrawElements)GetProcAddress("glDrawElements");
        GLFunctions::glDrawArrays = (PFN_glDrawArrays)GetProcAddress("glDrawArrays");
        GLFunctions::glViewport = (PFN_glViewport)GetProcAddress("glViewport");
        GLFunctions::glClearColor = (PFN_glClearColor)GetProcAddress("glClearColor");
        GLFunctions::glClear = (PFN_glClear)GetProcAddress("glClear");
        GLFunctions::glFlush = (PFN_glFlush)GetProcAddress("glFlush");
        GLFunctions::glGetError = (PFN_glGetError)GetProcAddress("glGetError");
    }

    void SGLRasterRender::displayHandle(const RenderHandle& handle)
    {
        //GLFunctions::glUseProgram(handle.programId_);

        //the common texture;
        //GLFunctions::glActiveTexture(GL_TEXTURE0);
        //GLFunctions::glBindTexture(handle._textureType, handle._textureId);
  
        GLFunctions::glBindVertexArray(handle.vao);
        //if (_drawElements)
        {
            GLFunctions::glDrawElements(GL_TRIANGLES, handle._indiceSiz, GL_UNSIGNED_INT, 0);
        }
        //else
        //{
        //    glDepthMask(GL_FALSE);
        //    GLFunctions::glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
        //    glDepthMask(GL_TRUE);
        //}

        //GLFunctions::glBindTexture(GL_TEXTURE_2D, 0);
    }

    void SGLRasterRender::useProgram(unsigned int shaderId) {
        GLFunctions::glUseProgram(shaderId);
    }

    unsigned int SGLRasterRender::TextureFromFile(const char* path, const std::string& directory, bool gamma /*= false*/)
    {
        std::string filename = std::string(path);
        filename = directory + '/' + filename;

        unsigned int textureID;
        GLFunctions::glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            GLFunctions::glBindTexture(GL_TEXTURE_2D, textureID);
            GLFunctions::glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            GLFunctions::glGenerateMipmap(GL_TEXTURE_2D);

            GLFunctions::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            GLFunctions::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            GLFunctions::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            GLFunctions::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            //std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }

    void SGLRasterRender::setModelMatrix(int shader, const Matrix4x4& model)
    {
        GLint modelLoc = GLFunctions::glGetUniformLocation(shader, "model");
        GLFunctions::glUniformMatrix4fv(modelLoc, 1, GL_TRUE, (float*)(&model));
    }

    void SGLRasterRender::setViewMatrix(int shader, const Matrix4x4& mat)
    {
        GLint viewLoc = GLFunctions::glGetUniformLocation(shader, "view");
        GLFunctions::glUniformMatrix4fv(viewLoc, 1, GL_TRUE, (float*)(&mat));
    }

    void SGLRasterRender::setProjectionMatrix(int shader, const Matrix4x4& mat)
    {
        //设置投影矩阵
        GLint proLoc = GLFunctions::glGetUniformLocation(shader, "projection");
        GLFunctions::glUniformMatrix4fv(proLoc, 1, GL_TRUE, (float*)(&mat));
    }

    void SGLRasterRender::setMatrix(int shader, const float* mat, int num, const char* name) {
        GLint proLoc = GLFunctions::glGetUniformLocation(shader, name);
        if (proLoc >= 0) {
            GLFunctions::glUniformMatrix4fv(proLoc, num, GL_TRUE, mat);
        }
        else {
            ASSERT(false);
        }
  
    }

    GLint SGLRasterRender::loadTexture(const std::string& path)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        unsigned int texture;
        GLFunctions::glGenTextures(1, &texture);
        GLFunctions::glBindTexture(GL_TEXTURE_2D, texture);

        // set the texture wrapping parameters
        GLFunctions::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);	// set texture wrapping to GL_REPEAT (default wrapping method)
        GLFunctions::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        // set texture filtering parameters
        GLFunctions::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        GLFunctions::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        GLFunctions::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
        return texture;
    }

    GLint SGLRasterRender::loadCubemap(const std::vector<std::string>& faces)
    {
        unsigned int textureID;
        GLFunctions::glGenTextures(1, &textureID);
        GLFunctions::glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        GLFunctions::glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        GLFunctions::glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GLFunctions::glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        GLFunctions::glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        GLFunctions::glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
                GLFunctions::glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );

            stbi_image_free(data);
        }

        return textureID;
    }

    void SGLRasterRender::clear(const Vector4D& rgba)
    {
        GLFunctions::glClearColor(rgba.x, rgba.y, rgba.z, rgba.w);
        GLFunctions::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void SGLRasterRender::setViewport(int width, int height)
    {
        GLFunctions::glViewport(0, 0, width, height);
    }

    void SGLRasterRender::display(int width, int height)
    {
        
    }

    void SGLRasterRender::bindTexture(int id, int type, int index, int offsetInShader, const char* name) {
        GLFunctions::glUniform1i(GLFunctions::glGetUniformLocation(id, name), offsetInShader);
        GLFunctions::glActiveTexture(index);
        GLFunctions::glBindTexture(type, id);
    }

    void SGLRasterRender::flush()
    {
        GLFunctions::glFlush();
    }

#if 0
    std::shared_ptr<VertexArray> SGLRasterRender::setupDatas( void* vertice, long long verticByteLen, void* indice, long long  indiceByteSiz) {
        ASSERT(verticByteLen > 0 && indiceByteSiz > 0);
        if (verticByteLen < 0 || indiceByteSiz <= 0) return nullptr;

        std::shared_ptr<VertexArray> va;
        std::shared_ptr<VertexBuffer> vb = std::make_shared<VertexBuffer>(vertice, verticByteLen);
        std::shared_ptr<IndexBuffer> eb = std::make_shared<IndexBuffer>(indice, indiceByteSiz / sizeof(int));

        VertexBufferLayout layout;
        va->addBuffer(vb);

        return data;
    }
#endif

    int SGLRasterRender::setupBufferData(void* data, long long byteLen, unsigned int bufTypeEnum, unsigned int drawEnum) {
        unsigned int h = 0;
        GLFunctions::glGenBuffers(1, &h);
        GLFunctions::glBindBuffer(bufTypeEnum, h);
        GLFunctions::glBufferData(bufTypeEnum, byteLen, data, drawEnum);
        return h;
    }

    void SGLRasterRender::bindBufferBase(unsigned int bufTypeEnum, unsigned int index, unsigned int hBuf) {
        GLFunctions::glBindBufferBase(bufTypeEnum, index, hBuf);
    }

    int SGLRasterRender::setupShaderStorageBuffer(void* data, long long byteLen, unsigned int drawEnum) {
        unsigned int h = 0;
        GLFunctions::glGenBuffers(1, &h);
        GLFunctions::glBindBuffer(GL_SHADER_STORAGE_BUFFER, h);
        GLFunctions::glBufferData(GL_SHADER_STORAGE_BUFFER, byteLen, NULL, drawEnum);

        //GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT; //FIXME
        //GLFunctions::glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0，byteLen, bufMask);

        return h;
    }

    void SGLRasterRender::setupLayout(int shaderLayoutId, int memberNum, unsigned int memberBasicType,bool normalize, int diff, int offset) {
        GLFunctions::glEnableVertexAttribArray(shaderLayoutId);
        GLFunctions::glVertexAttribPointer(shaderLayoutId, memberNum, memberBasicType, normalize, diff, (void*)offset);
    }

    void SGLRasterRender::setupLayoutI(int shaderLayoutId, int memberNum, unsigned int memberBasicType, int diff, int offset) {
        GLFunctions::glEnableVertexAttribArray(shaderLayoutId);
        GLFunctions::glVertexAttribIPointer(shaderLayoutId, memberNum, memberBasicType, diff, (void*)offset);
    }

    void* SGLRasterRender::GetProcAddress(const char* name) {
        void* p = (void*)wglGetProcAddress(name);
        if (p == 0 ||
            (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
            (p == (void*)-1))
        {
            HMODULE module = LoadLibraryA("opengl32.dll");
            p = (void*)::GetProcAddress(module, name);
        }

        return p;
    }
}

