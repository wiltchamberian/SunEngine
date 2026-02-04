#ifndef __SUN_RHI_RHIDEFINE_H
#define __SUN_RHI_RHIDEFINE_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "Common/BasicDataType.h"

namespace Sun {
namespace rhi {

    using Bool32 = uint32_t;

    using SampleMask = uint32_t;

    template<typename _T>
    using SmartPtr = std::shared_ptr<_T>;

    using DeviceSize = std::uint64_t;

    using Flags = uint32_t;
    using CullModeFlags = Flags;

#define DEFINE_HANDLE(object) struct object{ void* backend = nullptr; };
#define DEFINE_NON_DISPATCHABLE_HANDLE(object) struct object{ uint64_t backend = 0; };

#define UUID_SIZE                         16
#define MAX_PHYSICAL_DEVICE_NAME_SIZE     256

#define ATTACHMENT_UNUSED              (~0U)
#define FALSE                          0
#define LOD_CLAMP_NONE                 1000.0f
#define QUEUE_FAMILY_IGNORED           (~0U)
#define REMAINING_ARRAY_LAYERS         (~0U)
#define REMAINING_MIP_LEVELS           (~0U)
#define SUBPASS_EXTERNAL               (~0U)
#define TRUE                           1
#define WHOLE_SIZE                     (~0ULL)
#define MAX_MEMORY_TYPES               32
#define MAX_MEMORY_HEAPS               16
#define MAX_PHYSICAL_DEVICE_NAME_SIZE  256
#define UUID_SIZE                      16
#define MAX_EXTENSION_NAME_SIZE        256
#define MAX_DESCRIPTION_SIZE           256

    enum class EResult {
        SUCCESS,
        FAIL,
    };

    enum ECullModeFlagBits {
        CULL_MODE_NONE = 0,
        CULL_MODE_FRONT_BIT = 0x00000001,
        CULL_MODE_BACK_BIT = 0x00000002,
        CULL_MODE_FRONT_AND_BACK = 0x00000003,
        CULL_MODE_FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
    };

    enum class EShaderStageFlagBits {
        VERTEX_BIT = 0x00000001,
        TESSELLATION_CONTROL_BIT = 0x00000002,
        TESSELLATION_EVALUATION_BIT = 0x00000004,
        GEOMETRY_BIT = 0x00000008,
        FRAGMENT_BIT = 0x00000010,
        COMPUTE_BIT = 0x00000020,
        ALL_GRAPHICS = 0x0000001F,
        ALL = 0x7FFFFFFF,
        RAYGEN_BIT_KHR = 0x00000100,
        ANY_HIT_BIT_KHR = 0x00000200,
        CLOSEST_HIT_BIT_KHR = 0x00000400,
        MISS_BIT_KHR = 0x00000800,
        INTERSECTION_BIT_KHR = 0x00001000,
        CALLABLE_BIT_KHR = 0x00002000,
        TASK_BIT_NV = 0x00000040,
        MESH_BIT_NV = 0x00000080,
        RAYGEN_BIT_NV = RAYGEN_BIT_KHR,
        ANY_HIT_BIT_NV = ANY_HIT_BIT_KHR,
        CLOSEST_HIT_BIT_NV = CLOSEST_HIT_BIT_KHR,
        MISS_BIT_NV = MISS_BIT_KHR,
        INTERSECTION_BIT_NV = INTERSECTION_BIT_KHR,
        CALLABLE_BIT_NV = CALLABLE_BIT_KHR,
        FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
    };

    enum class ECompareOp {
        NEVER = 0,
        VLESS = 1,
        EQUAL = 2,
        LESS_OR_EQUAL = 3,
        GREATER = 4,
        NOT_EQUAL = 5,
        GREATER_OR_EQUAL = 6,
        ALWAYS = 7,
        MAX_ENUM = 0x7FFFFFFF
    };

