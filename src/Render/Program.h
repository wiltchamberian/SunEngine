#ifndef __PROGRAM_H
#define __PROGRAM_H

#include <memory>
#include <vector>
#include <string>

namespace Sun {

    class Shader;
    //对应RHI的IOpenglPipeline和VkDoom3的pipelineState_t
    class ShaderProgram {
    public:
        ShaderProgram();
        ShaderProgram(std::shared_ptr<Shader> shader);
        ShaderProgram(const std::vector<std::shared_ptr<Shader>>& vec);
        ~ShaderProgram();

        bool isValid() const {
            return id_ != -1;
        }

        __forceinline unsigned int getId() const {
            return id_;
        }
        friend bool operator <(const ShaderProgram& p1, const ShaderProgram& p2);
        //virtual unsigned int getId() const = 0;
    public:
        unsigned int id_ = -1;

        //just for debug
        std::string name_;
    };

    __forceinline bool operator < (const ShaderProgram& p1, const ShaderProgram& p2) {
        return p1.id_ < p2.id_;
    }

    inline bool operator > (const ShaderProgram& p1, const ShaderProgram& p2) {
        return p1.id_ > p2.id_;
    }
}

#endif