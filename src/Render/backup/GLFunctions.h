#ifndef __GLFUNCTIONS_H
#define __GLFUNCTIONS_H

#include <map>
#include "windows.h"
#include "gl/gl.h"
#include "GLMacros.h"

//all the opengl interfce for user to use
typedef char GLchar;
//typedef ptrdiff_t GLsizeiptr;

#define API_FUNC __stdcall

typedef const GLubyte* (*PFN_glGetString)(GLenum name);
typedef GLenum (*PFN_glCreateShader)(GLenum type);

typedef GLuint (*PFN_glShaderSource)(GLuint shader, GLsizei count, const GLchar* const*, const GLint* length);
typedef GLuint (*PFN_glCompileShader)(GLuint shader);
typedef void (*PFN_glGetShaderiv)(GLuint shader, GLenum pname, GLint* params);
typedef void (*PFN_glGetShaderInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*);

typedef GLuint(*PFN_glCreateProgram)();
typedef void (*PFN_glAttachShader)(GLuint, GLuint);
typedef void (*PFN_glLinkProgram)(GLuint);
typedef void (*PFN_glGetProgramiv)(GLuint, GLenum, GLint*);
typedef void (*PFN_glGetProgramInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*);
typedef void (*PFN_glDeleteShader)(GLuint);
typedef void (*PFN_glUseProgram)(GLuint);

typedef GLint(*PFN_glGetUniformLocation)(GLuint, const GLchar*);
typedef void (*PFN_glUniformMatrix4fv)(GLint, GLsizei, GLboolean, const GLfloat*);

typedef void (*PFN_glGenVertexArrays)(GLsizei, GLuint*);
typedef void (*PFN_glGenBuffers)(GLsizei, GLuint*);
typedef void (*PFN_glBindVertexArray)(GLuint);
typedef void (*PFN_glBindBuffer)(GLenum target, GLuint);
typedef void (*PFN_glBufferData)(GLenum, GLsizeiptr, const GLvoid*, GLenum);

typedef void (*PFN_glVertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);
typedef void (*PFN_glVertexAttribIPointer)(GLuint , GLint , GLenum , GLsizei , const GLvoid* );
typedef void (*PFN_glEnableVertexAttribArray)(GLuint);

