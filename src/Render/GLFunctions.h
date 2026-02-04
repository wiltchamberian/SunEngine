#ifndef __GLFUNCTIONS_H
#define __GLFUNCTIONS_H

#include <map>
#include "windows.h"
//#include "gl/gl.h"
//#include "GLMacros.h"
#include "ThirdParty/glfw/glad/glad.h"

//all the opengl interfce for user to use
typedef char GLchar;
//typedef ptrdiff_t GLsizeiptr;

#define API_FUNC __stdcall

typedef const GLubyte* (*PFN_glGetString)(GLenum name);
typedef GLenum (*PFN_glCreateShader)(GLenum type);

typedef GLuint (*PFN_glShaderSource)(GLuint shader, GLsizei count, const GLchar* const*, const GLint* length);
typedef GLuint (*PFN_glCompileShader)(GLuint shader);
typedef void (*PFN_glGetShaderiv)(GLuint shader, GLenum pname, GLint* params);
typedef void (*PFN_glGetIntegeri_v)(GLenum target,GLuint index,GLint* data);
typedef void (*PFN_glGetIntegerv)(GLenum pname,GLint* data);
typedef void (*PFN_glGetShaderInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*);

typedef GLuint(*PFN_glCreateProgram)();
typedef void (*PFN_glAttachShader)(GLuint, GLuint);
typedef void (*PFN_glLinkProgram)(GLuint);
typedef void (*PFN_glGetProgramiv)(GLuint, GLenum, GLint*);
typedef void (*PFN_glGetProgramInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*);
typedef void (*PFN_glDeleteShader)(GLuint);
typedef void (*PFN_glUseProgram)(GLuint);
typedef void (*PFN_glDispatchCompute)(GLuint num_groups_x,GLuint num_groups_y,GLuint num_groups_z);
typedef void (*PFN_glMemoryBarrier)(GLbitfield);

//参数1：hProgram 参数: uniform名字
typedef GLint(*PFN_glGetUniformLocation)(GLuint, const GLchar*);
typedef void (*PFN_glUniformMatrix4fv)(GLint, GLsizei, GLboolean, const GLfloat*);
typedef void (*PFN_glUniformMatrix3fv)(GLint location,GLsizei count,GLboolean transpose,const GLfloat* value);
typedef void (*PFN_glUniform1iv)(GLint location, GLsizei count, const GLint* value);
typedef void (*PFN_glUniform1i)(GLint location, GLint v0);
typedef void (*PFN_glUniform3f)(GLint location,GLfloat v0,GLfloat v1,GLfloat v2);
typedef void (*PFN_glUniform3fv)(GLint location,GLsizei count,const GLfloat* value);
typedef void (*PFN_glUniform4fv)(GLint location,GLsizei count,const GLfloat* value);
typedef void (*PFN_glUniform1f)(GLint location, GLfloat v0);
typedef void (*PFN_glUniform2f)(GLint location,GLfloat v0,GLfloat v1);
typedef void (*PFN_glUniform2fv)(GLint location,GLsizei count,const GLfloat* value);


typedef GLuint (*PFN_glGetSubroutineIndex)(GLuint program,GLenum shadertype,const GLchar* name);
typedef void (*PFN_glUniformSubroutinesuiv)(GLenum shadertype,GLsizei count,const GLuint* indices);


typedef void (*PFN_glGenVertexArrays)(GLsizei, GLuint*);
typedef void (*PFN_glDeleteVertexArrays)(GLsizei n,const GLuint* arrays);
typedef void (*PFN_glGenBuffers)(GLsizei, GLuint*);
typedef void (*PFN_glBindVertexArray)(GLuint);
typedef void (*PFN_glBindBuffer)(GLenum target, GLuint);
typedef GLboolean (*PFN_glUnmapBuffer)(GLenum target);
typedef void (*PFN_glBindBufferBase)(GLenum target, GLuint index, GLuint buffer);
typedef void (*PFN_glBufferData)(GLenum, GLsizeiptr, const GLvoid*, GLenum);
typedef void (*PFN_glBufferSubData)(GLenum target,GLintptr offset,GLsizeiptr size,const void* data);
typedef void (*PFN_glDeleteBuffers)(GLsizei n, const GLuint* buffers);
typedef void* (*PFN_glMapBufferRange)(GLenum target,GLintptr offset,GLsizeiptr length,GLbitfield access);

