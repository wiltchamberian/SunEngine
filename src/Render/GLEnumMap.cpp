#include "GLEnumMap.h"
#include "Util/exception.h"
//#include "Render/GLMacros.h"
#include "ThirdParty/glfw/glad/glad.h"

namespace Sun {

    unsigned int gl_enum_map(ECapability type) {
        switch (type) {    
            case ECapability::DEPTH_TEST: {
                return GL_DEPTH_TEST;
            }
            break;
            case ECapability::CULL_FACE: 
            {
                return GL_CULL_FACE;
            }
            break;
            case ECapability::BLEND:
            {
                return GL_BLEND;
            }
            break;
            case ECapability::CLIP_DISTANCE0:
            {
                return GL_CLIP_DISTANCE0;
            }
            break;
            case ECapability::CLIP_DISTANCE1:
            {
                return GL_CLIP_DISTANCE1;
            }
            break;
            case ECapability::CLIP_DISTANCE2:
            {
                return GL_CLIP_DISTANCE2;
            }
            break;
            case ECapability::CLIP_DISTANCE3:
            {
                return GL_CLIP_DISTANCE3;
            }
            break;
            case ECapability::CLIP_DISTANCE4:
            {
                return GL_CLIP_DISTANCE4;
            }
            break;
            case ECapability::CLIP_DISTANCE5:
            {
                return GL_CLIP_DISTANCE5;
            }
            break;
            case ECapability::CLIP_DISTANCE6:
            {
                return GL_CLIP_DISTANCE6;
            }
            break;
            case ECapability::CLIP_DISTANCE7:
            {
                return GL_CLIP_DISTANCE7;
            }
            break;
            case ECapability::COLOR_LOGIC_OP:
            {
                return GL_COLOR_LOGIC_OP;
            }
            break;
            case ECapability::DEBUG_OUTPUT:
            {
                return GL_DEBUG_OUTPUT;
            }
            break;
            case ECapability::DEBUG_OUTPUT_SYNCHRONOUS:
            {
                return GL_DEBUG_OUTPUT_SYNCHRONOUS;
            }
            break;
            case ECapability::DEPTH_CLAMP:
            {
                return GL_DEPTH_CLAMP;
            }
            break;
            case ECapability::DITHER:
            {
                return GL_DITHER;
            }
            break;
            case ECapability::FRAMEBUFFER_SRGB:
            {
                return GL_FRAMEBUFFER_SRGB;
            }
            break;
            case ECapability::LINE_SMOOTH:
            {
                return GL_LINE_SMOOTH;
            }
            break;
            case ECapability::MULTISAMPLE:
            {
                return GL_MULTISAMPLE;
            }
            break;
            case ECapability::POLYGON_OFFSET_FILL:
            {
                return GL_POLYGON_OFFSET_FILL;
            }
            break;
            case ECapability::POLYGON_OFFSET_LINE:
            {
                return GL_POLYGON_OFFSET_LINE;
            }
            break;
            case ECapability::POLYGON_OFFSET_POINT:
            {
                return GL_POLYGON_OFFSET_POINT;
            }
            break;
            case ECapability::POLYGON_SMOOTH:
            {
                return GL_POLYGON_SMOOTH;
            }
            break;
            case ECapability::PRIMITIVE_RESTART:
            {
                return GL_PRIMITIVE_RESTART;
            }
            break;
            case ECapability::PRIMITIVE_RESTART_FIXED_INDEX:
            {
                return GL_PRIMITIVE_RESTART_FIXED_INDEX;
            }
            break;
            case ECapability::RASTERIZER_DISCARD:
            {
                return GL_RASTERIZER_DISCARD;
            }
            break;
            case ECapability::SAMPLE_ALPHA_TO_COVERAGE:
            {
                return GL_SAMPLE_ALPHA_TO_COVERAGE;
            }
            break;
            case ECapability::SAMPLE_ALPHA_TO_ONE:
            {
                return GL_SAMPLE_ALPHA_TO_ONE;
            }
            break;
            case ECapability::SAMPLE_COVERAGE:
            {
                return GL_SAMPLE_COVERAGE;
            }
            break;
            case ECapability::SAMPLE_SHADING:
            {
                return GL_SAMPLE_SHADING;
            }
            break;
            case ECapability::SAMPLE_MASK:
            {
                return GL_SAMPLE_MASK;
            }
            break;
            case ECapability::SCISSOR_TEST:
            {
                return GL_SCISSOR_TEST;
            }
            break;
            case ECapability::TEXTURE_CUBE_MAP_SEAMLESS:
            {
                return GL_TEXTURE_CUBE_MAP_SEAMLESS;
            }
            break;
            case ECapability::PROGRAM_POINT_SIZE:
            {
                return GL_PROGRAM_POINT_SIZE;
            }
            break;
            default:
            {
                ASSERT(false);
                break;
            }
        }

        return 0;
    }

