#include "GLGraphicsLib.h"
#include <fstream>
#include <sstream>
#include <functional>
#include "Primitive/VertexBufferLayout.h"
#include "Render/GLShader.h"
#include "Render/GLVertexArray.h"
#include "Render/GLVertexBuffer.h"
#include "Render/GLIndexBuffer.h"
#include "Render/GLFunctions.h"
#include "Render/GLProgram.h"
#include "Render/GLEnumMap.h"
#include "Util/exception.h"
#include "PCH.h"
#ifdef _WIN32
#include "windows.h"
#endif

#include "ThirdParty/glfw/glfw3.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "ThirdParty/stb_image.h"

namespace Sun {

    using uint = unsigned int;

    void GLGraphicsLib::initEnvironment() {

    }

    ShaderProgram GLGraphicsLib::createProgram(const Shader& shader) {
        ShaderProgram program;

        const GLubyte* pVersion = glGetString(GL_VERSION);
        const GLubyte* pGlslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

        program.id_ = glCreateProgram();
        glAttachShader(program.id_, shader.getId());
        glLinkProgram(program.id_);
        int success = 0;
        char infoLog[512];
        glGetProgramiv(program.id_, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program.id_, 512, NULL, infoLog);
            ASSERT(false);
        }
        glUseProgram(program.id_);

