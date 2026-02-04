/*****************************************************************************
* @brief : Program
* 跨平台着色器程序类，封装一层接口统一Vulkan,opengl,以及可能
* 的d3d,matel着色器程序接口，后续可能修改
* @author : acedtang
* @date : 2021/4/14
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __SHADER_H
#define __SHADER_H

#include <string>

namespace Sun {

    class Program
    {
    public:
        Program();
        Program(const char* vertexPath, const char* fragmentPath);
        virtual void init() = 0;
        virtual bool compile() = 0;
        
        virtual unsigned int getProgramID() = 0;

        bool readVertexShader(const char* filepath);
        bool readFragmentShader(const char* filepath);
    protected:
        std::string vertexShaderPath_;
        std::string fragmentShaderPath_;
        std::string rayTracingComputePath_;

        //着色器字符串
        std::string vertexShader_;
        std::string fragmentShader_;
        std::string rayTracingComputeShader_;

        //句柄
        unsigned int vertexShaderID_;
        unsigned int fragmentShaderID_;
        unsigned int rayTracingComputeShaderID_;
        unsigned int programID_ = 0;

        bool isReady_ = false;
    };

}



#endif





