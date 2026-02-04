/*****************************************************************************
* @brief : BasicDataType
* 基础数据类型枚举
* @author : acedtang
* @date : 2021/8/9
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/
#ifndef __BASIC_DATA_TYPE_H
#define __BASIC_DATA_TYPE_H

namespace Sun {

	enum class EBasicDataType {
        BYTE,
        UNSIGNED_BYTE,
        SHORT,
        UNSIGNED_SHORT,
        INT,
        UNSIGNED_INT,
        FLOAT,
        _2BYTES,
        _3BYTES,
        _4BYTES,
        DOUBLE,
	};

    enum class ECapability {
        DEPTH_TEST,
        CULL_FACE,
        BLEND,
        CLIP_DISTANCE0,
        CLIP_DISTANCE1,
        CLIP_DISTANCE2,
        CLIP_DISTANCE3,
        CLIP_DISTANCE4,
        CLIP_DISTANCE5,
        CLIP_DISTANCE6,
        CLIP_DISTANCE7,
        COLOR_LOGIC_OP,
        DEBUG_OUTPUT,
        DEBUG_OUTPUT_SYNCHRONOUS,
        DEPTH_CLAMP,
        DITHER,
        FRAMEBUFFER_SRGB,
        LINE_SMOOTH,
        MULTISAMPLE,
        POLYGON_OFFSET_FILL,
        POLYGON_OFFSET_LINE,
        POLYGON_OFFSET_POINT,
        POLYGON_SMOOTH,
        PRIMITIVE_RESTART,
        PRIMITIVE_RESTART_FIXED_INDEX,
        RASTERIZER_DISCARD,
        SAMPLE_ALPHA_TO_COVERAGE,
        SAMPLE_ALPHA_TO_ONE,
        SAMPLE_COVERAGE,
        SAMPLE_SHADING,
        SAMPLE_MASK,
        SCISSOR_TEST,
        TEXTURE_CUBE_MAP_SEAMLESS,
        PROGRAM_POINT_SIZE,
    };

    enum class EAttachmentType {
        COLOR_ATTACHMENT0,
        COLOR_ATTACHMENT1,
        DEPTH_STENCIL_ATTACHMENT,
        DEPTH_ATTACHMENT,
    };

    enum class ETextureType {
        TEXTURE_2D,
        TEXTURE_3D,
        INVALID,
    };

    enum class ETextureUnit : int {
        TEXTURE0,
        TEXTURE1,
        TEXTURE2,
        TEXTURE3,
        TEXTURE4,
        TEXTURE5,
        TEXTURE6,
        TEXTURE7,
        TEXTURE8,
        TEXTURE9,
        TEXTURE10,
    };

    enum class EShaderType {
        VERTEX_SHADER,
        FRAGMENT_SHADER,
        GEOMETRY_SHADER,
        COMPUTE_SHADER,
    };

    enum class EBufferType {
        //buffer
        SHADER_STORAGE_BUFFER,
        ARRAY_BUFFER,
        FRAME_BUFFER,
    };

    enum class EFillMode {
        FILL,
        LINE,
        POINT,
    };

    enum class ECullMode {
        BACK,
        FRONT,
        FRONT_AND_BACK,
    };

    enum class EPrimitiveType {
        //triangle
        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN,
        TRIANGLE_STRIP_ADJACENCY,
        TRIANGLES_ADJACENCY,

        //points
        POINTS,

        //line
        LINES,
        LINE_STRIP,
        LINE_LOOP,
        LINE_STRIP_ADJACENCY,
        LINES_ADJACENCY,

        //patch
        PATCHES,
    };

    enum class EDrawMode {
        STATIC_DRAW,
        DYNAMIC_COPY,
    };

    enum class EFrameBufferType {
        FRAME_BUFFER,
        READ_FRAME_BUFFER,
    };

    enum class EFrameBufferState {
        FRAMEBUFFER_UNDEFINED,
        FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
        FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
        FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,
        FRAMEBUFFER_INCOMPLETE_READ_BUFFER,
        FRAMEBUFFER_UNSUPPORTED,
        FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,
        FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS,
        FRAMEBUFFER_COMPLETE,
    };

    enum class EBitField : unsigned int {
        COLOR_BUFFER_BIT = 0x01,
        DEPTH_BUFFER_BIT = 0x02,
        STENCIL_BUFFER_BIT = 0x04,

    };
    inline EBitField operator | (const EBitField& b1, const EBitField& b2) {
        return EBitField((unsigned int)b1 | (unsigned int)b2);
    }

    //纹理min mag filter
    enum class ETextureMinMagFilter {
        NEAREST,
        LINEAR,
        NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR,
    };

    enum class ETextureWrapper {
        REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER,
        MIRRORED_REPEAT,
        MIRROR_CLAMP_TO_EDGE,
    };

    enum class EDrawBuffer{
        NONE,
        FRONT_LEFT,
        FRONT_RIGHT,
        BACK_LEFT,
        BACK_RIGHT,
        FRONT,
        BACK,
        LEFT,
        RIGHT,
        FRONT_AND_BACK,
    };
}

#endif