    //this is copy from VkFormat,so can be directly cast to VkFormat
    //for opengl,should do a transform
    enum class EFormat {
        UNDEFINED = 0,
        R4G4_UNORM_PACK8 = 1,
        R4G4B4A4_UNORM_PACK16 = 2,
        B4G4R4A4_UNORM_PACK16 = 3,
        R5G6B5_UNORM_PACK16 = 4,
        B5G6R5_UNORM_PACK16 = 5,
        R5G5B5A1_UNORM_PACK16 = 6,
        B5G5R5A1_UNORM_PACK16 = 7,
        A1R5G5B5_UNORM_PACK16 = 8,
        R8_UNORM = 9,
        R8_SNORM = 10,
        R8_USCALED = 11,
        R8_SSCALED = 12,
        R8_UINT = 13,
        R8_SINT = 14,
        R8_SRGB = 15,
        R8G8_UNORM = 16,
        R8G8_SNORM = 17,
        R8G8_USCALED = 18,
        R8G8_SSCALED = 19,
        R8G8_UINT = 20,
        R8G8_SINT = 21,
        R8G8_SRGB = 22,
        R8G8B8_UNORM = 23,
        R8G8B8_SNORM = 24,
        R8G8B8_USCALED = 25,
        R8G8B8_SSCALED = 26,
        R8G8B8_UINT = 27,
        R8G8B8_SINT = 28,
        R8G8B8_SRGB = 29,
        B8G8R8_UNORM = 30,
        B8G8R8_SNORM = 31,
        B8G8R8_USCALED = 32,
        B8G8R8_SSCALED = 33,
        B8G8R8_UINT = 34,
        B8G8R8_SINT = 35,
        B8G8R8_SRGB = 36,
        R8G8B8A8_UNORM = 37,
        R8G8B8A8_SNORM = 38,
        R8G8B8A8_USCALED = 39,
        R8G8B8A8_SSCALED = 40,
        R8G8B8A8_UINT = 41,
        R8G8B8A8_SINT = 42,
        R8G8B8A8_SRGB = 43,
        B8G8R8A8_UNORM = 44,
        B8G8R8A8_SNORM = 45,
        B8G8R8A8_USCALED = 46,
        B8G8R8A8_SSCALED = 47,
        B8G8R8A8_UINT = 48,
        B8G8R8A8_SINT = 49,
        B8G8R8A8_SRGB = 50,
        A8B8G8R8_UNORM_PACK32 = 51,
        A8B8G8R8_SNORM_PACK32 = 52,
        A8B8G8R8_USCALED_PACK32 = 53,
        A8B8G8R8_SSCALED_PACK32 = 54,
        A8B8G8R8_UINT_PACK32 = 55,
        A8B8G8R8_SINT_PACK32 = 56,
        A8B8G8R8_SRGB_PACK32 = 57,
        A2R10G10B10_UNORM_PACK32 = 58,
        A2R10G10B10_SNORM_PACK32 = 59,
        A2R10G10B10_USCALED_PACK32 = 60,
        A2R10G10B10_SSCALED_PACK32 = 61,
        A2R10G10B10_UINT_PACK32 = 62,
        A2R10G10B10_SINT_PACK32 = 63,
        A2B10G10R10_UNORM_PACK32 = 64,
        A2B10G10R10_SNORM_PACK32 = 65,
        A2B10G10R10_USCALED_PACK32 = 66,
        A2B10G10R10_SSCALED_PACK32 = 67,
        A2B10G10R10_UINT_PACK32 = 68,
        A2B10G10R10_SINT_PACK32 = 69,
        R16_UNORM = 70,
        R16_SNORM = 71,
        R16_USCALED = 72,
        R16_SSCALED = 73,
        R16_UINT = 74,
        R16_SINT = 75,
        R16_SFLOAT = 76,
        R16G16_UNORM = 77,
        R16G16_SNORM = 78,
        R16G16_USCALED = 79,
        R16G16_SSCALED = 80,
        R16G16_UINT = 81,
        R16G16_SINT = 82,
        R16G16_SFLOAT = 83,
        R16G16B16_UNORM = 84,
        R16G16B16_SNORM = 85,
        R16G16B16_USCALED = 86,
        R16G16B16_SSCALED = 87,
        R16G16B16_UINT = 88,
        R16G16B16_SINT = 89,
        R16G16B16_SFLOAT = 90,
        R16G16B16A16_UNORM = 91,
        R16G16B16A16_SNORM = 92,
        R16G16B16A16_USCALED = 93,
        R16G16B16A16_SSCALED = 94,
        R16G16B16A16_UINT = 95,
        R16G16B16A16_SINT = 96,
        R16G16B16A16_SFLOAT = 97,
        R32_UINT = 98,
        R32_SINT = 99,
        R32_SFLOAT = 100,
        R32G32_UINT = 101,
        R32G32_SINT = 102,
        R32G32_SFLOAT = 103,
        R32G32B32_UINT = 104,
        R32G32B32_SINT = 105,
        R32G32B32_SFLOAT = 106,
        R32G32B32A32_UINT = 107,
        R32G32B32A32_SINT = 108,
        R32G32B32A32_SFLOAT = 109,
        R64_UINT = 110,
        R64_SINT = 111,
        R64_SFLOAT = 112,
        R64G64_UINT = 113,
        R64G64_SINT = 114,
        R64G64_SFLOAT = 115,
        R64G64B64_UINT = 116,
        R64G64B64_SINT = 117,
        R64G64B64_SFLOAT = 118,
        R64G64B64A64_UINT = 119,
        R64G64B64A64_SINT = 120,
        R64G64B64A64_SFLOAT = 121,
        B10G11R11_UFLOAT_PACK32 = 122,
        E5B9G9R9_UFLOAT_PACK32 = 123,
        D16_UNORM = 124,
        X8_D24_UNORM_PACK32 = 125,
        D32_SFLOAT = 126,
        S8_UINT = 127,
        D16_UNORM_S8_UINT = 128,
        D24_UNORM_S8_UINT = 129,
        D32_SFLOAT_S8_UINT = 130,
        BC1_RGB_UNORM_BLOCK = 131,
        BC1_RGB_SRGB_BLOCK = 132,
        BC1_RGBA_UNORM_BLOCK = 133,
        BC1_RGBA_SRGB_BLOCK = 134,
        BC2_UNORM_BLOCK = 135,
        BC2_SRGB_BLOCK = 136,
        BC3_UNORM_BLOCK = 137,
        BC3_SRGB_BLOCK = 138,
        BC4_UNORM_BLOCK = 139,
        BC4_SNORM_BLOCK = 140,
        BC5_UNORM_BLOCK = 141,
        BC5_SNORM_BLOCK = 142,
        BC6H_UFLOAT_BLOCK = 143,
        BC6H_SFLOAT_BLOCK = 144,
        BC7_UNORM_BLOCK = 145,
        BC7_SRGB_BLOCK = 146,
        ETC2_R8G8B8_UNORM_BLOCK = 147,
        ETC2_R8G8B8_SRGB_BLOCK = 148,
        ETC2_R8G8B8A1_UNORM_BLOCK = 149,
        ETC2_R8G8B8A1_SRGB_BLOCK = 150,
        ETC2_R8G8B8A8_UNORM_BLOCK = 151,
        ETC2_R8G8B8A8_SRGB_BLOCK = 152,
        EAC_R11_UNORM_BLOCK = 153,
        EAC_R11_SNORM_BLOCK = 154,
        EAC_R11G11_UNORM_BLOCK = 155,
        EAC_R11G11_SNORM_BLOCK = 156,
        ASTC_4x4_UNORM_BLOCK = 157,
        ASTC_4x4_SRGB_BLOCK = 158,
        ASTC_5x4_UNORM_BLOCK = 159,
        ASTC_5x4_SRGB_BLOCK = 160,
        ASTC_5x5_UNORM_BLOCK = 161,
        ASTC_5x5_SRGB_BLOCK = 162,
        ASTC_6x5_UNORM_BLOCK = 163,
        ASTC_6x5_SRGB_BLOCK = 164,
        ASTC_6x6_UNORM_BLOCK = 165,
        ASTC_6x6_SRGB_BLOCK = 166,
        ASTC_8x5_UNORM_BLOCK = 167,
        ASTC_8x5_SRGB_BLOCK = 168,
        ASTC_8x6_UNORM_BLOCK = 169,
        ASTC_8x6_SRGB_BLOCK = 170,
        ASTC_8x8_UNORM_BLOCK = 171,
        ASTC_8x8_SRGB_BLOCK = 172,
        ASTC_10x5_UNORM_BLOCK = 173,
        ASTC_10x5_SRGB_BLOCK = 174,
        ASTC_10x6_UNORM_BLOCK = 175,
        ASTC_10x6_SRGB_BLOCK = 176,
        ASTC_10x8_UNORM_BLOCK = 177,
        ASTC_10x8_SRGB_BLOCK = 178,
        ASTC_10x10_UNORM_BLOCK = 179,
        ASTC_10x10_SRGB_BLOCK = 180,
        ASTC_12x10_UNORM_BLOCK = 181,
        ASTC_12x10_SRGB_BLOCK = 182,
        ASTC_12x12_UNORM_BLOCK = 183,
        ASTC_12x12_SRGB_BLOCK = 184,
        G8B8G8R8_422_UNORM = 1000156000,
        B8G8R8G8_422_UNORM = 1000156001,
        G8_B8_R8_3PLANE_420_UNORM = 1000156002,
        G8_B8R8_2PLANE_420_UNORM = 1000156003,
        G8_B8_R8_3PLANE_422_UNORM = 1000156004,
        G8_B8R8_2PLANE_422_UNORM = 1000156005,
        G8_B8_R8_3PLANE_444_UNORM = 1000156006,
        R10X6_UNORM_PACK16 = 1000156007,
        R10X6G10X6_UNORM_2PACK16 = 1000156008,
        R10X6G10X6B10X6A10X6_UNORM_4PACK16 = 1000156009,
        G10X6B10X6G10X6R10X6_422_UNORM_4PACK16 = 1000156010,
        B10X6G10X6R10X6G10X6_422_UNORM_4PACK16 = 1000156011,
        G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16 = 1000156012,
        G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16 = 1000156013,
        G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16 = 1000156014,
        G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16 = 1000156015,
        G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16 = 1000156016,
        R12X4_UNORM_PACK16 = 1000156017,
        R12X4G12X4_UNORM_2PACK16 = 1000156018,
        R12X4G12X4B12X4A12X4_UNORM_4PACK16 = 1000156019,
        G12X4B12X4G12X4R12X4_422_UNORM_4PACK16 = 1000156020,
        B12X4G12X4R12X4G12X4_422_UNORM_4PACK16 = 1000156021,
        G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16 = 1000156022,
        G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16 = 1000156023,
        G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16 = 1000156024,
        G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16 = 1000156025,
        G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16 = 1000156026,
        G16B16G16R16_422_UNORM = 1000156027,
        B16G16R16G16_422_UNORM = 1000156028,
        G16_B16_R16_3PLANE_420_UNORM = 1000156029,
        G16_B16R16_2PLANE_420_UNORM = 1000156030,
        G16_B16_R16_3PLANE_422_UNORM = 1000156031,
        G16_B16R16_2PLANE_422_UNORM = 1000156032,
        G16_B16_R16_3PLANE_444_UNORM = 1000156033,
        PVRTC1_2BPP_UNORM_BLOCK_IMG = 1000054000,
        PVRTC1_4BPP_UNORM_BLOCK_IMG = 1000054001,
        PVRTC2_2BPP_UNORM_BLOCK_IMG = 1000054002,
        PVRTC2_4BPP_UNORM_BLOCK_IMG = 1000054003,
        PVRTC1_2BPP_SRGB_BLOCK_IMG = 1000054004,
        PVRTC1_4BPP_SRGB_BLOCK_IMG = 1000054005,
        PVRTC2_2BPP_SRGB_BLOCK_IMG = 1000054006,
        PVRTC2_4BPP_SRGB_BLOCK_IMG = 1000054007,
        ASTC_4x4_SFLOAT_BLOCK_EXT = 1000066000,
        ASTC_5x4_SFLOAT_BLOCK_EXT = 1000066001,
        ASTC_5x5_SFLOAT_BLOCK_EXT = 1000066002,
        ASTC_6x5_SFLOAT_BLOCK_EXT = 1000066003,
        ASTC_6x6_SFLOAT_BLOCK_EXT = 1000066004,
        ASTC_8x5_SFLOAT_BLOCK_EXT = 1000066005,
        ASTC_8x6_SFLOAT_BLOCK_EXT = 1000066006,
        ASTC_8x8_SFLOAT_BLOCK_EXT = 1000066007,
        ASTC_10x5_SFLOAT_BLOCK_EXT = 1000066008,
        ASTC_10x6_SFLOAT_BLOCK_EXT = 1000066009,
        ASTC_10x8_SFLOAT_BLOCK_EXT = 1000066010,
        ASTC_10x10_SFLOAT_BLOCK_EXT = 1000066011,
        ASTC_12x10_SFLOAT_BLOCK_EXT = 1000066012,
        ASTC_12x12_SFLOAT_BLOCK_EXT = 1000066013,
        A4R4G4B4_UNORM_PACK16_EXT = 1000340000,
        A4B4G4R4_UNORM_PACK16_EXT = 1000340001,
        G8B8G8R8_422_UNORM_KHR = G8B8G8R8_422_UNORM,
        B8G8R8G8_422_UNORM_KHR = B8G8R8G8_422_UNORM,
        G8_B8_R8_3PLANE_420_UNORM_KHR = G8_B8_R8_3PLANE_420_UNORM,
        G8_B8R8_2PLANE_420_UNORM_KHR = G8_B8R8_2PLANE_420_UNORM,
        G8_B8_R8_3PLANE_422_UNORM_KHR = G8_B8_R8_3PLANE_422_UNORM,
        G8_B8R8_2PLANE_422_UNORM_KHR = G8_B8R8_2PLANE_422_UNORM,
        G8_B8_R8_3PLANE_444_UNORM_KHR = G8_B8_R8_3PLANE_444_UNORM,
        R10X6_UNORM_PACK16_KHR = R10X6_UNORM_PACK16,
        R10X6G10X6_UNORM_2PACK16_KHR = R10X6G10X6_UNORM_2PACK16,
        R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR = R10X6G10X6B10X6A10X6_UNORM_4PACK16,
        G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR = G10X6B10X6G10X6R10X6_422_UNORM_4PACK16,
        B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR = B10X6G10X6R10X6G10X6_422_UNORM_4PACK16,
        G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR = G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16,
        G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR = G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16,
        G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR = G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16,
        G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR = G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16,
        G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR = G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16,
        R12X4_UNORM_PACK16_KHR = R12X4_UNORM_PACK16,
        R12X4G12X4_UNORM_2PACK16_KHR = R12X4G12X4_UNORM_2PACK16,
        R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR = R12X4G12X4B12X4A12X4_UNORM_4PACK16,
        G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR = G12X4B12X4G12X4R12X4_422_UNORM_4PACK16,
        B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR = B12X4G12X4R12X4G12X4_422_UNORM_4PACK16,
        G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR = G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16,
        G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR = G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16,
        G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR = G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16,
        G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR = G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16,
        G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR = G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16,
        G16B16G16R16_422_UNORM_KHR = G16B16G16R16_422_UNORM,
        B16G16R16G16_422_UNORM_KHR = B16G16R16G16_422_UNORM,
        G16_B16_R16_3PLANE_420_UNORM_KHR = G16_B16_R16_3PLANE_420_UNORM,
        G16_B16R16_2PLANE_420_UNORM_KHR = G16_B16R16_2PLANE_420_UNORM,
        G16_B16_R16_3PLANE_422_UNORM_KHR = G16_B16_R16_3PLANE_422_UNORM,
        G16_B16R16_2PLANE_422_UNORM_KHR = G16_B16R16_2PLANE_422_UNORM,
        G16_B16_R16_3PLANE_444_UNORM_KHR = G16_B16_R16_3PLANE_444_UNORM,
        MAX_ENUM = 0x7FFFFFFF
    };