//参数1:对应着色器中的值，参数2:字段个数，参数3:字段类型，参数4:表示是否归一化（无符号映射为
//[0,1]有符号映射为[-1,1]，参数5:表示该属性相邻两次出现时的偏移间隔字节数,参数6:表示当前属性在属性组中的起始偏移量;
typedef void (*PFN_glVertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);
typedef void (*PFN_glVertexAttribIPointer)(GLuint , GLint , GLenum , GLsizei , const GLvoid* );
typedef void (*PFN_glVertexAttribDivisor)(GLuint index, GLuint divisor);
typedef void (*PFN_glEnableVertexAttribArray)(GLuint);

typedef void (*PFN_glActiveTexture)(GLenum);
typedef void (*PFN_glBindTexture)(GLenum, GLuint);
typedef void (*PFN_glTexBuffer)(GLenum target, GLenum internalformat, GLuint buffer);
typedef void (*PFN_glGenTextures)(GLsizei n, GLuint* textures);
typedef void (*PFN_glDeleteTextures)(GLsizei n, const GLuint* textures);
typedef void (*PFN_glTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (*PFN_glTexImage3D)(GLenum target,
    GLint level,
    GLint internalformat,
    GLsizei width,
    GLsizei height,
    GLsizei depth,
    GLint border,
    GLenum format,
    GLenum type,
    const void* data);
typedef void (*PFN_glTexStorage2D)(GLenum target,GLsizei levels,GLenum internalformat,GLsizei width,GLsizei height);
typedef void (*PFN_glTexParameterf)(GLenum target,GLenum pname,GLfloat param);

//参数1:绑定纹理的图像单元索引(glsl中layout中的binding值)
//参数2:纹理句柄 参数3:纹理level 参数4:纹理是否是层级纹理 参数5当不是layered纹理时，指定layer
//参数6:访问权限 参数7:纹理类别
typedef void (*PFN_glBindImageTexture)(GLuint unit,GLuint texture,GLint level,GLboolean layered,GLint layer,GLenum access,GLenum format);
typedef void (*PFN_glGenerateMipmap)(GLenum target);
typedef void (*PFN_glTexParameteri)(GLenum target, GLenum pname, GLint param);

typedef void (*PFN_glDrawElements)(GLenum, GLsizei, GLenum, const GLvoid*);
typedef void (*PFN_glDrawElementsInstanced)(GLenum mode,
    GLsizei count,
    GLenum type,
    const void* indices,
    GLsizei instancecount);
typedef void (*PFN_glDrawArrays)(GLenum, GLint, GLsizei);
typedef void (*PFN_glDrawArraysInstanced)(GLenum mode,
    GLint first,
    GLsizei count,
    GLsizei instancecount);

typedef void (*PFN_glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (*PFN_glClearColor)(GLclampf, GLclampf, GLclampf, GLclampf);
typedef void (*PFN_glClear)(GLbitfield);
typedef void (*PFN_glFlush)();
typedef GLenum (*PFN_glGetError)(void);
typedef void (*PFN_glEnable)(GLenum cap);
typedef void (*PFN_glDisable)(GLenum cap);
typedef void (*PFN_glBlendFunc)(GLenum sfactor, GLenum dfactor);
typedef void (*PFN_glPointSize)(GLfloat size);

typedef void (*PFN_glDepthRange)(GLdouble nearVal, GLdouble farVal);
typedef GLboolean(*PFN_glIsEnabled)(GLdouble nearVal, GLdouble farVal);
typedef void (*PFN_GetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint* params);

//test




namespace Sun {
    class GLFunctions
    {
    public:
        //functions 
        static PFN_glGetString glGetString;
        static PFN_glCreateShader glCreateShader;
        static PFN_glShaderSource glShaderSource;
        static PFN_glCompileShader glCompileShader;
        static PFN_glGetShaderiv glGetShaderiv;
        static PFN_glGetIntegeri_v glGetIntegeri_v;
        static PFN_glGetIntegerv glGetIntegerv;
        static PFN_glGetShaderInfoLog glGetShaderInfoLog;
        
        static PFN_glCreateProgram glCreateProgram;
        static PFN_glAttachShader glAttachShader;
        static PFN_glLinkProgram glLinkProgram;
        static PFN_glGetProgramiv glGetProgramiv;
        static PFN_glGetProgramInfoLog glGetProgramInfoLog;
        static PFN_glDeleteShader glDeleteShader;
        static PFN_glUseProgram glUseProgram;
        static PFN_glDispatchCompute glDispatchCompute;
        static PFN_glMemoryBarrier glMemoryBarrier;
        
        static PFN_glGetUniformLocation glGetUniformLocation;
        static PFN_glUniformMatrix4fv glUniformMatrix4fv;
        static PFN_glUniformMatrix3fv glUniformMatrix3fv;
        static PFN_glUniform1iv glUniform1iv;
        static PFN_glUniform1i glUniform1i;
        static PFN_glUniform3f glUniform3f;
        static PFN_glUniform1f glUniform1f;
        static PFN_glUniform2f glUniform2f;
        static PFN_glUniform3fv glUniform3fv;
        static PFN_glUniform4fv glUniform4fv;
        static PFN_glUniform2fv glUniform2fv;
        static PFN_glGetSubroutineIndex glGetSubroutineIndex;
        static PFN_glUniformSubroutinesuiv glUniformSubroutinesuiv;

        static PFN_glGenVertexArrays glGenVertexArrays;
        static PFN_glDeleteVertexArrays glDeleteVertexArrays;
        static PFN_glGenBuffers glGenBuffers;
        static PFN_glBindVertexArray glBindVertexArray;
        static PFN_glBindBuffer glBindBuffer;
        static PFN_glUnmapBuffer glUnmapBuffer;
        static PFN_glBindBufferBase glBindBufferBase;
        static PFN_glBufferData glBufferData;
        static PFN_glBufferSubData glBufferSubData;
        static PFN_glDeleteBuffers glDeleteBuffers;
        static PFN_glMapBufferRange glMapBufferRange;

        static PFN_glVertexAttribPointer glVertexAttribPointer;
        static PFN_glVertexAttribIPointer glVertexAttribIPointer;
        static PFN_glVertexAttribDivisor glVertexAttribDivisor;
        static PFN_glEnableVertexAttribArray glEnableVertexAttribArray;

        static PFN_glActiveTexture glActiveTexture;
        static PFN_glBindTexture glBindTexture;
        static PFN_glTexBuffer glTexBuffer;
        static PFN_glGenTextures glGenTextures;
        static PFN_glDeleteTextures glDeleteTextures;
        static PFN_glTexImage2D glTexImage2D;
        static PFN_glTexImage3D glTexImage3D;
        static PFN_glTexStorage2D glTexStorage2D;
        static PFN_glTexParameterf glTexParameterf;
        static PFN_glBindImageTexture glBindImageTexture;
        static PFN_glGenerateMipmap glGenerateMipmap;
        static PFN_glTexParameteri glTexParameteri;

        static PFN_glDrawElements glDrawElements;
        static PFN_glDrawElementsInstanced glDrawElementsInstanced;
        static PFN_glDrawArrays glDrawArrays;
        static PFN_glDrawArraysInstanced glDrawArraysInstanced;

        static PFN_glViewport glViewport;
        static PFN_glClearColor glClearColor;
        static PFN_glClear glClear;
        static PFN_glFlush glFlush;
        static PFN_glGetError glGetError;
        static PFN_glEnable glEnable;
        static PFN_glDisable glDisable;
        static PFN_glBlendFunc glBlendFunc;
        static PFN_glPointSize glPointSize;

        static PFN_glDepthRange glDepthRange;
        static PFN_glIsEnabled glIsEnabled;
        static PFN_GetTexLevelParameteriv GetTexLevelParameteriv;

    };

}


#endif

