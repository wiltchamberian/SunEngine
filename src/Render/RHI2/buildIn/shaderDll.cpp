#include "vulkan_core.h"
#include "shaderDll.h"

VkAttachmentDescription format = {

    0,
    VkFormat::eUndefined,
    VkSampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eDontCare,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::eDepthStencilAttachmentOptimal


}