    enum class ECompositeAlphaFlagBitsKHR {
        OPAQUE_BIT_KHR = 0x00000001,
        PRE_MULTIPLIED_BIT_KHR = 0x00000002,
        POST_MULTIPLIED_BIT_KHR = 0x00000004,
        INHERIT_BIT_KHR = 0x00000008,
        MAX_ENUM_KHR = 0x7FFFFFFF
    };
    typedef Flags CompositeAlphaFlagsKHR;
    typedef Flags SurfaceTransformFlagsKHR;

    enum class EPresentModeKHR {
        IMMEDIATE_KHR = 0,
        MAILBOX_KHR = 1,
        FIFO_KHR = 2,
        FIFO_RELAXED_KHR = 3,
        SHARED_DEMAND_REFRESH_KHR = 1000111000,
        SHARED_CONTINUOUS_REFRESH_KHR = 1000111001,
        MAX_ENUM_KHR = 0x7FFFFFFF
    } ;

    enum class EColorSpaceKHR {
        SRGB_NONLINEAR_KHR = 0,
        DISPLAY_P3_NONLINEAR_EXT = 1000104001,
        EXTENDED_SRGB_LINEAR_EXT = 1000104002,
        DISPLAY_P3_LINEAR_EXT = 1000104003,
        DCI_P3_NONLINEAR_EXT = 1000104004,
        BT709_LINEAR_EXT = 1000104005,
        BT709_NONLINEAR_EXT = 1000104006,
        BT2020_LINEAR_EXT = 1000104007,
        HDR10_ST2084_EXT = 1000104008,
        DOLBYVISION_EXT = 1000104009,
        HDR10_HLG_EXT = 1000104010,
        ADOBERGB_LINEAR_EXT = 1000104011,
        ADOBERGB_NONLINEAR_EXT = 1000104012,
        PASS_THROUGH_EXT = 1000104013,
        EXTENDED_SRGB_NONLINEAR_EXT = 1000104014,
        DISPLAY_NATIVE_AMD = 1000213000,
        VK_COLORSPACE_SRGB_NONLINEAR_KHR = SRGB_NONLINEAR_KHR,
        DCI_P3_LINEAR_EXT = DISPLAY_P3_LINEAR_EXT,
        MAX_ENUM_KHR = 0x7FFFFFFF
    } ;