    unsigned int gl_enum_map(EPrimitiveType type) {
        switch (type) {
            case EPrimitiveType::TRIANGLES:
            {
                return GL_TRIANGLES;
            }
            break;
            case EPrimitiveType::TRIANGLE_STRIP:
            {
                return GL_TRIANGLE_STRIP;
            }
            break;
            case EPrimitiveType::TRIANGLE_FAN:
            {
                return GL_TRIANGLE_FAN;
            }
            break;
            case EPrimitiveType::TRIANGLE_STRIP_ADJACENCY:
            {
                return GL_TRIANGLE_STRIP_ADJACENCY;
            }
            break;
            case EPrimitiveType::TRIANGLES_ADJACENCY:
            {
                return GL_TRIANGLES_ADJACENCY;
            }
            break;
            case EPrimitiveType::POINTS:
            {
                return GL_POINTS;
            }
            break;
            case EPrimitiveType::LINES:
            {
                return GL_LINES;
            }
            break;
            case EPrimitiveType::LINE_STRIP:
            {
                return GL_LINE_STRIP;
            }
            break;
            case EPrimitiveType::LINE_LOOP:
            {
                return GL_LINE_LOOP;
            }
            break;
            case EPrimitiveType::LINE_STRIP_ADJACENCY:
            {
                return GL_LINE_STRIP_ADJACENCY;
            }
            break;
            case EPrimitiveType::LINES_ADJACENCY:
            {
                return GL_LINES_ADJACENCY;
            }
            break;
            case EPrimitiveType::PATCHES:
            {
                return GL_PATCHES;
            }
            break;
            default:
            {
                assert(false);
                break;
            }
        }
        return 0;
    }

    unsigned int gl_enum_map(EBufferType type) {
        switch (type) {
            case EBufferType::SHADER_STORAGE_BUFFER: 
            {
                return GL_SHADER_STORAGE_BUFFER;
            }
            break;
            case EBufferType::ARRAY_BUFFER:
            {
                return GL_ARRAY_BUFFER;
            }
            break;
            default: {
                assert(false);
                break;
            }
        }
        return 0;
    }

    unsigned int gl_enum_map(EFillMode fillMode) {
        switch (fillMode) {
            case EFillMode::FILL: {
                return GL_FILL;
            }
            break;
            case EFillMode::LINE: {
                return GL_LINE;
            }
            break;
            case EFillMode::POINT: {
                return GL_POINT;
            }
            break;
            default: {
                assert(false);
                break;
            }
        }
        return 0;
    }

    unsigned int gl_enum_map(EBasicDataType type) {
        switch (type) {
            case EBasicDataType::INT:
            {
                return GL_INT;
            }
            break;
            case EBasicDataType::UNSIGNED_INT:
            {
                return GL_UNSIGNED_INT;
            }
            break;
            case EBasicDataType::SHORT:
            {
                return GL_SHORT;
            }
            break;
            case EBasicDataType::UNSIGNED_SHORT:
            {
                return GL_UNSIGNED_SHORT;
            }
            break;
            case EBasicDataType::FLOAT:
            {
                return GL_FLOAT;
            }
            break;
            case EBasicDataType::BYTE:
            {
                return GL_BYTE;
            }
            break;
            case EBasicDataType::UNSIGNED_BYTE:
            {
                return GL_UNSIGNED_BYTE;
            }
            break;
            default:
            {
                assert(false);
                break;
            }
        }
        return GL_INT;
    }

    unsigned int gl_enum_map(EDrawMode mode) {
        switch (mode) {
            case EDrawMode::STATIC_DRAW:
            {
                return GL_STATIC_DRAW;
            }
            break;
            case EDrawMode::DYNAMIC_COPY:
            {
                return GL_DYNAMIC_COPY;
            }
            break;
            default: {
                assert(false);
                break;
            }
            
        }
        return 0;
    }

    unsigned int gl_enum_map(ECullMode mode) {
        switch (mode) {
            case ECullMode::BACK:
            {
                return GL_BACK;
            }
            break;
            case ECullMode::FRONT:
            {
                return GL_FRONT;
            }
            break;
            case ECullMode::FRONT_AND_BACK:
            {
                return GL_FRONT_AND_BACK;
            }
            break;
            default:
            {
                assert(false);
                break;
            }
        }
        return 0;
    }

    unsigned int gl_enum_map(EShaderType type) {
        switch (type) {
            case EShaderType::VERTEX_SHADER:
            {
                return GL_VERTEX_SHADER;
            }
            break;
            case EShaderType::FRAGMENT_SHADER:
            {
                return GL_FRAGMENT_SHADER;
            }
            break;
            case EShaderType::GEOMETRY_SHADER:
            {
                return GL_GEOMETRY_SHADER;
            }
            break;
            case EShaderType::COMPUTE_SHADER:
            {
                return GL_COMPUTE_SHADER;
            }
            break;
            default: {
                assert(false);
                break;
            }
        }
        return 0;
    }

