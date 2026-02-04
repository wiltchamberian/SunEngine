#ifndef __GL_ENUM_H
#define __GL_ENUM_H

#include "Common/Macros.h"
#include "Common/PixelFormat.h"
#include "Common/BasicDataType.h"

namespace Sun {
    //use this format for differ from opengl func
    extern unsigned int gl_enum_map(ECapability type);

    extern unsigned int gl_enum_map(EPrimitiveType type);
    extern unsigned int gl_enum_map(EBufferType type); 
    extern unsigned int gl_enum_map(EFillMode fillMode);
    extern unsigned int gl_enum_map(EBasicDataType type);
    extern unsigned int gl_enum_map(EDrawMode mode);
    extern unsigned int gl_enum_map(ECullMode mode);
    extern unsigned int gl_enum_map(EShaderType type);
    extern unsigned int gl_enum_map(ETextureType texType);
    extern unsigned int gl_enum_map(ETextureUnit unit);
    extern unsigned int gl_enum_map(EPixelFormat pixelFormat);
    extern unsigned int gl_enum_map(EFrameBufferType type);
    extern unsigned int gl_enum_map(EAttachmentType type);
    extern unsigned int gl_enum_map(EBitField field);
    extern int gl_enum_map(ETextureWrapper wrapper);
    extern int gl_enum_map(ETextureMinMagFilter filter);
    extern unsigned int gl_enum_map(EDrawBuffer drawBuf);

    extern EFrameBufferState gl_enum_map(unsigned int framebufferState);
}


#endif 