    enum class EMemoryPropertyFlagBits {
        DEVICE_LOCAL_BIT = 0x00000001,
        HOST_VISIBLE_BIT = 0x00000002,
        HOST_COHERENT_BIT = 0x00000004,
        HOST_CACHED_BIT = 0x00000008,
        LAZILY_ALLOCATED_BIT = 0x00000010,
        PROTECTED_BIT = 0x00000020,
        DEVICE_COHERENT_BIT_AMD = 0x00000040,
        DEVICE_UNCACHED_BIT_AMD = 0x00000080,
        MAX_ENUM = 0x7FFFFFFF
    } ;
    typedef Flags MemoryPropertyFlags;

    enum class ESurfaceTransformFlagBitsKHR {
        IDENTITY_BIT_KHR = 0x00000001,
        ROTATE_90_BIT_KHR = 0x00000002,
        ROTATE_180_BIT_KHR = 0x00000004,
        ROTATE_270_BIT_KHR = 0x00000008,
        HORIZONTAL_MIRROR_BIT_KHR = 0x00000010,
        HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR = 0x00000020,
        HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR = 0x00000040,
        HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR = 0x00000080,
        INHERIT_BIT_KHR = 0x00000100,
        MAX_ENUM_KHR = 0x7FFFFFFF
    };

    enum class ESampleCountFlagBits {
        _1_BIT = 0x00000001,
        _2_BIT = 0x00000002,
        _4_BIT = 0x00000004,
        _8_BIT = 0x00000008,
        _16_BIT = 0x00000010,
        _32_BIT = 0x00000020,
        _64_BIT = 0x00000040,
        MAX_ENUM = 0x7FFFFFFF
    };
    typedef Flags SampleCountFlags;

    enum class EPhysicalDeviceType {
        OTHER = 0,
        INTEGRATED_GPU = 1,
        DISCRETE_GPU = 2,
        VIRTUAL_GPU = 3,
        CPU = 4,
        MAX_ENUM = 0x7FFFFFFF
    } ;

    //this is copy form VkPrimitiveTopology
    enum EPrimitiveTopology {
        POINT_LIST = 0,
        LINE_LIST = 1,
        LINE_STRIP = 2,
        TRIANGLE_LIST = 3,
        TRIANGLE_STRIP = 4,
        TRIANGLE_FAN = 5,
        LINE_LIST_WITH_ADJACENCY = 6,
        LINE_STRIP_WITH_ADJACENCY = 7,
        TRIANGLE_LIST_WITH_ADJACENCY = 8,
        TRIANGLE_STRIP_WITH_ADJACENCY = 9,
        TOPOLOGY_PATCH_LIST = 10,
        TOPOLOGY_MAX_ENUM = 0x7FFFFFFF
    };

    enum class EMemoryHeapFlagBits {
        DEVICE_LOCAL_BIT = 0x00000001,
        MULTI_INSTANCE_BIT = 0x00000002,
        MULTI_INSTANCE_BIT_KHR = MULTI_INSTANCE_BIT,
        MAX_ENUM = 0x7FFFFFFF
    } ;
    typedef Flags MemoryHeapFlags;


    //the same as VkFrontFace
    enum class EFrontFace {
        COUNTER_CLOCKWISE = 0,
        CLOCKWISE = 1,
        MAX_ENUM = 0x7FFFFFFF
    };

    //the same as VkVertexInputRate
    enum class EVertexInputRate {
        VERTEX = 0,
        INSTANCE = 1,
        MAX_ENUM = 0x7FFFFFFF
    };

