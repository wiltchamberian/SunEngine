/*****************************************************************************
* @brief : GLProgramFactory
* 实例化ProgramFactory
* @author : acedtang
* @date : 2021/5/6
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __GL_PROGRAM_FACTORY_H
#define __GL_PROGRAM_FACTORY_H

#include "ProgramFactory.h"

namespace Sun {

    class GLProgramFactory : public ProgramFactory {
    public:
        virtual int createProgram(int shader) override;
        virtual int createProgram(const std::vector<int>& shaders) override;
    };

    /*
    //设置矩阵 （初始化，重要,所有uniform对象最好都初始化!)
		GLint modelLoc = GLFunctions::glGetUniformLocation(programID_, "model");
		if (modelLoc != -1) {
			Matrix4x4 model;
			GLFunctions::glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)(&model));
		}
		
		GLint viewLoc = GLFunctions::glGetUniformLocation(programID_, "view");
		if (viewLoc != -1) {
			Matrix4x4 view;
			GLFunctions::glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)(&view));
		}
		
		GLint proLoc = GLFunctions::glGetUniformLocation(programID_, "projection");
		if (proLoc != -1) {
			Matrix4x4 pro;
			GLFunctions::glUniformMatrix4fv(proLoc, 1, GL_FALSE, (float*)(&pro));
		}
		
		GLint boneLoc = GLFunctions::glGetUniformLocation(programID_, "gBones");
		if (boneLoc != -1) {
			std::vector<Matrix4x4> mats(100); //FIXME
			GLFunctions::glUniformMatrix4fv(boneLoc, 100, GL_FALSE, (float*)(mats.data()));
		}
    
    */
}


#endif