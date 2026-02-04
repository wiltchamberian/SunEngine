
#ifdef SHADER_EXPORTS
#define SHADER_API __declspec(dllexport)
#else
#define SHADER_API __declspec(dllimport)
#endif

#include "vulkan_core.h"

extern SHADER_API  VkAttachmentDescription format;