    //the same as VkStencilOp
    enum class EStencilOp {
        KEEP = 0,
        ZERO = 1,
        REPLACE = 2,
        INCREMENT_AND_CLAMP = 3,
        DECREMENT_AND_CLAMP = 4,
        INVERT = 5,
        INCREMENT_AND_WRAP = 6,
        DECREMENT_AND_WRAP = 7,
        MAX_ENUM = 0x7FFFFFFF
    };

    enum class ELogicOp {
        CLEAR = 0,
        AND = 1,
        AND_REVERSE = 2,
        COPY = 3,
        AND_INVERTED = 4,
        NO_OP = 5,
        XOR = 6,
        OR = 7,
        NOR = 8,
        EQUIVALENT = 9,
        INVERT = 10,
        OR_REVERSE = 11,
        COPY_INVERTED = 12,
        OR_INVERTED = 13,
        NAND = 14,
        SET = 15,
        MAX_ENUM = 0x7FFFFFFF
    };

    enum class EPolygonMode {
        FILL = 0,
        LINE = 1,
        POINT = 2,
        FILL_RECTANGLE_NV = 1000153000,
        MAX_ENUM = 0x7FFFFFFF
    };

    enum class EBlendFactor {
        ZERO = 0,
        ONE = 1,
        SRC_COLOR = 2,
        ONE_MINUS_SRC_COLOR = 3,
        DST_COLOR = 4,
        ONE_MINUS_DST_COLOR = 5,
        SRC_ALPHA = 6,
        ONE_MINUS_SRC_ALPHA = 7,
        DST_ALPHA = 8,
        ONE_MINUS_DST_ALPHA = 9,
        CONSTANT_COLOR = 10,
        ONE_MINUS_CONSTANT_COLOR = 11,
        CONSTANT_ALPHA = 12,
        ONE_MINUS_CONSTANT_ALPHA = 13,
        SRC_ALPHA_SATURATE = 14,
        SRC1_COLOR = 15,
        ONE_MINUS_SRC1_COLOR = 16,
        SRC1_ALPHA = 17,
        ONE_MINUS_SRC1_ALPHA = 18,
        MAX_ENUM = 0x7FFFFFFF
    };

    enum class EBlendOp {
        ADD = 0,
        SUBTRACT = 1,
        REVERSE_SUBTRACT = 2,
        MIN = 3,
        MAX = 4,
        ZERO_EXT = 1000148000,
        SRC_EXT = 1000148001,
        DST_EXT = 1000148002,
        SRC_OVER_EXT = 1000148003,
        DST_OVER_EXT = 1000148004,
        SRC_IN_EXT = 1000148005,
        DST_IN_EXT = 1000148006,
        SRC_OUT_EXT = 1000148007,
        DST_OUT_EXT = 1000148008,
        SRC_ATOP_EXT = 1000148009,
        DST_ATOP_EXT = 1000148010,
        XOR_EXT = 1000148011,
        MULTIPLY_EXT = 1000148012,
        SCREEN_EXT = 1000148013,
        OVERLAY_EXT = 1000148014,
        DARKEN_EXT = 1000148015,
        LIGHTEN_EXT = 1000148016,
        COLORDODGE_EXT = 1000148017,
        COLORBURN_EXT = 1000148018,
        HARDLIGHT_EXT = 1000148019,
        SOFTLIGHT_EXT = 1000148020,
        DIFFERENCE_EXT = 1000148021,
        EXCLUSION_EXT = 1000148022,
        INVERT_EXT = 1000148023,
        INVERT_RGB_EXT = 1000148024,
        LINEARDODGE_EXT = 1000148025,
        LINEARBURN_EXT = 1000148026,
        VIVIDLIGHT_EXT = 1000148027,
        LINEARLIGHT_EXT = 1000148028,
        PINLIGHT_EXT = 1000148029,
        HARDMIX_EXT = 1000148030,
        HSL_HUE_EXT = 1000148031,
        HSL_SATURATION_EXT = 1000148032,
        HSL_COLOR_EXT = 1000148033,
        HSL_LUMINOSITY_EXT = 1000148034,
        PLUS_EXT = 1000148035,
        PLUS_CLAMPED_EXT = 1000148036,
        PLUS_CLAMPED_ALPHA_EXT = 1000148037,
        PLUS_DARKER_EXT = 1000148038,
        MINUS_EXT = 1000148039,
        MINUS_CLAMPED_EXT = 1000148040,
        CONTRAST_EXT = 1000148041,
        INVERT_OVG_EXT = 1000148042,
        RED_EXT = 1000148043,
        GREEN_EXT = 1000148044,
        BLUE_EXT = 1000148045,
        MAX_ENUM = 0x7FFFFFFF
    };

    enum class EColorComponentFlagBits {
        R_BIT = 0x00000001,
        G_BIT = 0x00000002,
        B_BIT = 0x00000004,
        A_BIT = 0x00000008,
        MAX_ENUM = 0x7FFFFFFF
    };
    using ColorComponentFlags = Flags;

    //Must the same as VkDynamicState
    enum class EDynamicState {
        VIEWPORT = 0,
        SCISSOR = 1,
        LINE_WIDTH = 2,
        DEPTH_BIAS = 3,
        BLEND_CONSTANTS = 4,
        DEPTH_BOUNDS = 5,
        STENCIL_COMPARE_MASK = 6,
        STENCIL_WRITE_MASK = 7,
        STENCIL_REFERENCE = 8,
        VIEWPORT_W_SCALING_NV = 1000087000,
        DISCARD_RECTANGLE_EXT = 1000099000,
        SAMPLE_LOCATIONS_EXT = 1000143000,
        RAY_TRACING_PIPELINE_STACK_SIZE_KHR = 1000347000,
        VIEWPORT_SHADING_RATE_PALETTE_NV = 1000164004,
        VIEWPORT_COARSE_SAMPLE_ORDER_NV = 1000164006,
        EXCLUSIVE_SCISSOR_NV = 1000205001,
        FRAGMENT_SHADING_RATE_KHR = 1000226000,
        LINE_STIPPLE_EXT = 1000259000,
        CULL_MODE_EXT = 1000267000,
        FRONT_FACE_EXT = 1000267001,
        PRIMITIVE_TOPOLOGY_EXT = 1000267002,
        VIEWPORT_WITH_COUNT_EXT = 1000267003,
        SCISSOR_WITH_COUNT_EXT = 1000267004,
        VERTEX_INPUT_BINDING_STRIDE_EXT = 1000267005,
        DEPTH_TEST_ENABLE_EXT = 1000267006,
        DEPTH_WRITE_ENABLE_EXT = 1000267007,
        DEPTH_COMPARE_OP_EXT = 1000267008,
        DEPTH_BOUNDS_TEST_ENABLE_EXT = 1000267009,
        STENCIL_TEST_ENABLE_EXT = 1000267010,
        STENCIL_OP_EXT = 1000267011,
        MAX_ENUM = 0x7FFFFFFF
    };