    unsigned int gl_enum_map(ETextureType texType) {
        switch (texType) {
            case ETextureType::TEXTURE_2D:
            {
                return GL_TEXTURE_2D;
            }
            break;
            case ETextureType::TEXTURE_3D:
            {
                return GL_TEXTURE_3D;
            }
            break;
            default:
            {
                assert(false);
                break;
            }
        }
        return GL_TEXTURE_2D;
    }

    unsigned int gl_enum_map(ETextureUnit unit) {
        switch (unit) {
            case ETextureUnit::TEXTURE0:
            {
                return GL_TEXTURE0;
            }
            break;
            case ETextureUnit::TEXTURE1:
            {
                return GL_TEXTURE1;
            }
            break;
            case ETextureUnit::TEXTURE2:
            {
                return GL_TEXTURE2;
            }
            break;
            case ETextureUnit::TEXTURE3:
            {
                return GL_TEXTURE3;
            }
            break;
            case ETextureUnit::TEXTURE4:
            {
                return GL_TEXTURE4;
            }
            break;
            case ETextureUnit::TEXTURE5:
            {
                return GL_TEXTURE5;
            }
            break;
            case ETextureUnit::TEXTURE6:
            {
                return GL_TEXTURE6;
            }
            break;
            case ETextureUnit::TEXTURE7:
            {
                return GL_TEXTURE7;
            }
            break;
            case ETextureUnit::TEXTURE8:
            {
                return GL_TEXTURE8;
            }
            break;
            case ETextureUnit::TEXTURE9:
            {
                return GL_TEXTURE9;
            }
            break;
            case ETextureUnit::TEXTURE10:
            {
                return GL_TEXTURE10;
            }
            break;
            default: {
                ASSERT(false);
                break;
            }
        }
        return GL_TEXTURE0;
    }

    unsigned int gl_enum_map(EPixelFormat pixelFormat) {
        switch (pixelFormat) {
            case EPixelFormat::RGB: {
                return GL_RGB;
            }
            break;
            case EPixelFormat::RGBA:
            {
                return GL_RGBA;
            }
            break;
            case EPixelFormat::RGB16F:
            {
                return GL_RGB16F;
            }
            break;
            case EPixelFormat::RGB32F:
            {
                return GL_RGB32F;
            }
            break;
            case EPixelFormat::RGBA16F:
            {
                return GL_RGBA16F;
            }
            break;
            case EPixelFormat::RGBA32F:
            {
                return GL_RGBA32F;
            }
            break;
            case EPixelFormat::STENCIL_INDEX:
            {
                return GL_STENCIL_INDEX;
            }
            break;
            case EPixelFormat::DEPTH_COMPONENT:
            {
                return GL_DEPTH_COMPONENT;
            }
            break;
            case EPixelFormat::DEPTH_STENCIL:
            {
                return GL_DEPTH_STENCIL;
            }
            break;
            case EPixelFormat::DEPTH24_STENCIL8:
            {
                return GL_DEPTH24_STENCIL8;
            }
            break;
            default:
            {
                assert(false);
                break;
            }
        }
        return GL_RGB;
    }

    unsigned int gl_enum_map(EFrameBufferType type) {
        switch (type) {
        case EFrameBufferType::FRAME_BUFFER:
        {
            return GL_FRAMEBUFFER;
        }
        break;
        case EFrameBufferType::READ_FRAME_BUFFER:
        {
            return GL_READ_FRAMEBUFFER;
        }
        break;
        default:
        {
            assert(false);
            break;
        }
        }
        return 0;
    }

    unsigned int gl_enum_map(EAttachmentType type) {
        switch (type) {
        case EAttachmentType::COLOR_ATTACHMENT0:
        {
            return GL_COLOR_ATTACHMENT0;
        }
        break;
        case EAttachmentType::COLOR_ATTACHMENT1:
        {
            return GL_COLOR_ATTACHMENT1;
        }
        break;
        case EAttachmentType::DEPTH_STENCIL_ATTACHMENT:
        {
            return GL_DEPTH_STENCIL_ATTACHMENT;
        }
        break;
        case EAttachmentType::DEPTH_ATTACHMENT: {
            return GL_DEPTH_ATTACHMENT;
        }
        break;
        default:
        {
            assert(false);
            break;
        }
        }
        return 0;
    }

