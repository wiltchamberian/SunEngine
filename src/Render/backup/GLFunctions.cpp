#include "GLFunctions.h"

namespace Sun {

    std::map<const char*, void*> GLFunctions::func_ptr_map;

    const GLubyte* GLFunctions::glGetString(GLenum name)
    {
        PFN_glGetString pfunc = (PFN_glGetString)func_ptr_map["glGetString"];
        return glGetString_(name);
    }

    GLenum GLFunctions::glCreateShader(GLenum type)
    {
        PFN_glCreateShader pfunc = (PFN_glCreateShader)func_ptr_map["glCreateShader"];
        return pfunc(type);
    }

    GLuint GLFunctions::glShaderSource(GLuint shader, GLsizei count, const GLchar* const* c, const GLint* length)
    {
        PFN_glShaderSource pfunc = (PFN_glShaderSource)func_ptr_map["glShaderSource"];
        return pfunc(shader, count, c, length);
    }

    GLuint GLFunctions::glCompileShader(GLuint shader)
    {
        PFN_glCompileShader pfunc = (PFN_glCompileShader)func_ptr_map["glCompileShader"];
        return pfunc(shader);
    }

    void GLFunctions::glGetShaderiv(GLuint shader, GLenum pname, GLint* params)
    {
        PFN_glGetShaderiv pfunc = (PFN_glGetShaderiv)func_ptr_map["glGetShaderiv"];
        pfunc(shader, pname, params);
    }

    void GLFunctions::glGetShaderInfoLog(GLuint shader, GLsizei siz, GLsizei* sizptr, GLchar* cptr)
    {
        PFN_glGetShaderInfoLog pfunc = (PFN_glGetShaderInfoLog)func_ptr_map["glGetShaderInfoLog"];
        pfunc(shader, siz, sizptr, cptr);
    }

    GLuint GLFunctions::glCreateProgram()
    {
        PFN_glCreateProgram pfunc = (PFN_glCreateProgram)func_ptr_map["glCreateProgram"];
        return pfunc();
    }

    void GLFunctions::glAttachShader(GLuint shader, GLuint shader2)
    {
        PFN_glAttachShader pfunc = (PFN_glAttachShader)func_ptr_map["glAttachShader"];
        pfunc(shader, shader2);
    }

    void GLFunctions::glLinkProgram(GLuint id)
    {
        PFN_glLinkProgram pfunc = (PFN_glLinkProgram)func_ptr_map["glLinkProgram"];
        pfunc(id);
    }

    void GLFunctions::glGetProgramiv(GLuint u1, GLenum u2, GLint* u3)
    {
        PFN_glGetProgramiv pfunc = (PFN_glGetProgramiv)func_ptr_map["glGetProgramiv"];
        pfunc(u1, u2, u3);
    }

    void GLFunctions::glGetProgramInfoLog(GLuint u1, GLsizei u2, GLsizei* u3, GLchar* u4)
    {
        PFN_glGetProgramInfoLog pfunc = (PFN_glGetProgramInfoLog)func_ptr_map["glGetProgramInfoLog"];
        pfunc(u1, u2, u3, u4);
    }

    void GLFunctions::glDeleteShader(GLuint u1)
    {
        PFN_glDeleteShader pfunc = (PFN_glDeleteShader)func_ptr_map["glDeleteShader"];
        pfunc(u1);
    }

    void GLFunctions::glUseProgram(GLuint u1)
    {
        PFN_glUseProgram pfunc = (PFN_glUseProgram)func_ptr_map["glUseProgram"];
        pfunc(u1);
    }

    GLint GLFunctions::glGetUniformLocation(GLuint u1, const GLchar* u2)
    {
        PFN_glGetUniformLocation pfunc = (PFN_glGetUniformLocation)func_ptr_map["glGetUniformLocation"];
        return pfunc(u1, u2);
    }

    void GLFunctions::glUniformMatrix4fv(GLint u1, GLsizei u2, GLboolean u3, const GLfloat* u4)
    {
        PFN_glUniformMatrix4fv pfunc = (PFN_glUniformMatrix4fv)func_ptr_map["glUniformMatrix4fv"];
        pfunc(u1, u2, u3, u4);
    }

    void GLFunctions::glGenVertexArrays(GLsizei u1, GLuint* u2)
    {
        PFN_glGenVertexArrays pfunc = (PFN_glGenVertexArrays)func_ptr_map["glGenVertexArrays"];
        pfunc(u1, u2);
    }

    void GLFunctions::glGenBuffers(GLsizei u1, GLuint* u2)
    {
        PFN_glGenBuffers pfunc = (PFN_glGenBuffers)func_ptr_map["glGenBuffers"];
        pfunc(u1, u2);
    }