    enum class EQueueFlagBits {
        GRAPHICS_BIT = 0x00000001,
        COMPUTE_BIT = 0x00000002,
        TRANSFER_BIT = 0x00000004,
        SPARSE_BINDING_BIT = 0x00000008,
        PROTECTED_BIT = 0x00000010,
        MAX_ENUM = 0x7FFFFFFF
    } ;
    typedef Flags QueueFlags;
    typedef Flags DeviceCreateFlags;

    enum class EImageUsageFlagBits {
        TRANSFER_SRC_BIT = 0x00000001,
        TRANSFER_DST_BIT = 0x00000002,
        SAMPLED_BIT = 0x00000004,
        STORAGE_BIT = 0x00000008,
        COLOR_ATTACHMENT_BIT = 0x00000010,
        DEPTH_STENCIL_ATTACHMENT_BIT = 0x00000020,
        TRANSIENT_ATTACHMENT_BIT = 0x00000040,
        INPUT_ATTACHMENT_BIT = 0x00000080,
        SHADING_RATE_IMAGE_BIT_NV = 0x00000100,
        VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT = 0x00000200,
        VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR = SHADING_RATE_IMAGE_BIT_NV,
        MAX_ENUM = 0x7FFFFFFF
    } ;
    typedef Flags ImageUsageFlags;
    typedef Flags InstanceCreateFlags;

    /////////////////////////////////////////////////////////////

    

    struct ApplicationInfo {
        std::string        applicationName;
        uint32_t           applicationVersion;
        std::string        engineName;
        uint32_t           engineVersion;
        uint32_t           apiVersion;
    };

    struct HGraphicsPipeline {
    public:
        uint64_t backend = 0;
    };

    //struct VkVertexInputAttributeDescription {
    //    uint32_t    location;
    //    uint32_t    binding;
    //    RHIFormat    format;
    //    uint32_t    offset;
    //} VkVertexInputAttributeDescription;

	class VertexFormat {

	};

    enum class IndexType {


    };

    class Texture {

    };

    class HBuffer {
        void* handle_;
    };

    struct HSurface {
        uint64_t backend = 0;
    };

    struct PhysicalDeviceLimits {
        uint32_t              maxImageDimension1D;
        uint32_t              maxImageDimension2D;
        uint32_t              maxImageDimension3D;
        uint32_t              maxImageDimensionCube;
        uint32_t              maxImageArrayLayers;
        uint32_t              maxTexelBufferElements;
        uint32_t              maxUniformBufferRange;
        uint32_t              maxStorageBufferRange;
        uint32_t              maxPushConstantsSize;
        uint32_t              maxMemoryAllocationCount;
        uint32_t              maxSamplerAllocationCount;
        DeviceSize            bufferImageGranularity;
        DeviceSize            sparseAddressSpaceSize;
        uint32_t              maxBoundDescriptorSets;
        uint32_t              maxPerStageDescriptorSamplers;
        uint32_t              maxPerStageDescriptorUniformBuffers;
        uint32_t              maxPerStageDescriptorStorageBuffers;
        uint32_t              maxPerStageDescriptorSampledImages;
        uint32_t              maxPerStageDescriptorStorageImages;
        uint32_t              maxPerStageDescriptorInputAttachments;
        uint32_t              maxPerStageResources;
        uint32_t              maxDescriptorSetSamplers;
        uint32_t              maxDescriptorSetUniformBuffers;
        uint32_t              maxDescriptorSetUniformBuffersDynamic;
        uint32_t              maxDescriptorSetStorageBuffers;
        uint32_t              maxDescriptorSetStorageBuffersDynamic;
        uint32_t              maxDescriptorSetSampledImages;
        uint32_t              maxDescriptorSetStorageImages;
        uint32_t              maxDescriptorSetInputAttachments;
        uint32_t              maxVertexInputAttributes;
        uint32_t              maxVertexInputBindings;
        uint32_t              maxVertexInputAttributeOffset;
        uint32_t              maxVertexInputBindingStride;
        uint32_t              maxVertexOutputComponents;
        uint32_t              maxTessellationGenerationLevel;
        uint32_t              maxTessellationPatchSize;
        uint32_t              maxTessellationControlPerVertexInputComponents;
        uint32_t              maxTessellationControlPerVertexOutputComponents;
        uint32_t              maxTessellationControlPerPatchOutputComponents;
        uint32_t              maxTessellationControlTotalOutputComponents;
        uint32_t              maxTessellationEvaluationInputComponents;
        uint32_t              maxTessellationEvaluationOutputComponents;
        uint32_t              maxGeometryShaderInvocations;
        uint32_t              maxGeometryInputComponents;
        uint32_t              maxGeometryOutputComponents;
        uint32_t              maxGeometryOutputVertices;
        uint32_t              maxGeometryTotalOutputComponents;
        uint32_t              maxFragmentInputComponents;
        uint32_t              maxFragmentOutputAttachments;
        uint32_t              maxFragmentDualSrcAttachments;
        uint32_t              maxFragmentCombinedOutputResources;
        uint32_t              maxComputeSharedMemorySize;
        uint32_t              maxComputeWorkGroupCount[3];
        uint32_t              maxComputeWorkGroupInvocations;
        uint32_t              maxComputeWorkGroupSize[3];
        uint32_t              subPixelPrecisionBits;
        uint32_t              subTexelPrecisionBits;
        uint32_t              mipmapPrecisionBits;
        uint32_t              maxDrawIndexedIndexValue;
        uint32_t              maxDrawIndirectCount;
        float                 maxSamplerLodBias;
        float                 maxSamplerAnisotropy;
        uint32_t              maxViewports;
        uint32_t              maxViewportDimensions[2];
        float                 viewportBoundsRange[2];
        uint32_t              viewportSubPixelBits;
        size_t                minMemoryMapAlignment;
        DeviceSize            minTexelBufferOffsetAlignment;
        DeviceSize            minUniformBufferOffsetAlignment;
        DeviceSize            minStorageBufferOffsetAlignment;
        int32_t               minTexelOffset;
        uint32_t              maxTexelOffset;
        int32_t               minTexelGatherOffset;
        uint32_t              maxTexelGatherOffset;
        float                 minInterpolationOffset;
        float                 maxInterpolationOffset;
        uint32_t              subPixelInterpolationOffsetBits;
        uint32_t              maxFramebufferWidth;
        uint32_t              maxFramebufferHeight;
        uint32_t              maxFramebufferLayers;
        SampleCountFlags      framebufferColorSampleCounts;
        SampleCountFlags      framebufferDepthSampleCounts;
        SampleCountFlags      framebufferStencilSampleCounts;
        SampleCountFlags      framebufferNoAttachmentsSampleCounts;
        uint32_t              maxColorAttachments;
        SampleCountFlags      sampledImageColorSampleCounts;
        SampleCountFlags      sampledImageIntegerSampleCounts;
        SampleCountFlags      sampledImageDepthSampleCounts;
        SampleCountFlags      sampledImageStencilSampleCounts;
        SampleCountFlags      storageImageSampleCounts;
        uint32_t              maxSampleMaskWords;
        Bool32                timestampComputeAndGraphics;
        float                 timestampPeriod;
        uint32_t              maxClipDistances;
        uint32_t              maxCullDistances;
        uint32_t              maxCombinedClipAndCullDistances;
        uint32_t              discreteQueuePriorities;
        float                 pointSizeRange[2];
        float                 lineWidthRange[2];
        float                 pointSizeGranularity;
        float                 lineWidthGranularity;
        Bool32                strictLines;
        Bool32                standardSampleLocations;
        DeviceSize            optimalBufferCopyOffsetAlignment;
        DeviceSize            optimalBufferCopyRowPitchAlignment;
        DeviceSize            nonCoherentAtomSize;
    };