typedef void (*PFN_glActiveTexture)(GLenum);
typedef void (*PFN_glBindTexture)(GLenum, GLuint);
typedef void (*PFN_glGenTextures)(GLsizei n, GLuint* textures);
typedef void (*PFN_glTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (*PFN_glGenerateMipmap)(GLenum target);
typedef void (*PFN_glTexParameteri)(GLenum target, GLenum pname, GLint param);

typedef void (*PFN_glDrawElements)(GLenum, GLsizei, GLenum, const GLvoid*);
typedef void (*PFN_glDrawArrays)(GLenum, GLint, GLsizei);


typedef void (*PFN_glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (*PFN_glClearColor)(GLclampf, GLclampf, GLclampf, GLclampf);
typedef void (*PFN_glClear)(GLbitfield);
typedef void (*PFN_glFlush)();

typedef void (*PFN_glDepthRange)(GLdouble nearVal, GLdouble farVal);
typedef GLboolean(*PFN_glIsEnabled)(GLdouble nearVal, GLdouble farVal);
typedef void (*PFN_GetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint* params);

namespace Sun {
    class GLFunctions
    {
        friend class GLLoader;
        friend class SGLRasterRender;
    public:
        static const GLubyte* glGetString(GLenum name);
        static GLenum glCreateShader(GLenum type);

        static GLuint glShaderSource(GLuint shader, GLsizei count, const GLchar* const*, const GLint* length);
        static GLuint glCompileShader(GLuint shader);
        static void glGetShaderiv(GLuint shader, GLenum pname, GLint* params);
        static void glGetShaderInfoLog(GLuint, GLsizei, GLsizei*, GLchar*);

        static GLuint glCreateProgram();
        static void glAttachShader(GLuint, GLuint);
        static void glLinkProgram(GLuint);
        static void glGetProgramiv(GLuint, GLenum, GLint*);
        static void glGetProgramInfoLog(GLuint, GLsizei, GLsizei*, GLchar*);
        static void glDeleteShader(GLuint);
        static void glUseProgram(GLuint);

        static GLint glGetUniformLocation(GLuint, const GLchar*);
        static void glUniformMatrix4fv(GLint, GLsizei, GLboolean, const GLfloat*);

        static void glGenVertexArrays(GLsizei, GLuint*);
        static void glGenBuffers(GLsizei, GLuint*);
        static void glBindVertexArray(GLuint);
        static void glBindBuffer(GLenum target, GLuint);
        static void glBindBufferBase(GLenum target,GLuint index, GLuint buffer);
        static void glBufferData(GLenum, GLsizeiptr, const GLvoid*, GLenum);

        static void glVertexAttribPointer(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);
        static void glVertexAttribIPointer(GLuint, GLint, GLenum, GLsizei, const GLvoid*);
        static void glEnableVertexAttribArray(GLuint);

        static void glActiveTexture(GLenum);
        static void glBindTexture(GLenum, GLuint);
        static void glGenTextures(GLsizei n, GLuint* textures);
        static void glTexImage2D(GLenum target, GLint level, GLenum internalformat,
            GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
        static void glGenerateMipmap(GLenum target);
        static void glTexParameteri(GLenum target, GLenum pname, GLint param);

        static void glDrawElements(GLenum, GLsizei, GLenum, const GLvoid*);
        static void glDrawArrays(GLenum, GLint, GLsizei);

        static void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
        static void glClearColor(GLclampf, GLclampf, GLclampf, GLclampf);
        static void glClear(GLbitfield);
        static void glFlush();
    protected:
        static std::map<const char*, void*> func_ptr_map;

        //functions:
        static const GLubyte* glGetString(GLenum name);
        static GLenum glCreateShader(GLenum type);

        static GLuint glShaderSource(GLuint shader, GLsizei count, const GLchar* const*, const GLint* length);
        static GLuint glCompileShader(GLuint shader);
        static void glGetShaderiv(GLuint shader, GLenum pname, GLint* params);
        static void glGetShaderInfoLog(GLuint, GLsizei, GLsizei*, GLchar*);

        static GLuint glCreateProgram();
        static void glAttachShader(GLuint, GLuint);
        static void glLinkProgram(GLuint);
        static void glGetProgramiv(GLuint, GLenum, GLint*);
        static void glGetProgramInfoLog(GLuint, GLsizei, GLsizei*, GLchar*);
        static void glDeleteShader(GLuint);
        static void glUseProgram(GLuint);

        static GLint glGetUniformLocation(GLuint, const GLchar*);
        static void glUniformMatrix4fv(GLint, GLsizei, GLboolean, const GLfloat*);

        static void glGenVertexArrays(GLsizei, GLuint*);
        static void glGenBuffers(GLsizei, GLuint*);
        static void glBindVertexArray(GLuint);
        static void glBindBuffer(GLenum target, GLuint);
        static void glBindBufferBase(GLenum target, GLuint index, GLuint buffer);
        static void glBufferData(GLenum, GLsizeiptr, const GLvoid*, GLenum);

        //functions 
        PFN_glGetString glGetString_;
        PFN_glCreateShader glCreateShader_;
        PFN_glShaderSource glShaderSource_;
        PFN_glCompileShader glCompileShader_;
        PFN_glGetShaderiv glGetShaderiv_;
        PFN_glGetShaderInfoLog glGetShaderInfoLog_;

        PFN_glCreateProgram glCreateProgram_;
        PFN_glAttachShader glAttachShader_;
        PFN_glLinkProgram glLinkProgram_;
        PFN_glGetProgramiv glGetProgramiv_;
        PFN_glGetProgramInfoLog glGetProgramInfoLog_;
        PFN_glDeleteShader glDeleteShader_;
        PFN_glUseProgram glUseProgram_;

        PFN_glGetUniformLocation glGetUniformLocation_;
        PFN_glUniformMatrix4fv glUniformMatrix4fv_;

        PFN_glGenVertexArrays glGenVertexArrays_;
        PFN_glGenBuffers glGenBuffers_;
        PFN_glBindVertexArray glBindVertexArray_;
        PFN_glBindBuffer glBindBuffer_;
        PFN_glBufferData glBufferData_;

        PFN_glVertexAttribPointer glVertexAttribPointer_;
        PFN_glVertexAttribIPointer glVertexAttribIPointer_;
        PFN_glEnableVertexAttribArray glEnableVertexAttribArray_;

        PFN_glActiveTexture glActiveTexture_;
        PFN_glBindTexture glBindTexture_;
        PFN_glGenTextures glGenTextures_;
        PFN_glTexImage2D glTexImage2D_;
        PFN_glGenerateMipmap glGenerateMipmap_;
        PFN_glTexParameteri glTexParameteri_;

        PFN_glDrawElements glDrawElements_;
        PFN_glDrawArrays glDrawArrays_;

        PFN_glViewport glViewport_;
        PFN_glClearColor glClearColor_;
        PFN_glClear glClear_;
        PFN_glFlush glFlush_;

        PFN_glDepthRange glDepthRange_;
        PFN_glIsEnabled glIsEnabled_;
        PFN_GetTexLevelParameteriv GetTexLevelParameteriv_;

    };

}


#endif

