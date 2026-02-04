/*****************************************************************************
* @brief : obj格式的内存表示，该格式方便导入导出
* 
* @author : acedtang
* @date : 2021/2/14
* @version : ver 1.0
* @inparam : 
* @outparam : 
*****************************************************************************/

#ifndef __OBJ_H
#define __OBJ_H

#include <vector>
#include <string>
#include "Math/Vector2D.h"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"

namespace Sun
{
    struct FaceMember
    {
        FaceMember() {
        }
        FaceMember(int a, int b, int c, int d) {
            index = a;
            textureIndex = b;
            normalIndex = c;
            rgbaIndex = d;
        }
        int index;
        int textureIndex;
        int normalIndex = -1;
        int rgbaIndex = -1;
    };

    class GFace
    {
    public:
        std::vector<FaceMember> _members;
    };

    class GObj
    {
    public:
        std::vector<GFace> _faces;
        std::vector<vec3> _vertices;
        std::vector<vec3> _normals;
        std::vector<vec2> _textures;
        //原则上不属于obj格式，但是我们可以在此基础上扩展自己的格式
        //使之向下兼容obj
        std::vector<vec4> _rgbas;
        //关联的纹理路径
        std::string _texturePath;
        //该obj的名字
        std::string _name;
        //shader_id
        int _shaderId = 0;
        //obj path
        std::string _path;
        //class 
        std::string _className;
    };
}


#endif