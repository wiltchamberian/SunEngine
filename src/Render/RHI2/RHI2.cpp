#include "RHI2.h"

namespace Sun {

    namespace rhi {

        IQueue::~IQueue() {

        }

        ISurface::~ISurface() {

        }

        IDeviceMemory::~IDeviceMemory() {

        }

        IBuffer::~IBuffer() {

        }

        IFrameBuffer::~IFrameBuffer() {

        }

        IShaderModule::~IShaderModule() {

        }

        IPipelineLayout::~IPipelineLayout() {

        }

        IRenderPass::~IRenderPass() {

        }

        IDescriptorSetLayout::~IDescriptorSetLayout() {

        }

        IPipeline::~IPipeline() {

        }

        ICommandPool::~ICommandPool() {

        }

        IImageView::~IImageView() {

        }

        IImage::~IImage() {

        }

        ISwapchain::~ISwapchain() {

        }

        IDevice::~IDevice() {

        }

        vk::Format IPhysicalDevice::chooseSupportedFormat(const std::vector<vk::Format>& formats,
            const vk::ImageTiling& tiling, vk::FormatFeatureFlags features) {

            for (const vk::Format& format : formats) {
                vk::FormatProperties props = this->getFormatProperties(format);
                if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
                    return format;
                }
                else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features) {
                    return format;
                }
            }
            throw std::runtime_error("failed to find supported format!");
            return vk::Format();
        }

        RHI2::~RHI2() {

        }

    }
}