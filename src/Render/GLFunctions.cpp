#include "GLFunctions.h"


namespace Sun {

#if 1
    //functions 
    PFN_glGetString GLFunctions::glGetString;
    PFN_glCreateShader GLFunctions::glCreateShader;
    PFN_glShaderSource GLFunctions::glShaderSource;
    PFN_glCompileShader GLFunctions::glCompileShader;
    PFN_glGetShaderiv GLFunctions::glGetShaderiv;
    PFN_glGetIntegeri_v GLFunctions::glGetIntegeri_v;
    PFN_glGetIntegerv GLFunctions::glGetIntegerv;
    PFN_glGetShaderInfoLog GLFunctions::glGetShaderInfoLog;

    PFN_glCreateProgram GLFunctions::glCreateProgram;
    PFN_glAttachShader GLFunctions::glAttachShader;
    PFN_glLinkProgram GLFunctions::glLinkProgram;
    PFN_glGetProgramiv GLFunctions::glGetProgramiv;
    PFN_glGetProgramInfoLog GLFunctions::glGetProgramInfoLog;
    PFN_glDeleteShader GLFunctions::glDeleteShader;
    PFN_glUseProgram GLFunctions::glUseProgram;
    PFN_glDispatchCompute GLFunctions::glDispatchCompute;
    PFN_glMemoryBarrier GLFunctions::glMemoryBarrier;

    PFN_glGetUniformLocation GLFunctions::glGetUniformLocation;
    PFN_glUniformMatrix4fv GLFunctions::glUniformMatrix4fv;
    PFN_glUniformMatrix3fv GLFunctions::glUniformMatrix3fv;
    PFN_glUniform1iv GLFunctions::glUniform1iv;
    PFN_glUniform1i GLFunctions::glUniform1i;
    PFN_glUniform3f GLFunctions::glUniform3f;
    PFN_glUniform1f GLFunctions::glUniform1f;
    PFN_glUniform2f GLFunctions::glUniform2f;
    PFN_glUniform3fv GLFunctions::glUniform3fv;
    PFN_glUniform4fv GLFunctions::glUniform4fv;
    PFN_glUniform2fv GLFunctions::glUniform2fv;
    PFN_glGetSubroutineIndex GLFunctions::glGetSubroutineIndex;
    PFN_glUniformSubroutinesuiv GLFunctions::glUniformSubroutinesuiv;

    PFN_glGenVertexArrays GLFunctions::glGenVertexArrays;
    PFN_glDeleteVertexArrays GLFunctions::glDeleteVertexArrays;
    PFN_glGenBuffers GLFunctions::glGenBuffers;
    PFN_glBindVertexArray GLFunctions::glBindVertexArray;
    PFN_glBindBuffer GLFunctions::glBindBuffer;
    PFN_glBindBufferBase GLFunctions::glBindBufferBase;
    PFN_glBufferData GLFunctions::glBufferData;
    PFN_glBufferSubData GLFunctions::glBufferSubData;
    PFN_glDeleteBuffers GLFunctions::glDeleteBuffers;
    PFN_glMapBufferRange GLFunctions::glMapBufferRange;
    PFN_glUnmapBuffer GLFunctions::glUnmapBuffer;

    PFN_glVertexAttribPointer GLFunctions::glVertexAttribPointer;
    PFN_glVertexAttribIPointer GLFunctions::glVertexAttribIPointer;
    PFN_glVertexAttribDivisor GLFunctions::glVertexAttribDivisor;
    PFN_glEnableVertexAttribArray GLFunctions::glEnableVertexAttribArray;

    PFN_glActiveTexture GLFunctions::glActiveTexture;
    PFN_glBindTexture GLFunctions::glBindTexture;
    PFN_glTexBuffer GLFunctions::glTexBuffer;
    PFN_glGenTextures GLFunctions::glGenTextures;
    PFN_glDeleteTextures GLFunctions::glDeleteTextures;
    PFN_glTexImage2D GLFunctions::glTexImage2D;
    PFN_glTexImage3D GLFunctions::glTexImage3D;
    PFN_glTexStorage2D GLFunctions::glTexStorage2D;
    PFN_glTexParameterf GLFunctions::glTexParameterf;
    PFN_glBindImageTexture GLFunctions::glBindImageTexture;
    PFN_glGenerateMipmap GLFunctions::glGenerateMipmap;
    PFN_glTexParameteri GLFunctions::glTexParameteri;

    PFN_glDrawElements GLFunctions::glDrawElements;
    PFN_glDrawElementsInstanced GLFunctions::glDrawElementsInstanced;
    PFN_glDrawArrays GLFunctions::glDrawArrays;
    PFN_glDrawArraysInstanced GLFunctions::glDrawArraysInstanced;

    PFN_glViewport GLFunctions::glViewport;
    PFN_glClearColor GLFunctions::glClearColor;
    PFN_glClear GLFunctions::glClear;
    PFN_glFlush GLFunctions::glFlush;
    PFN_glGetError GLFunctions::glGetError;
    PFN_glEnable GLFunctions::glEnable;
    PFN_glDisable GLFunctions::glDisable;
    PFN_glBlendFunc GLFunctions::glBlendFunc;
    PFN_glPointSize GLFunctions::glPointSize;

    PFN_glDepthRange GLFunctions::glDepthRange;
    PFN_glIsEnabled GLFunctions::glIsEnabled;
    PFN_GetTexLevelParameteriv GLFunctions::GetTexLevelParameteriv;
#endif
}