    typedef struct PhysicalDeviceSparseProperties {
        Bool32    residencyStandard2DBlockShape;
        Bool32    residencyStandard2DMultisampleBlockShape;
        Bool32    residencyStandard3DBlockShape;
        Bool32    residencyAlignedMipSize;
        Bool32    residencyNonResidentStrict;
    } PhysicalDeviceSparseProperties;

    typedef struct MemoryType {
        MemoryPropertyFlags      propertyFlags;
        uint32_t                 heapIndex;
    } MemoryType;

    typedef struct MemoryHeap {
        DeviceSize         size;
        MemoryHeapFlags    flags;
    } MemoryHeap;

    struct PhysicalDeviceMemoryProperties {
        uint32_t        memoryTypeCount;
        MemoryType      memoryTypes[MAX_MEMORY_TYPES];
        uint32_t        memoryHeapCount;
        MemoryHeap      memoryHeaps[MAX_MEMORY_HEAPS];
    } ;

    typedef struct PhysicalDeviceProperties {
        uint32_t                            apiVersion;
        uint32_t                            driverVersion;
        uint32_t                            vendorID;
        uint32_t                            deviceID;
        EPhysicalDeviceType                 deviceType;
        char                                deviceName[MAX_PHYSICAL_DEVICE_NAME_SIZE];
        uint8_t                             pipelineCacheUUID[UUID_SIZE];
        PhysicalDeviceLimits                limits;
        PhysicalDeviceSparseProperties      sparseProperties;
    } PhysicalDeviceProperties;

    DEFINE_HANDLE(HPhysicalDevice)


    typedef struct PhysicalDeviceFeatures {
        Bool32    robustBufferAccess;
        Bool32    fullDrawIndexUint32;
        Bool32    imageCubeArray;
        Bool32    independentBlend;
        Bool32    geometryShader;
        Bool32    tessellationShader;
        Bool32    sampleRateShading;
        Bool32    dualSrcBlend;
        Bool32    logicOp;
        Bool32    multiDrawIndirect;
        Bool32    drawIndirectFirstInstance;
        Bool32    depthClamp;
        Bool32    depthBiasClamp;
        Bool32    fillModeNonSolid;
        Bool32    depthBounds;
        Bool32    wideLines;
        Bool32    largePoints;
        Bool32    alphaToOne;
        Bool32    multiViewport;
        Bool32    samplerAnisotropy;
        Bool32    textureCompressionETC2;
        Bool32    textureCompressionASTC_LDR;
        Bool32    textureCompressionBC;
        Bool32    occlusionQueryPrecise;
        Bool32    pipelineStatisticsQuery;
        Bool32    vertexPipelineStoresAndAtomics;
        Bool32    fragmentStoresAndAtomics;
        Bool32    shaderTessellationAndGeometryPointSize;
        Bool32    shaderImageGatherExtended;
        Bool32    shaderStorageImageExtendedFormats;
        Bool32    shaderStorageImageMultisample;
        Bool32    shaderStorageImageReadWithoutFormat;
        Bool32    shaderStorageImageWriteWithoutFormat;
        Bool32    shaderUniformBufferArrayDynamicIndexing;
        Bool32    shaderSampledImageArrayDynamicIndexing;
        Bool32    shaderStorageBufferArrayDynamicIndexing;
        Bool32    shaderStorageImageArrayDynamicIndexing;
        Bool32    shaderClipDistance;
        Bool32    shaderCullDistance;
        Bool32    shaderFloat64;
        Bool32    shaderInt64;
        Bool32    shaderInt16;
        Bool32    shaderResourceResidency;
        Bool32    shaderResourceMinLod;
        Bool32    sparseBinding;
        Bool32    sparseResidencyBuffer;
        Bool32    sparseResidencyImage2D;
        Bool32    sparseResidencyImage3D;
        Bool32    sparseResidency2Samples;
        Bool32    sparseResidency4Samples;
        Bool32    sparseResidency8Samples;
        Bool32    sparseResidency16Samples;
        Bool32    sparseResidencyAliased;
        Bool32    variableMultisampleRate;
        Bool32    inheritedQueries;
    } PhysicalDeviceFeatures;

    typedef struct Extent2D {
        uint32_t    width;
        uint32_t    height;
    } Extent2D;

    typedef struct Extent3D {
        uint32_t    width;
        uint32_t    height;
        uint32_t    depth;
    } Extent3D;

    typedef struct Offset2D {
        int32_t    x;
        int32_t    y;
    } Offset2D;

    typedef struct Offset3D {
        int32_t    x;
        int32_t    y;
        int32_t    z;
    } Offset3D;

    typedef struct Rect2D {
        Offset2D    offset;
        Extent2D    extent;
    } Rect2D;

    typedef struct SurfaceCapabilitiesKHR {
        uint32_t                         minImageCount;
        uint32_t                         maxImageCount;
        Extent2D                         currentExtent;
        Extent2D                         minImageExtent;
        Extent2D                         maxImageExtent;
        uint32_t                         maxImageArrayLayers;
        SurfaceTransformFlagsKHR         supportedTransforms;
        ESurfaceTransformFlagBitsKHR     currentTransform;
        CompositeAlphaFlagsKHR           supportedCompositeAlpha;
        ImageUsageFlags                  supportedUsageFlags;
    } SurfaceCapabilitiesKHR;