        return program;
    }

    ShaderProgram GLGraphicsLib::createProgram(const std::vector<Shader>& vec) {
        ShaderProgram program;
        
        const GLubyte* pVersion = glGetString(GL_VERSION);
        const GLubyte* pGlslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

        program.id_ = glCreateProgram();
        for (int i = 0; i < vec.size(); ++i) {
            glAttachShader(program.id_, vec[i].getId());
        }

        glLinkProgram(program.id_);
        int success = 0;
        char infoLog[512];
        glGetProgramiv(program.id_, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program.id_, 512, NULL, infoLog);
            ASSERT(false);
        }
        glUseProgram(program.id_);

        return program;
    }

    void GLGraphicsLib::deleteProgram(const ShaderProgram& pro) {
        glDeleteProgram(pro.id_);
    }

    Shader GLGraphicsLib::createShader(const char* str, EShaderType type, bool pathOrSrc) {
        Shader shader;
        shader.type_ = type;
        if (pathOrSrc) {
            shader.id_ = createShaderByPath(str , type);
        }
        else {
            shader.id_ = createShaderBySrc(str, type);
        }
        return shader;
    }

    void GLGraphicsLib::deleteShader(const Shader& shader) {
        glDeleteShader(shader.id_);
    }

    unsigned int GLGraphicsLib::genVertexArray() {
        unsigned int id;
        glGenVertexArrays(1, &id);
        return id;
    }

    void GLGraphicsLib::deleteVertexArray(unsigned int id) {
        if(id != INVALID_HANDLE)
            glDeleteVertexArrays(1, &id);
    }

    void GLGraphicsLib::bindVertexArray(unsigned int id) {
        glBindVertexArray(id);
    }

    void GLGraphicsLib::bindVertexBuffer(unsigned int id) {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    unsigned int GLGraphicsLib::genVertexBuffer(const void* data, long long byteSize, EDrawMode drawType) {
        unsigned int usage = gl_enum_map(drawType);
        unsigned int id;
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, byteSize, data, usage);
        return id;
    }

    unsigned int GLGraphicsLib::genIndexBuffer(const void* data, long long byteSize, EDrawMode drawType) {
        unsigned int usage = gl_enum_map(drawType);
        unsigned int id;
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, byteSize, data, usage);
        return id;
    }

    void GLGraphicsLib::bindBuffer(unsigned int target, unsigned int buffer) {
        glBindBuffer(target, buffer);
    }

    void GLGraphicsLib::bindWindow(void* hwnd) {
#if 1
#ifdef _WIN32
        HWND hWnd = HWND(hwnd);

        //获取HDC，并随意设置一个空像素格式
        HGLRC       hRC;
        HDC hDC = GetDC(hWnd);
        PIXELFORMATDESCRIPTOR pfd;

        typedef bool (WINAPI* WGLChoosePixelFormatARB) (HDC, const int*, const float*, unsigned int, int*, unsigned int*);

        WGLChoosePixelFormatARB wglChoosePixelFormatARB = (WGLChoosePixelFormatARB)wglGetProcAddress("wglChoosePixelFormatARB");

        ////像素格式
        //PIXELFORMATDESCRIPTOR pfd = //定义像素格式
        //{
        //    sizeof(PIXELFORMATDESCRIPTOR), // 上述格式描述符的大小
        //    1,         // 版本号
        //    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        //    PFD_TYPE_RGBA,      // 申请 RGBA 格式
        //    32,
        //    0, 0, 0, 0, 0, 0,     // 忽略的色彩位
        //    8,         // 无Alpha缓存
        //    0,         // 忽略Shift Bit
        //    0,         // 无累加缓存
        //    0, 0, 0, 0,       // 忽略聚集位
        //    32,         // 32位 Z-缓存 (深度缓存)
        //    8,         // 无蒙板缓存
        //    0,         // 无辅助缓存
        //    PFD_MAIN_PLANE,      // 主绘图层
        //    0,         // Reserved
        //    0, 0, 0        // 忽略层遮罩
        //};
        //int nIndex = ChoosePixelFormat(hDC, &pfd); //选择刚刚定义的像素格式
        //if (nIndex == 0)
        //{
        //    return;
        //}


        SetPixelFormat(hDC, 1, &pfd);

        ////multisample
        //UINT Multisample = 4; //4倍多重采样率
        //int pixelformat;
        //UINT numformat;
        //float   fAttributes[] = { 0, 0 };
        //int iAtributes[] =
        //{
        //    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        //    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        //    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        //    WGL_COLOR_BITS_ARB, 24,
        //    WGL_ALPHA_BITS_ARB, 8,
        //    WGL_DEPTH_BITS_ARB, 32,
        //    WGL_STENCIL_BITS_ARB, 8,
        //    WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        //    WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
        //    WGL_SAMPLES_ARB, Multisample,
        //    0, 0
        //};
        ////----------取得可用的多重采样像素格式
        //if (!wglChoosePixelFormatARB(hDC, iAtributes, fAttributes, 1, &pixelformat, &numformat))
        //{
        //    MessageBox(L"找不到可用的抗锯齿格式");
        //}

        hRC = wglCreateContext(hDC);

        wglMakeCurrent(hDC, hRC);
#endif
#ifdef _LINUX

#endif

#endif
    }

    void GLGraphicsLib::load() {
        //if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        //    assert(false);
        //}
        //因为这里不想依赖glfw，因此用自定义的，不过暂时只支持Windows，后续可以扩展
        if (!gladLoadGLLoader(GetProcAddress)) {
            assert(false);
        }
        glEnable(GL_CULL_FACE);
#if 0
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
        GLFunctions::glUniformMatrix3fv = (PFN_glUniformMatrix3fv)GetProcAddress("glUniformMatrix3fv");
        GLFunctions::glUniform1iv = (PFN_glUniform1iv)GetProcAddress("glUniform1iv");
        GLFunctions::glUniform1i = (PFN_glUniform1i)GetProcAddress("glUniform1i");
        GLFunctions::glUniform3f = (PFN_glUniform3f)GetProcAddress("glUniform3f");
        GLFunctions::glUniform1f = (PFN_glUniform1f)GetProcAddress("glUniform1f");
        GLFunctions::glUniform2f = (PFN_glUniform2f)GetProcAddress("glUniform2f");
        GLFunctions::glUniform2fv = (PFN_glUniform2fv)GetProcAddress("glUniform2fv");
        GLFunctions::glUniform3fv = (PFN_glUniform3fv)GetProcAddress("glUniform3fv");
        GLFunctions::glUniform4fv = (PFN_glUniform4fv)GetProcAddress("glUniform4fv");
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
        GLFunctions::glVertexAttribDivisor = (PFN_glVertexAttribDivisor)GetProcAddress("glVertexAttribDivisor");
        GLFunctions::glEnableVertexAttribArray = (PFN_glEnableVertexAttribArray)GetProcAddress("glEnableVertexAttribArray");
        GLFunctions::glActiveTexture = (PFN_glActiveTexture)GetProcAddress("glActiveTexture");
        GLFunctions::glBindTexture = (PFN_glBindTexture)GetProcAddress("glBindTexture");
        GLFunctions::glTexBuffer = (PFN_glTexBuffer)GetProcAddress("glTexBuffer");
        GLFunctions::glGenTextures = (PFN_glGenTextures)GetProcAddress("glGenTextures");
        GLFunctions::glDeleteTextures = (PFN_glDeleteTextures)GetProcAddress("glDeleteTextures");
        GLFunctions::glTexImage2D = (PFN_glTexImage2D)GetProcAddress("glTexImage2D");
        GLFunctions::glTexImage3D = (PFN_glTexImage3D)GetProcAddress("glTexImage3D");
        GLFunctions::glTexStorage2D = (PFN_glTexStorage2D)GetProcAddress("glTexStorage2D");
        GLFunctions::glTexParameterf = (PFN_glTexParameterf)GetProcAddress("glTexParameterf");
        GLFunctions::glBindImageTexture = (PFN_glBindImageTexture)GetProcAddress("glBindImageTexture");
        GLFunctions::glGenerateMipmap = (PFN_glGenerateMipmap)GetProcAddress("glGenerateMipmap");
        GLFunctions::glTexParameteri = (PFN_glTexParameteri)GetProcAddress("glTexParameteri");
        GLFunctions::glDrawElements = (PFN_glDrawElements)GetProcAddress("glDrawElements");
        GLFunctions::glDrawElementsInstanced = (PFN_glDrawElementsInstanced)GetProcAddress("glDrawElementsInstanced");
        GLFunctions::glDrawArrays = (PFN_glDrawArrays)GetProcAddress("glDrawArrays");
        GLFunctions::glDrawArraysInstanced = (PFN_glDrawArraysInstanced)GetProcAddress("glDrawArraysInstanced");
        GLFunctions::glViewport = (PFN_glViewport)GetProcAddress("glViewport");
        GLFunctions::glClearColor = (PFN_glClearColor)GetProcAddress("glClearColor");
        GLFunctions::glClear = (PFN_glClear)GetProcAddress("glClear");
        GLFunctions::glFlush = (PFN_glFlush)GetProcAddress("glFlush");
        GLFunctions::glGetError = (PFN_glGetError)GetProcAddress("glGetError");
        GLFunctions::glEnable = (PFN_glEnable)GetProcAddress("glEnable");
        GLFunctions::glDisable = (PFN_glDisable)GetProcAddress("glDisable");
        GLFunctions::glBlendFunc = (PFN_glBlendFunc)GetProcAddress("glBlendFunc");
        GLFunctions::glPointSize = (PFN_glPointSize)GetProcAddress("glPointSize");
#endif
    }

    void GLGraphicsLib::useProgram(const ShaderProgram& pro) {
        glUseProgram(pro.id_);
    }

    unsigned int GLGraphicsLib::readTexture(const char* path, const std::string& directory, bool gamma) {
#if 0
        std::string filename = std::string(path);
        filename = directory + '/' + filename;

        unsigned int textureID;
        glGenTextures(1, &textureID);

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

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            //std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
#endif
        return 0;
    }

    void GLGraphicsLib::drawArray(EPrimitiveType mode, int first, int count) {
        unsigned int glMode = gl_enum_map(mode);
        glDrawArrays(glMode, first, count);
    }

    void GLGraphicsLib::drawArrayInstanced(EPrimitiveType mode, int first, int count, int instanceCount) {
        unsigned int glMode = gl_enum_map(mode);
        glDrawArraysInstanced(glMode, first, count,instanceCount);
    }

    void GLGraphicsLib::drawElements(EPrimitiveType mode, int byteSiz, EBasicDataType type, const void* indices) {
        unsigned int glMode = gl_enum_map(mode);
        unsigned int glType = gl_enum_map(type);
        glDrawElements(glMode, byteSiz, glType/*GL_UNSIGNED_INT*/, indices);
    }

    void GLGraphicsLib::drawElementsInstanced(EPrimitiveType mode, int byteSiz, EBasicDataType type, const void* indices,int instanceCount) {
        unsigned int glMode = gl_enum_map(mode);
        unsigned int glType = gl_enum_map(type);
        glDrawElementsInstanced(glMode, byteSiz, glType/*GL_UNSIGNED_INT*/, indices, instanceCount);
    }

    void GLGraphicsLib::setBool(unsigned int pro, const std::string& name, bool value) {
        GLint loc = glGetUniformLocation(pro, name.c_str());
        if (loc < 0) {
            ASSERT(false);
            return;
        }
        glUniform1i(loc, (int)value);
    }

    void GLGraphicsLib::setInt(unsigned int pro, const std::string& name, int value) {
        GLint loc = glGetUniformLocation(pro, name.c_str());
        if (loc < 0) {
            //ASSERT(false);
            return;
        }
        glUniform1i(loc, value);
    }

    void GLGraphicsLib::setFloat(unsigned int pro, const std::string& name, float value) {
        GLint loc = glGetUniformLocation(pro, name.c_str());
        if (loc < 0) {
            //ASSERT(false);
            return;
        }
        glUniform1f(loc, value);
    }

    void GLGraphicsLib::setVec2(unsigned int pro, const std::string& name, const vec2& value) {
        GLint loc = glGetUniformLocation(pro, name.c_str());
        if (loc < 0) {
            ASSERT(false);
            return;
        }
        glUniform2fv(loc, 1, &value.x);
    }

    void GLGraphicsLib::setVec2(unsigned int pro, const std::string& name, float x, float y) {
        GLint loc = glGetUniformLocation(pro, name.c_str());
        if (loc < 0) {
            ASSERT(false);
            return;
        }
        glUniform2f(loc, x, y);
    }

    void GLGraphicsLib::setVec3(unsigned int pro, const std::string& name, const vec3& value) {
        GLint loc = glGetUniformLocation(pro, name.c_str());
        if (loc < 0) {
            //ASSERT(false);
            return;
        }
        glUniform3fv(loc, 1, &value.x);
    }

    void GLGraphicsLib::setVec3(unsigned int pro, const std::string& name, float x, float y, float z) {
        GLint loc = glGetUniformLocation(pro, name.c_str());
        if (loc < 0) {
            ASSERT(false);
            return;
        }
        glUniform3f(loc, x, y, z);
    }

    void GLGraphicsLib::setVec4(unsigned int pro, const std::string& name, const vec4& value) {
        GLint loc = glGetUniformLocation(pro, name.c_str());
        if (loc < 0) {
            printf("glsl uniform %s not used\n", name.c_str());
            return;
        }
        glUniform4fv(loc, 1, &value.x);
    }


    //void setMat2(const std::string& name, const Matrix2x2& mat) override;
    void GLGraphicsLib::setMat3(unsigned int pro, const char* name, const float* mat, int count) {
        GLint loc = glGetUniformLocation(pro, name);
        if (loc < 0) {
            ASSERT(false);
            return;
        }
        glUniformMatrix3fv(loc, count, GL_TRUE, mat);
    }
    void GLGraphicsLib::setMat4(unsigned int pro, const char* name, const float* mat, int count) {
        GLint loc = glGetUniformLocation(pro, name);
        if (loc < 0) {
            //ASSERT(false);
            //printf("glsl uniform %s not used\n", name);
            return;
        }
        glUniformMatrix4fv(loc, count, GL_TRUE, mat);
    }

    void GLGraphicsLib::enable(ECapability cap) {
        glEnable(gl_enum_map(cap));
    }

    void GLGraphicsLib::disable(ECapability cap) {
        glDisable(gl_enum_map(cap));
    }

    void GLGraphicsLib::genFrameBuffers(int size, unsigned int* ids) {
        glGenFramebuffers(size, ids);
    }

    void GLGraphicsLib::bindFrameBuffer(unsigned int frambuffer) {
        glBindFramebuffer(GL_FRAMEBUFFER, frambuffer);
    }

    void GLGraphicsLib::framebufferTexture2D(const FrameBufferTexture2D& input) {
        GLenum ftarget = gl_enum_map(input.target);
        GLenum attach = gl_enum_map(input.attachment);
        GLenum texTarget = gl_enum_map(input.textarget);
        glFramebufferTexture2D(ftarget, attach, texTarget, input.hTex, input.level);
    }

    void GLGraphicsLib::setDrawBuffer(EDrawBuffer drawBuffer) {

    }

    void GLGraphicsLib::setReadBuffer(EDrawBuffer drawBuffer) {

    }

    void GLGraphicsLib::drawBuffers(int n, const unsigned int* bufs) {
        glDrawBuffers(n, bufs);
    }

    void GLGraphicsLib::genRenderbuffers(int n, unsigned int* renderbuffers) {
        glGenRenderbuffers(n, renderbuffers);
    }

    void GLGraphicsLib::bindRenderbuffer(unsigned int renderbuffer) {
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    }

    void GLGraphicsLib::renderbufferStorage(EPixelFormat internalformat, int width, int height) {
        GLenum interalFormatEnum = gl_enum_map(internalformat);
        glRenderbufferStorage(GL_RENDERBUFFER, interalFormatEnum, width, height);
    }

    void GLGraphicsLib::framebufferRenderbuffer(EFrameBufferType type, EAttachmentType attachment, unsigned int renderbuffer) {
        GLenum target = gl_enum_map(type);
        GLenum attch = gl_enum_map(attachment);
        glFramebufferRenderbuffer(target, attch, GL_RENDERBUFFER, renderbuffer);
    }

    EFrameBufferState GLGraphicsLib::checkFramebufferStatus(EFrameBufferType type) {
        unsigned int map = gl_enum_map(type);
        unsigned int ret =  glCheckFramebufferStatus(map);
        return gl_enum_map(ret);
    }

    int GLGraphicsLib::loadTexture(const std::string& path) {
#if 0
        int width, height, nrChannels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
        return texture;
#endif 
        assert(false);
        return 0;
    }

    unsigned int GLGraphicsLib::genTexture() {
        unsigned int texture;
        glGenTextures(1, &texture);
        return texture;
    }

    int GLGraphicsLib::texImage2D(const TexImage2D& input) {
        glBindTexture(GL_TEXTURE_2D, input.handle);

        GLenum inputFormat = gl_enum_map(input.inputPixelFormat);
        GLint internalFormat = gl_enum_map(input.internalPixelFormat);
        GLenum dataType = gl_enum_map(input.pixelDataType);
        GLint wrap = gl_enum_map(input.wrapperMode);
        GLint filter = gl_enum_map(input.filter);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, input.width, input.height, 0, inputFormat, dataType, input.data);
        return 0;
    }

    int GLGraphicsLib::loadCubemap(const std::vector<std::string>& faces) {
#if 0
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

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
#endif 
        assert(false);
        return 0;
    }

    //第一个id是表示纹理句柄，type表示纹理的类型(gl:GL_TEXTURE_2D),
    // index表示纹理索引(gl:GL_TEXTURE0),offsetInShader:位于着色器中第几个，name:纹理在着色器中的名字
    void GLGraphicsLib::bindTexture(int pro, int textureHandle, ETextureType type, ETextureUnit index, int offsetInShader, const char* texureNameInShader) {
        int glType = gl_enum_map(type);
        int glIndex = gl_enum_map(index);
        int id = glGetUniformLocation(pro, texureNameInShader);
        glUniform1i(id , offsetInShader);
        glActiveTexture(glIndex);
        glBindTexture(glType, textureHandle);
    }

    void GLGraphicsLib::bindTexture(ETextureType type ,int textureHandle) {
        int glType = gl_enum_map(type);
        glBindTexture(glType, textureHandle);
    }

    void GLGraphicsLib::bindTexture(ETextureType type, int pro, const char* texureNameInShadadr, int textureHandle) {
        int id = glGetUniformLocation(pro, texureNameInShadadr);
        GLint unit = 0;
        glGetUniformiv(pro, id, &unit);
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(gl_enum_map(type), textureHandle);
    }

    void GLGraphicsLib::unBindTexture(int pro, int textureHandle, ETextureType type, ETextureUnit index, int offsetInShader, const char* texureNameInShader) {
        if (type == ETextureType::INVALID || textureHandle == INVALID_HANDLE) return;

        int glType = gl_enum_map(type);
        int glIndex = gl_enum_map(index);
        int id = glGetUniformLocation(pro, texureNameInShader);
        glUniform1i(id, offsetInShader);
        glActiveTexture(glIndex);
        glBindTexture(glType, 0);  
    }

    void GLGraphicsLib::deleteTexture(int textureHandle) {
        unsigned int u = textureHandle;
        glDeleteTextures(1, &u);
    }

    void GLGraphicsLib::setupLayout(int shaderLayoutId, int memberNum, unsigned int memberBasicType, bool normalize, int diff, int offset) {
        glEnableVertexAttribArray(shaderLayoutId);
        glVertexAttribPointer(shaderLayoutId, memberNum, memberBasicType, normalize, diff, (void*)offset);
    }

    void GLGraphicsLib::setupLayoutI(int shaderLayoutId, int memberNum, unsigned int memberBasicType, int diff, int offset) {
        glEnableVertexAttribArray(shaderLayoutId);
        glVertexAttribIPointer(shaderLayoutId, memberNum, memberBasicType, diff, (void*)offset);
    }

    unsigned int GLGraphicsLib::genBufferData(void* data, long long byteLen, EBufferType bufTypeEnum, EDrawMode drawEnum) {
        unsigned int h = 0;
        glGenBuffers(1, &h);
        glBindBuffer(gl_enum_map(bufTypeEnum), h);
        glBufferData(gl_enum_map(bufTypeEnum), byteLen, data, gl_enum_map(drawEnum));
        return h;
    }

    void GLGraphicsLib::deleteBufferData(unsigned int h) {
        glDeleteBuffers(1, &h);
    }

    void GLGraphicsLib::bufferSubData(EBufferType targe, int offset, int siz, const void* data) {
        GLenum target = gl_enum_map(targe);
        glBufferSubData(target, offset, siz, data);
    }

    void GLGraphicsLib::bindBufferBase(EBufferType bufTypeEnum, unsigned int index, unsigned int hBuf) {
        glBindBufferBase(gl_enum_map(bufTypeEnum), index, hBuf);
    }

    //传递数据到shader_storage buffer
    int GLGraphicsLib::setupShaderStorageBuffer(void* data, long long byteLen, unsigned int drawEnum) {
        unsigned int h = 0;
        glGenBuffers(1, &h);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, h);
        glBufferData(GL_SHADER_STORAGE_BUFFER, byteLen, NULL, drawEnum);

        //GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT; //FIXME
        //glMapBufferRange(GL_SHADER_STORAGE_BUFFER,0，byteLen, bufMask);

        return h;
    }

    unsigned int GLGraphicsLib::genStorageTexture(ETextureType target, int levels, ETextureUnit numTexture, EPixelFormat rgbaFormat, int width, int height) {
        unsigned int handle = static_cast<unsigned int>(-1);
        glGenTextures(1, &handle);
        glActiveTexture(gl_enum_map(numTexture));
        glBindTexture(gl_enum_map(target), handle);
        glTexStorage2D(gl_enum_map(target), 1, gl_enum_map(rgbaFormat), width , height);
        return handle;
    }

    unsigned int GLGraphicsLib::genTexture(ETextureType target, int levels, ETextureType numTexture, EPixelFormat rgbaFormat, int width, int height) {
        //todo
        return -1;
    }

    void GLGraphicsLib::clearColor(float r, float g, float b, float a) {
        glClearColor(r, g, b, a);
    }

    void GLGraphicsLib::clear(EBitField field) {
        GLbitfield bitfield = gl_enum_map(field);
        glClear(bitfield);
    }

    void GLGraphicsLib::setViewport(int x,int y,int width, int height) {
        glViewport(x, y, width, height);
    }

    void GLGraphicsLib::display(int width, int height) {
    }

    void GLGraphicsLib::flush() {
        glFlush();
    }

    void GLGraphicsLib::setVertexBufferLayout(const VertexBufferLayout& layout) {
        for (int i = 0; i < layout.size(); ++i) {
            const VertexLayoutElement& ele = layout[i];
            glEnableVertexAttribArray(ele.layoutIndex);
            if (ele.type == EBasicDataType::FLOAT) {
                glVertexAttribPointer(ele.layoutIndex, ele.count, GL_FLOAT, ele.normalized, ele.stride, (void*)ele.offset);
            }
            else if (ele.type == EBasicDataType::INT) {
                glVertexAttribIPointer(ele.layoutIndex, ele.count, GL_INT, ele.stride, (void*)ele.offset);
            }
            else {
                assert(false);
            }
            //added for instance
            if (ele.attributeDivisor > 0) {
                glVertexAttribDivisor(ele.layoutIndex, ele.attributeDivisor);
            }
        }
    }

    void GLGraphicsLib::setPolygonMode(ECullMode cullMode ,EFillMode fillMode) {
        unsigned int glEnum =  gl_enum_map(cullMode);
        unsigned int glEnum2 = gl_enum_map(fillMode);
        glPolygonMode(glEnum, glEnum2);
    }

    VboEboHandle GLGraphicsLib::applyForVboEbo(const VboEboCreateInfo& ci) {
        VboEboHandle handle;
        long long verticeByteLen = (long long)ci.verticeCount * ci.verticeSiz;
        long long indiceByteLen = (long long)ci.indiceCount * ci.indiceCount;
        handle.vbo = genVertexBuffer(ci.vertices, verticeByteLen, ci.drawMode);
        if (ci.indices != nullptr && indiceByteLen > 0) {
            handle.ebo = genIndexBuffer(ci.indices, indiceByteLen, ci.drawMode);
        }
        return handle;
    }

    void GLGraphicsLib::bindVboEboToVao(VboEboHandle handle, unsigned int vao,const VertexBufferLayout& layout) {
        bindVertexArray(vao);
        if (handle.vbo != INVALID_HANDLE) {
            glBindBuffer(GL_ARRAY_BUFFER, handle.vbo);
        }
        if (handle.ebo != INVALID_HANDLE) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle.ebo);
        } 
        setVertexBufferLayout(layout);
    }

    void GLGraphicsLib::applyGpuVertexData(GpuVertexDataHandle& handle, const VertexBufferLayout& layout,
        void* vertice, int verticeCount, int verticeSize, void* indice, int indexCount, int indexSize, EDrawMode usage) {
        if (vertice == nullptr || verticeCount <= 0 ||verticeSize <= 0) return;

        long long verticeByteLen = (long long)verticeCount * verticeSize;
        long long indiceByteLen = (long long)indexCount * indexSize;

        handle.vao = genVertexArray();
        bindVertexArray(handle.vao);

        handle.vec.emplace_back();
        handle.vec.back().vbo = genVertexBuffer(vertice, verticeByteLen, usage);
        handle.vertexCount = verticeCount;

        if (indice != nullptr && indiceByteLen > 0) {
            handle.vec.back().ebo = genIndexBuffer(indice, indiceByteLen, usage);
            handle.indexCount = indexCount;
        }
        //for test FIXME
        assert(handle.vec.size() == 1);
        
        setVertexBufferLayout(layout);
        bindVertexArray(0);

        return;
    }

    void GLGraphicsLib::applyGpuVertexDataArray(GpuVertexDataHandle& handle, const VertexDataArray& vertexDataArray) {
        handle.vao = genVertexArray();
        bindVertexArray(handle.vao);

        handle.indexCount = vertexDataArray.indiceCount;
        handle.vertexCount = vertexDataArray.vertexCount;

        for (int i = 0; i < vertexDataArray.size(); ++i) {
            handle.vec.emplace_back();
            const VertexData& vd = vertexDataArray[i];
            long long verticeByteLen = (long long)vd.vertexCount * vd.vertexSiz;
            long long indiceByteLen = (long long)vd.indiceCount * vd.indiceSiz;

            handle.vec.back().vbo = genVertexBuffer(vd.vertice, verticeByteLen, vd.usage);
            //handle.vertexBufferByteSiz = vd.verticeByteLen;
            
            if (vd.indice != nullptr && indiceByteLen > 0) {
                handle.vec.back().ebo = genIndexBuffer(vd.indice, indiceByteLen, vd.usage);
                //handle.indexBufferByteSiz = indiceByteLen;
            }

            setVertexBufferLayout(vd.layout);
        }

        return;

    }

    ShaderProgram GLGraphicsLib::createShaderProgram(const ProgramCreateInfo& createInfo) {
        std::vector<Shader> shaders;
        for (int i = 0; i < createInfo.infos_.size(); ++i) {
            std::string path = createInfo.dir_ + createInfo.infos_[i].name;
            Shader shader = createShader(path.c_str(), createInfo.infos_[i].type, true);
            shaders.emplace_back(shader);
        }
        ShaderProgram program = createProgram(shaders);
        program.name_ = createInfo.shaderProgramName_;
        for (int i = 0; i < shaders.size(); ++i) {
            deleteShader(shaders[i]);
        }

        //初始化glsl全局变量
        for (auto& it : createInfo.uniformInfos_) {
            switch (it.type) {
            case UniformType::VEC3:
            {
                setVec3(program.getId(), it.name,std::get<vec3>(it.var));
            }
            break;
            case UniformType::MAT3:
            {
                std::vector<Matrix3x3> vec(it.num);
                for (int i = 0; i < vec.size(); ++i) {
                    vec[i] = std::get<Matrix3x3>(it.var);
                }
                setMat3(program.getId(), it.name.c_str(), (float*)vec.data(), it.num);
            }
            break;
            case UniformType::MAT4:
            {
                std::vector<Matrix4x4> vec(it.num);
                for (int i = 0; i < vec.size(); ++i) {
                    vec[i] = std::get<Matrix4x4>(it.var);
                }
                setMat4(program.getId(), it.name.c_str(), (float*)vec.data(), it.num);
            }
            break;
            case UniformType::SAMPLER2D:
            case UniformType::INT: 
            {
                setInt(program.getId(), it.name.c_str(), std::get<int>(it.var));
            }
            break;
            default:
            {
                assert(false);
            }
            break;
            }
        }
 
        return program;
    }

    void GLGraphicsLib::drawCall(const GpuVertexDataHandle& handle) {
        if (handle.indexCount == 0) {
            if (handle.numInstance <= 0) {
                drawArray(handle.mode, 0, handle.vertexCount);
            }
            else {
                drawArrayInstanced(handle.mode, 0 ,handle.vertexCount, handle.numInstance);
            }
        }
        else {
            if (handle.numInstance <= 0) {
                //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                drawElements(handle.mode, handle.indexCount,EBasicDataType::UNSIGNED_INT,(const void*)(0));
            }
            else {
                drawElementsInstanced(handle.mode,handle.indexCount, EBasicDataType::UNSIGNED_INT,(const void*)(0),handle.numInstance);
            }
        }
    }

    int GLGraphicsLib::createShaderBySrc(const char* src , EShaderType type) {
        int id = glCreateShader(gl_enum_map(type));
        glShaderSource(id, 1, &src, NULL);
        glCompileShader(id);
        int  success = 0;
        char infoLog[512];
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            printf("%s:%s\n", __FUNCTION__, infoLog);
            ASSERT(false);
        }
        return id;
    }

    int GLGraphicsLib::createShaderByPath(const char* path, EShaderType type) {
        std::fstream fs(path);
        if (!fs.is_open()) {
            ASSERT(false);
            fs.close();
            return -1;
        }

        std::stringstream buffer;
        buffer << fs.rdbuf();
        std::string src = buffer.str();

        int id = createShaderBySrc(src.c_str(), type);
        return id;
    }

    void* GetProcAddress(const char* name) {
        //这个函数和图形api关系不大，且依赖windows库，可能不要放在这
#if 1
#ifdef _WIN32
        void* p = (void*)wglGetProcAddress(name);
        if (p == 0 ||
            (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
            (p == (void*)-1))
        {
            HMODULE module = LoadLibraryA("opengl32.dll");
            p = (void*)::GetProcAddress(module, name);
        }

        return p;
#endif
#endif
        return 0;
    }
}