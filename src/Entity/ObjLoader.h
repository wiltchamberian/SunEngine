/*****************************************************************************
* @brief : obj文件加载器，支持加载和导出obj
* @author : acedtang
* @date : 2021/2/14
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/

#ifndef __METEOR_OBJ_LOADER_H
#define __METEOR_OBJ_LOADER_H

#include <string>
#include <vector>
#include "Entity/Obj.h"

namespace Sun
{
    class SObjLoader
    {
    public:
        void setDir(const std::string& dir);
        void setShaderDir(const std::string& dir);
        std::vector<GObj> loadObjs();
        void loadObj(const std::string& str, GObj& obj);
        static void exportObj(const std::string& str, const GObj& obj);
    public:
        std::vector<std::string> _vertice_shader_paths;
        std::vector<std::string> _fragment_shader_paths;
        std::string _dir;
        std::string _shaderDir;
    };
}


#endif

