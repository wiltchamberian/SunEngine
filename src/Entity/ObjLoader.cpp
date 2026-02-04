#include "ObjLoader.h"
#include "Math/Vector3D.h"
#include "Math/Vector2D.h"
#include <fstream>
#include <string>
#include <sstream>
#include "Util/exception.h"
#include "Util/StringUtil.h"

namespace Sun
{
    void SObjLoader::setDir(const std::string& dir)
    {
        _dir = dir;
    }

    void SObjLoader::setShaderDir(const std::string& dir)
    {
        _shaderDir = dir;
    }

    std::vector<GObj> SObjLoader::loadObjs()
    {
        std::vector<GObj> vec;

        std::fstream fs(_dir + "/MeteorMap.txt", std::ios::in);
        if (fs.fail()) {
            ASSERT(false);
            return {};
        }

        std::string line;
        while (!fs.eof()) {
            std::getline(fs, line);
            line = Sun::removePrefixIn(line, "\t ");
            if (!line.compare(0, 1, "}"))
                break;
            if (!line.compare(1, 11, "shader_path")) {
                while (std::getline(fs, line)) {
                    line = Sun::removePrefixIn(line, "\t ");
                    if (!line.compare(0, 1, "[")) {
                        continue;
                    }
                    if (!line.compare(0, 1, "]")) {
                        break;
                    }
                    //remove the " ",

                    std::string tmp;
                    if (line.back() == ',')
                        tmp = line.substr(1, line.size() - 3);
                    else
                        tmp = line.substr(1, line.size() - 2);
                    _vertice_shader_paths.push_back(_shaderDir + "/" + tmp);
                    std::getline(fs, line);
                    line = Sun::removePrefixIn(line, "\t ");
                    if (line.back() == ',')
                        tmp = line.substr(1, line.size() - 3);
                    else
                        tmp = line.substr(1, line.size() - 2);
                    _fragment_shader_paths.push_back(_shaderDir + "/" + tmp);
                }
            }
            else if (!line.compare(1, 17, "objs_to_be_loaded")) {
                while (std::getline(fs, line)) {
                    line = Sun::removePrefixIn(line, "\t ");
                    if (!line.compare(0, 1, "{")) {
                        GObj obj;
                        //"name":" ",
                        std::getline(fs, line);
                        line = Sun::removePrefixIn(line, "\t ");
                        obj._name = line.substr(8, line.size() - 10);
                        //"shader_id":13,
                        std::getline(fs, line);
                        line = Sun::removePrefixIn(line, "\t ");
                        obj._shaderId = std::stoi(line.substr(12, line.size() - 13));
                        //"path": "abc",
                        std::getline(fs, line);
                        line = Sun::removePrefixIn(line, "\t ");
                        obj._path = _dir + "/" + line.substr(8, line.size() - 10);
                        //"texture_path": "abc",
                        std::getline(fs, line);
                        line = Sun::removePrefixIn(line, "\t ");
                        obj._texturePath = _dir + "/" + line.substr(16, line.size() - 18);
                        //"class:""abc",
                        std::getline(fs, line);
                        line = Sun::removePrefixIn(line, "\t ");
                        obj._className = line.substr(9, line.size() - 11);
                        //"isLoad":1
                        int load = 0;
                        std::getline(fs, line);
                        line = Sun::removePrefixIn(line, "\t ");
                        load = std::stoi(line.substr(9, 1));
                        if (load) {
                            vec.push_back(obj);
                        }
                        //"}"
                        std::getline(fs, line);
                        continue;
                    }
                    if (!line.compare(0, 1, "]")) {
                        break;
                    }
                }
            }
        }
        fs.close();

        for (int i = 0; i < vec.size(); ++i) {
            loadObj(vec[i]._path, vec[i]);
        }

        return std::move(vec);
    }

    void SObjLoader::loadObj(const std::string& str, GObj& obj)
    {
        std::fstream fs(str, std::ios::in);
        if (!fs.is_open()) return;
        std::string line;
        char trash;
        while (!fs.eof())
        {
            std::getline(fs, line);
            std::istringstream is(line);
            if (!line.compare(0, 2, "v ")) {
                is >> trash;
                vec3 pt;
                is >> pt.x >> pt.y >> pt.z;
                obj._vertices.push_back(pt);
            }
            else if (!line.compare(0, 3, "vn ")) {
                is >> trash >> trash;
                vec3 vn;
                is >> vn.x >> vn.y >> vn.z;
                obj._normals.push_back(vn);
            }
            else if (!line.compare(0, 3, "vt ")) {
                is >> trash >> trash;
                vec2 vt;
                is >> vt.x >> vt.y;
                obj._textures.push_back(vt);
            }
            else if (!line.compare(0, 5, "rgba ")) {
                is >> trash >> trash >> trash >> trash;
                vec4 color;
                is >> color.x >> color.y >> color.z >> color.w;
                obj._rgbas.push_back(color);
            }
            else if (!line.compare(0, 2, "f ")) {
                is >> trash;
                GFace face;
                std::vector<std::pair<int, int>> vec = Sun::splitString(line, 2, line.size(), ' ');
                for (int i = 0; i < vec.size(); ++i) {
                    std::vector<std::pair<int, int>> tmpVec = Sun::splitString(line, vec[i].first, vec[i].second, '/');
                    FaceMember member;
                    int* index = (int*)(&member);
                    for (int j = 0; j < tmpVec.size(); ++j) {
                        index[j] = std::stoi(line.substr(tmpVec[j].first, tmpVec[j].second - tmpVec[j].first)) - 1;
                    }
                    face._members.push_back(member);
                }
                obj._faces.push_back(face);
            }
        }
    }

    void SObjLoader::exportObj(const std::string& str, const GObj& obj)
    {
        std::fstream fs(str, std::ios::out | std::ios::ate);
        if (fs.fail()) return;

        fs << "#Sun\n";
        for (int i = 0; i < obj._vertices.size(); ++i) {
            fs << "v " << obj._vertices[i].x << " " << obj._vertices[i].y
                << " " << obj._vertices[i].z << "\n";
        }
        fs << "\n";
        for (int i = 0; i < obj._textures.size(); ++i) {
            fs << "vt " << obj._textures[i].x << " " << obj._textures[i].y << '\n';
        }
        fs << "\n";
        for (int i = 0; i < obj._normals.size(); ++i) {
            fs << "vn " << obj._normals[i].x << " " << obj._normals[i].y << " " << obj._normals[i].z << '\n';
        }
        fs << "\n";
        for (int i = 0; i < obj._rgbas.size(); ++i) {
            fs << "rgba " << obj._rgbas[i].x << " " << obj._rgbas[i].y << " " << obj._rgbas[i].z << '\n';
        }
        fs << "\n";

        //Ð´Èëface
        for (int i = 0; i < obj._faces.size(); ++i) {
            fs << "f ";
            for (int j = 0; j < obj._faces[i]._members.size(); ++j) {
                fs << obj._faces[i]._members[j].index + 1 << "/"
                    << obj._faces[i]._members[j].textureIndex + 1 << "/"
                    << obj._faces[i]._members[j].normalIndex + 1 << "/"
                    << obj._faces[i]._members[j].rgbaIndex + 1;
                if (j != obj._faces[i]._members.size())
                    fs << " ";
            }
            fs << "\n";
        }
    }
}