    void GLFunctions::glBindVertexArray(GLuint u1)
    {
        PFN_glBindVertexArray pfunc = (PFN_glBindVertexArray)func_ptr_map["glBindVertexArray"];
        pfunc(u1);
    }

    void GLFunctions::glBindBuffer(GLenum target, GLuint u)
    {
        PFN_glBindBuffer pfunc = (PFN_glBindBuffer)func_ptr_map["glBindBuffer"];
        pfunc(target, u);
    }

    void GLFunctions::glBindBufferBase(GLenum target, GLuint index, GLuint buffer) {

    }

    void GLFunctions::glBufferData(GLenum u1, GLsizeiptr u2, const GLvoid* u3, GLenum u4)
    {
        PFN_glBufferData pfunc = (PFN_glBufferData)func_ptr_map["glBufferData"];
        pfunc(u1, u2, u3, u4);
    }

    void GLFunctions::glVertexAttribPointer(GLuint u1, GLint u2, GLenum u3, GLboolean u4, GLsizei u5, const GLvoid* u6)
    {
        PFN_glVertexAttribPointer pfunc = (PFN_glVertexAttribPointer)func_ptr_map["glVertexAttribPointer"];
        pfunc(u1, u2, u3, u4, u5, u6);
    }

    void GLFunctions::glVertexAttribIPointer(GLuint u1, GLint u2, GLenum u3, GLsizei u4, const GLvoid* u5) {
        PFN_glVertexAttribIPointer pfunc = (PFN_glVertexAttribIPointer)func_ptr_map["glVertexAttribIPointer"];
        pfunc(u1, u2, u3, u4, u5);
    }

    void GLFunctions::glEnableVertexAttribArray(GLuint u1)
    {
        PFN_glEnableVertexAttribArray pfunc = (PFN_glEnableVertexAttribArray)func_ptr_map["glEnableVertexAttribArray"];
        pfunc(u1);
    }

    void GLFunctions::glActiveTexture(GLenum u1)
    {
        PFN_glActiveTexture pfunc = (PFN_glActiveTexture)func_ptr_map["glActiveTexture"];
        pfunc(u1);
    }

    void GLFunctions::glBindTexture(GLenum u1, GLuint u2)
    {
        PFN_glBindTexture pfunc = (PFN_glBindTexture)func_ptr_map["glBindTexture"];
        pfunc(u1, u2);
    }

    void GLFunctions::glGenTextures(GLsizei n, GLuint* textures)
    {
        PFN_glGenTextures pfunc = (PFN_glGenTextures)func_ptr_map["glGenTextures"];
        pfunc(n, textures);
    }

    void GLFunctions::glTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels)
    {
        PFN_glTexImage2D pfunc = (PFN_glTexImage2D)func_ptr_map["glTexImage2D"];
        pfunc(target, level, internalformat, width, height, border, format, type, pixels);
    }

    void GLFunctions::glGenerateMipmap(GLenum target)
    {
        PFN_glGenerateMipmap pfunc = (PFN_glGenerateMipmap)func_ptr_map["glGenerateMipmap"];
        pfunc(target);
    }

    void GLFunctions::glTexParameteri(GLenum target, GLenum pname, GLint param)
    {
        PFN_glTexParameteri pfunc = (PFN_glTexParameteri)func_ptr_map["glTexParameteri"];
        pfunc(target, pname, param);
    }

    void GLFunctions::glDrawElements(GLenum u1, GLsizei u2, GLenum u3, const GLvoid* u4)
    {
        PFN_glDrawElements pfunc = (PFN_glDrawElements)func_ptr_map["glDrawElements"];
        pfunc(u1, u2, u3, u4);
    }

    void GLFunctions::glDrawArrays(GLenum u1, GLint u2, GLsizei u3)
    {
        PFN_glDrawArrays pfunc = (PFN_glDrawArrays)func_ptr_map["glDrawArrays"];
        pfunc(u1, u2, u3);
    }

    void GLFunctions::glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
    {
        PFN_glViewport pfunc = (PFN_glViewport)func_ptr_map["glViewport"];
        pfunc(x, y, width, height);
    }

    void GLFunctions::glClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a)
    {
        PFN_glClearColor pfunc = (PFN_glClearColor)func_ptr_map["glClearColor"];
        pfunc(r, g, b, a);
    }

    void GLFunctions::glClear(GLbitfield b)
    {
        PFN_glClear pfunc = (PFN_glClear)func_ptr_map["glClear"];
        pfunc(b);
    }

    void GLFunctions::glFlush()
    {
        PFN_glFlush pfunc = (PFN_glFlush)func_ptr_map["glFlush"];
        pfunc();
    }

}