    unsigned int gl_enum_map(EBitField field) {
        unsigned int res = 0;
        if ((unsigned int)field & (unsigned int)EBitField::COLOR_BUFFER_BIT) {
            res = res | GL_COLOR_BUFFER_BIT;
        }
        if ((unsigned int)field & (unsigned int)EBitField::DEPTH_BUFFER_BIT) {
            res = res | GL_DEPTH_BUFFER_BIT;
        }
        if ((unsigned int)field & (unsigned int)EBitField::STENCIL_BUFFER_BIT) {
            res = res | GL_STENCIL_BUFFER_BIT;
        }
        return res;
    }

    int gl_enum_map(ETextureWrapper wrapper) {
        switch (wrapper) {
        case ETextureWrapper::REPEAT:
            return GL_REPEAT;
            break;
        case ETextureWrapper::CLAMP_TO_BORDER:
            return GL_CLAMP_TO_BORDER;
            break;
        case ETextureWrapper::CLAMP_TO_EDGE:
            return GL_CLAMP_TO_EDGE;
            break;
        case ETextureWrapper::MIRROR_CLAMP_TO_EDGE:
            return GL_MIRROR_CLAMP_TO_EDGE;
            break;
        case ETextureWrapper::MIRRORED_REPEAT:
            return GL_MIRRORED_REPEAT;
            break;
        default:
            return GL_REPEAT;
        }
        return GL_REPEAT;
    }

    int gl_enum_map(ETextureMinMagFilter filter) {
        switch (filter) {
        case ETextureMinMagFilter::LINEAR:
            return GL_LINEAR;
            break;
        case ETextureMinMagFilter::LINEAR_MIPMAP_LINEAR:
            return GL_LINEAR_MIPMAP_LINEAR;
            break;
        case ETextureMinMagFilter::LINEAR_MIPMAP_NEAREST:
            return GL_LINEAR_MIPMAP_NEAREST;
                break;
        case ETextureMinMagFilter::NEAREST:
            return GL_NEAREST;
            break;
        case ETextureMinMagFilter::NEAREST_MIPMAP_LINEAR:
            return GL_NEAREST_MIPMAP_LINEAR;
            break;
        case ETextureMinMagFilter::NEAREST_MIPMAP_NEAREST:
            return GL_NEAREST_MIPMAP_NEAREST;
            break;
        default:
            assert(false);
            return GL_LINEAR;
        }
        return GL_LINEAR;
    }

    unsigned int gl_enum_map(EDrawBuffer drawBuf) {
        switch (drawBuf) {
        case EDrawBuffer::BACK: 
        {
            return GL_BACK;
        }
        break;
        case EDrawBuffer::FRONT:
        {
            return GL_FRONT;
        }
        break;
        case EDrawBuffer::LEFT:
        {
            return GL_LEFT;
        }
        break;
        case EDrawBuffer::RIGHT:
        {
            return GL_RIGHT;
        }
        break;
        case EDrawBuffer::FRONT_LEFT:
        {
            return GL_FRONT_LEFT;
        }
        break;
        case EDrawBuffer::FRONT_RIGHT:
        {
            return GL_FRONT_RIGHT;
        }
        break;
        case EDrawBuffer::BACK_LEFT:
        {
            return GL_BACK_LEFT;
        }
        break;
        case EDrawBuffer::BACK_RIGHT:
        {
            return GL_BACK_RIGHT;
        }
        break;
        case EDrawBuffer::NONE:
        {
            return GL_NONE;
        }
        break;
        case EDrawBuffer::FRONT_AND_BACK:
        {
            return GL_FRONT_AND_BACK;
        }
        break;
        default: {
            assert(false);
            return GL_FRONT_AND_BACK;
        }
        }
        return GL_FRONT_AND_BACK;
    }

    EFrameBufferState gl_enum_map(unsigned int st) {
        EFrameBufferState state;
        switch (st) {
            case GL_FRAMEBUFFER_UNDEFINED:
            {
                return EFrameBufferState::FRAMEBUFFER_UNDEFINED;
            }
            break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            {
                return EFrameBufferState::FRAMEBUFFER_INCOMPLETE_ATTACHMENT;
            }
            break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            {
                return EFrameBufferState::FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT;
            }
            break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            {
                return EFrameBufferState::FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER;
            }
            break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            {
                return EFrameBufferState::FRAMEBUFFER_INCOMPLETE_READ_BUFFER;
            }
            break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
            {
                return EFrameBufferState::FRAMEBUFFER_UNSUPPORTED;
            }
            break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            {
                return EFrameBufferState::FRAMEBUFFER_INCOMPLETE_MULTISAMPLE;
            }
            break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            {
                return EFrameBufferState::FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS;
            }
            break;
            case GL_FRAMEBUFFER_COMPLETE:
            {
                return EFrameBufferState::FRAMEBUFFER_COMPLETE;
            }
            break;
            default:
            {
                assert(false);
                break;
            }
        }
        return EFrameBufferState::FRAMEBUFFER_UNDEFINED;
    }

}