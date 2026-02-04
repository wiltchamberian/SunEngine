#ifndef __SUN_RHI2_H
#define __SUN_RHI2_H


#include "RHI2Define.h"
#include "vulkan/utils/utils.hpp"

namespace Sun {

namespace rhi {

    class IQueue {
    public:
        virtual ~IQueue();
    };

    class ISurface {
    public:
        virtual ~ISurface();
    };

    class IDeviceMemory {
    public:
        virtual ~IDeviceMemory();
    };

    class IBuffer {
    public:
        virtual ~IBuffer();
    };

    class IFrameBuffer {
    public:
        virtual ~IFrameBuffer();
    };

    class IDescriptorSetLayout {
    public:
        virtual ~IDescriptorSetLayout();
    };

    class ISemaphore {

    };

    class IImageView {
    public:
        virtual ~IImageView();
    };

    class IImage {
    public:
        virtual ~IImage();
        //输入ci中的image参数无需填写，即使填写了也不生效，该参数默认会使用当前image替换
        virtual RHIPtr<IImageView> createImageView(const vk::ImageViewCreateInfo& ci) = 0;
    protected:
    };

    class ISwapchain {
    public:
        virtual ~ISwapchain();
        virtual std::vector<RHIPtr<IImage>> getImagesKHR() = 0;
    protected:
        struct Entry
        {
            RHIPtr<IImage>     image;
            RHIPtr<IImageView> imageView;
            // be aware semaphore index may not match active image index
            RHIPtr<ISemaphore> readSemaphore{};
            RHIPtr<ISemaphore> writtenSemaphore{};
        };
        std::vector< Entry> entries_;
    };

    class ICommandBuffer {

    };

    class ICommandPool {
    public:
        virtual ~ICommandPool();
        virtual std::vector<RHIPtr<ICommandBuffer>> allocateCommandBuffers(const vk::CommandBufferAllocateInfo& ai) = 0;
    };

    class IDevice {
    public:
        virtual ~IDevice();
        virtual RHIPtr<ICommandPool> createCommandPool(const vk::CommandPoolCreateInfo& ci) = 0;

        virtual std::vector<RHIPtr<ICommandBuffer>> allocateCommandBuffers(const vk::CommandBufferAllocateInfo& ai) = 0;
              
        virtual RHIPtr<ISwapchain> createSwapChain(const vk::SwapchainCreateInfoKHR& ci) = 0;

        virtual RHIPtr<IImage> createImage(const vk::ImageCreateInfo& ci) = 0;
            
        virtual RHIPtr<IImageView> createImageView(const vk::ImageViewCreateInfo& ci) = 0;

        virtual RHIPtr<IRenderPass> createRenderPass(const vk::RenderPassCreateInfo& createInfo) = 0;

        virtual RHIPtr<IPipeline> createGraphicsPipeline(const vk::GraphicsPipelineCreateInfo& ci) = 0;
        
        virtual RHIPtr<IDescriptorSetLayout> createDescriptorSetLayout(const vk::DescriptorSetLayoutCreateInfo& ci) = 0;
        
        virtual RHIPtr<IPipelineLayout> createPipelineLayout(const vk::PipelineLayoutCreateInfo& ci) = 0;
        
        virtual RHIPtr<IShaderModule> createShaderModule(const vk::ShaderModuleCreateInfo& ci) = 0;

        virtual RHIPtr<IFrameBuffer> createFrameBuffer(const vk::FramebufferCreateInfo& ci) = 0;
        
        virtual RHIPtr<IBuffer> createBuffer(const vk::BufferCreateInfo& ci) = 0;
        
        virtual RHIPtr<IDeviceMemory> allocMemory(const vk::MemoryAllocateInfo& ai) = 0;
        
        virtual RHIPtr<IQueue> getQueue(uint32_t queueFamilyIndex, uint32_t queueIndex) = 0;
    };

    class IPhysicalDevice {
    public:
        virtual std::vector<vk::QueueFamilyProperties>  getQueueFamilyProperties() = 0 ;
        virtual RHIPtr<IDevice> createDevice(const vk::DeviceCreateInfo& ci) = 0;
        
        virtual VkBool32 getSurfaceSupportKHR(uint32_t queueFamilyIndex, RHIPtr<ISurface> surface) = 0;
        virtual std::vector<vk::SurfaceFormatKHR> getSurfaceFormatsKHR(RHIPtr<ISurface> surface) = 0;
        virtual vk::SurfaceCapabilitiesKHR getSurfaceCapabilitiesKHR(RHIPtr<ISurface> surface) = 0;
        virtual std::vector<vk::PresentModeKHR> getSurfacePresentModesKHR(RHIPtr<ISurface> surface) = 0;
       
        virtual std::vector<vk::ExtensionProperties> enumerateDeviceExtensionProperties() = 0;

        virtual vk::PhysicalDeviceMemoryProperties getMemoryProperties() = 0;
        virtual vk::PhysicalDeviceProperties getProperties() = 0;
        virtual vk::PhysicalDeviceFeatures getFeatures() = 0;
        virtual vk::FormatProperties getFormatProperties(vk::Format format) = 0;

    public: //non virtual functions
        //从formats中选择出tiling属性 涵盖features的 format
        vk::Format chooseSupportedFormat(const std::vector<vk::Format>& formats,
            const vk::ImageTiling& tiling, vk::FormatFeatureFlags features);
    };

    class IContext {
    public:
        virtual void destroy() = 0;

        //virtual vk::DebugUtilsMessengerEXT createDebugUtilsMessengerEXT() = 0;
        //virtual void destroyDebugUtilsMessengerEXT(vk::DebugUtilsMessengerEXT) = 0;

        virtual std::vector<RHIPtr<IPhysicalDevice>> enumeratePhysicalDevices() = 0;
       
    };

    class RHI2 {
    public:
        virtual ~RHI2();
        virtual RHIPtr<IContext> createContext(const vk::InstanceCreateInfo& ci) = 0;
    };

}

}

#endif