    typedef struct SurfaceFormatKHR {
        EFormat           format;
        EColorSpaceKHR    colorSpace;
    } SurfaceFormatKHR;

    typedef struct QueueFamilyProperties {
        QueueFlags      queueFlags;
        uint32_t        queueCount;
        uint32_t        timestampValidBits;
        Extent3D        minImageTransferGranularity;
    } QueueFamilyProperties;

    struct ExtensionProperties {
        char        extensionName[MAX_EXTENSION_NAME_SIZE];
        uint32_t    specVersion;
    } ;

    struct DeviceInfo {
        HPhysicalDevice					    device;
        PhysicalDeviceProperties			props;
        PhysicalDeviceMemoryProperties	    memProps;
        PhysicalDeviceFeatures			    features;
        SurfaceCapabilitiesKHR			    surfaceCaps;
        std::vector<SurfaceFormatKHR>		surfaceFormats;
        std::vector<EPresentModeKHR>	    presentModes;
        std::vector<QueueFamilyProperties>	queueFamilyProps;
        std::vector<ExtensionProperties>    extensionProps;
    };

    using DevicesInfo = std::vector<DeviceInfo>;

    //屏蔽掉不同操作系统的窗口句柄的不同
    struct HWindowHandle {
        uint64_t backend;
    };

    struct HDrawCall{
    public:
        uint32_t  indexCount;
        uint32_t  instanceCount;
        uint32_t  firstIndex;
        int32_t   vertexOffset;
        uint32_t  firstInstance;
    };

    struct ShaderCreateInfo {
        EShaderStageFlagBits    stage;
        std::string             name;   
        std::string             mainFuncName;
    };

    struct RHIShaderProgramCreateInfo {
        std::string dir;
        std::vector<ShaderCreateInfo> infos;
        //glsl global uniform init

        //std::vector<Uniform> uniformInfos_;
        std::string shaderProgramName; //used for map key
    };

    struct HShaderModule {
        EShaderStageFlagBits stage;
        void* backend;
        std::string name;
        std::string mainFuncName;
    };

    struct VertexInputBindingDescription {
        uint32_t             binding;
        uint32_t             stride;
        EVertexInputRate     inputRate;
    };

    struct VertexInputAttributeDescription {
        uint32_t location = 0;
            
        uint32_t binding = 0;

        //vulkan: can be cast  to VkFormat
        //opengl, trans to pair (count, type) 
        EFormat format;

        //单次出现相对起始位置的偏移量
        uint32_t offset;

        //only for opengl!
        uint32_t stride;

        //only for opengl!
        int attributeDivisor = 0;
    };

    //for opengl : bindings is not needed
    //顶点布局格式 
    struct VertexLayout {
        std::vector<VertexInputBindingDescription> bindings;
        std::vector<VertexInputAttributeDescription> attribs;
    };

    struct DrawTopology {
        EPrimitiveTopology topo = EPrimitiveTopology::TRIANGLE_LIST;
        uint32_t primitiveRestartEnable = false;
    };

    struct HDevice {
        void* backend = 0;
    };

    struct PhysicalDevice {
        void* backend;
    };

    //same as Vulkan
    struct Viewport {
        float    x;
        float    y;
        float    width;
        float    height;
        float    minDepth;
        float    maxDepth;
    };

    struct ViewportState {
        std::vector<Viewport>                      viewports;
        std::vector<Rect2D>                        scissors;
    };

    struct RasterizationState {
        Bool32                                    depthClampEnable;
        Bool32                                    rasterizerDiscardEnable;
        EPolygonMode                              polygonMode;
        CullModeFlags                             cullMode;
        EFrontFace                                frontFace;
        Bool32                                    depthBiasEnable;
        float                                     depthBiasConstantFactor;
        float                                     depthBiasClamp;
        float                                     depthBiasSlopeFactor;
        float                                     lineWidth;
    };

    struct MultiSampleState {
        ESampleCountFlagBits                      rasterizationSamples;
        Bool32                                    sampleShadingEnable;
        float                                     minSampleShading;
        SampleMask                                SampleMask;
        Bool32                                    alphaToCoverageEnable;
        Bool32                                    alphaToOneEnable;
    };

    struct StencilOpState {
        EStencilOp     failOp;
        EStencilOp     passOp;
        EStencilOp     depthFailOp;
        ECompareOp     compareOp;
        uint32_t       compareMask;
        uint32_t       writeMask;
        uint32_t       reference;
    };

    struct PipelineDepthStencilState {
        Bool32                                    depthTestEnable;
        Bool32                                    depthWriteEnable;
        ECompareOp                                depthCompareOp;
        Bool32                                    depthBoundsTestEnable;
        Bool32                                    stencilTestEnable;
        StencilOpState                            front;
        StencilOpState                            back;
        float                                     minDepthBounds;
        float                                     maxDepthBounds;
    };

    //must be the same as VkPipelineColorBlendAttachementState
    struct PipelineColorBlendAttachmentState {
        Bool32                  blendEnable;
        EBlendFactor            srcColorBlendFactor;
        EBlendFactor            dstColorBlendFactor;
        EBlendOp                colorBlendOp;
        EBlendFactor            srcAlphaBlendFactor;
        EBlendFactor            dstAlphaBlendFactor;
        EBlendOp                alphaBlendOp;
        ColorComponentFlags     colorWriteMask;
    };

    struct PipelineColorBlendState {
        Bool32                                           logicOpEnable;
        ELogicOp                                         logicOp;
        std::vector<PipelineColorBlendAttachmentState>   attachments;
        float                                            blendConstants[4];
    };

    struct DynamicStates {
        std::vector<EDynamicState> dynamicStates;
    };

    struct HDescriptorSetLayout {
        uint64_t backend;
    };

    using ShaderStageFlags = Flags;
    struct PushConstantRange {
        ShaderStageFlags      stageFlags;
        uint32_t              offset;
        uint32_t              size;
    };
    
    //描述符布局和常量
    struct PipelineLayout {
        std::vector<HDescriptorSetLayout>  setLayouts;
        std::vector<PushConstantRange>     pushConstantRanges;
    };

    struct HPipelineLayout {
        uint64_t backend = 0;
    };

    struct HRenderPass {
        uint64_t backend = 0;
    };

    struct HContext {
        void* backend = nullptr;
    };
}
}

#endif