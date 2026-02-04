#ifndef __SUN_RHI2_VULKAN_H
#define __SUN_RHI2_VULKAN_H

#include "RHI2.h"

namespace Sun {
    namespace rhi {

        class VulkanQueue :public IQueue {
            friend class VulkanDevice;
        public:
            ~VulkanQueue();
        protected:
            vk::Queue queue_;
            RHIPtr<VulkanDevice> device_;
        };

        class VulkanSurface :public ISurface {
            friend class RHIVulkanSurfaceBuilder;
            friend class VulkanPhysicalDevice;
        public:

        public:
            vk::SurfaceKHR surface_;
        };

        class VulkanDeviceMemory : public IDeviceMemory {
            friend class VulkanDevice;
        public:
            ~VulkanDeviceMemory();
        protected:
            vk::DeviceMemory memory_;
            RHIPtr<VulkanDevice> device_;
        };

        class VulkanBuffer :public IBuffer {
            friend class VulkanDevice;
        public:
            ~VulkanBuffer();
        protected:
            vk::Buffer buffer_;
            RHIPtr<VulkanDevice> device_;
        };

        class VulkanFrameBuffer :public IFrameBuffer {
            friend class VulkanDevice;
        public:
            ~VulkanFrameBuffer();
        protected:
            vk::Framebuffer frameBuffer_;
            RHIPtr<VulkanDevice> device_;
        };

        class VulkanShaderModule :public IShaderModule {
            friend class VulkanDevice;
        public:
            ~VulkanShaderModule();
        protected:
            vk::ShaderModule shader_;
            RHIPtr<VulkanDevice> device_;
        };

        class VulkanPipelineLayout :public IPipelineLayout {
            friend class VulkanDevice;
        public:
            ~VulkanPipelineLayout();
        protected:
            vk::PipelineLayout layout_;
            RHIPtr<VulkanDevice> device_;
        };

        class VulkanDescriptorSetLayout : public IDescriptorSetLayout {
            friend class VulkanDevice;
        public:
            ~VulkanDescriptorSetLayout();
        protected:
            vk::DescriptorSetLayout dslayout_;
            RHIPtr<VulkanDevice> device_;
        };

        class VulkanPipeline :public IPipeline {
            friend class VulkanDevice;
        public:
            ~VulkanPipeline();
        protected:
            vk::Pipeline pipeline_;
            RHIPtr<VulkanDevice> device_;
        };

        class VulkanRenderPass : public IRenderPass {
            friend class VulkanDevice;
        public:
            ~VulkanRenderPass();
        protected:
            vk::RenderPass renderPass_;
            RHIPtr<VulkanDevice>  device_;
        };

        class VulkanImageView : public IImageView {
            friend class VulkanDevice;
        public:
            ~VulkanImageView();
        protected:
            vk::ImageView imageView_;
            RHIPtr<VulkanDevice> device_;
        };

        class VulkanImage :public IImage {
            friend class VulkanDevice;
        public:
            VulkanImage() {}
            VulkanImage(const vk::Image& img) :image_(img) {}
            ~VulkanImage();
            RHIPtr<IImageView> createImageView(const vk::ImageViewCreateInfo& ci) override;
        public:
            vk::Image image_;
            RHIPtr<VulkanDevice> device_;
        };

        class VulkanSwapchain : public ISwapchain {
            friend class VulkanDevice;
        public:
            std::vector<RHIPtr<IImage>> getImagesKHR() override;
        protected:
            vk::SwapchainKHR swapChain_;
            RHIPtr<VulkanDevice> device_;
        };

        class VulkanCommandBuffer : public ICommandBuffer {
            friend class VulkanDevice;
        public:
        protected:
            vk::CommandBuffer cmdBuf_;
        };

        class VulkanCommandPool : public ICommandPool {
            friend class VulkanDevice;
        public:
            virtual ~VulkanCommandPool();
            std::vector<RHIPtr<ICommandBuffer>> allocateCommandBuffers(const vk::CommandBufferAllocateInfo& ai) override;
        protected:
            vk::CommandPool pool_;
            RHIPtr<VulkanDevice> device_;
        };

        class VulkanDevice :public IDevice ,public std::enable_shared_from_this<VulkanDevice> {
            friend class VulkanPhysicalDevice;
            friend class VulkanImage;
            friend class VulkanImageView;
            friend class VulkanRenderPass;
            friend class VulkanCommandPool;
            friend class VulkanPipeline;
            friend class VulkanDescriptorSetLayout;
            friend class VulkanPipelineLayout;
            friend class VulkanShaderModule;
            friend class VulkanFrameBuffer;
            friend class VulkanBuffer;
            friend class VulkanDeviceMemory;
            friend class VulkanSwapchain;
        public:
            ~VulkanDevice();
            RHIPtr<ICommandPool> createCommandPool(const vk::CommandPoolCreateInfo& ci) override;
            
            std::vector<RHIPtr<ICommandBuffer>> allocateCommandBuffers(const vk::CommandBufferAllocateInfo& ai) override;
            
            RHIPtr<ISwapchain> createSwapChain(const vk::SwapchainCreateInfoKHR& ci) override;

            RHIPtr<IImage> createImage(const vk::ImageCreateInfo& ci) override;
           
            RHIPtr<IImageView> createImageView(const vk::ImageViewCreateInfo& ci) override;
           
            RHIPtr<IRenderPass> createRenderPass(const vk::RenderPassCreateInfo& createInfo) override;
        
            RHIPtr<IPipeline> createGraphicsPipeline(const vk::GraphicsPipelineCreateInfo& ci) override;

            RHIPtr<IDescriptorSetLayout> createDescriptorSetLayout(const vk::DescriptorSetLayoutCreateInfo& ci) override;

            RHIPtr<IPipelineLayout> createPipelineLayout(const vk::PipelineLayoutCreateInfo& ci) override;
        
            RHIPtr<IShaderModule> createShaderModule(const vk::ShaderModuleCreateInfo& ci) override;
        
            RHIPtr<IFrameBuffer> createFrameBuffer(const vk::FramebufferCreateInfo& ci) override;
        
            RHIPtr<IBuffer> createBuffer(const vk::BufferCreateInfo& ci) override;

            RHIPtr<IDeviceMemory> allocMemory(const vk::MemoryAllocateInfo& ai) override;
        
            RHIPtr<IQueue> getQueue(uint32_t queueFamilyIndex, uint32_t queueIndex) override;
        protected:
            void destroySwapchain(VulkanSwapchain* swapChain);
            void destroyCommandPool(VulkanCommandPool* pool);
            void destroyRenderPass(VulkanRenderPass* renderPass);
            void destroyImage(VulkanImage* image);
            void destroyImageView(VulkanImageView* imageView);
            void destroyPipeline(VulkanPipeline* pipeline);
            void destroyDescriptorSetLayout(VulkanDescriptorSetLayout* dslayout);
            void destroyPipelineLayout(VulkanPipelineLayout* layout);
            void destroyShaderModule(VulkanShaderModule* shader);
            void destroyFrameBuffer(VulkanFrameBuffer* frameBuffer);
            void destroyBuffer(VulkanBuffer* buffer);
            void freeMemory(VulkanDeviceMemory* memory);
            vk::Device device_;
        };

        class VulkanPhysicalDevice: public IPhysicalDevice {
            friend class VulkanContext;
        public:
            std::vector<vk::QueueFamilyProperties>  getQueueFamilyProperties() override;
            RHIPtr<IDevice> createDevice(const vk::DeviceCreateInfo& ci) override;

            VkBool32 getSurfaceSupportKHR(uint32_t queueFamilyIndex, RHIPtr<ISurface> surface) override;
            std::vector<vk::SurfaceFormatKHR> getSurfaceFormatsKHR(RHIPtr<ISurface> surface) override;
            vk::SurfaceCapabilitiesKHR getSurfaceCapabilitiesKHR(RHIPtr<ISurface> surface) override;
            std::vector<vk::PresentModeKHR> getSurfacePresentModesKHR(RHIPtr<ISurface> surface) override;

            std::vector<vk::ExtensionProperties> enumerateDeviceExtensionProperties() override;

            vk::PhysicalDeviceMemoryProperties getMemoryProperties() override;
            vk::PhysicalDeviceProperties getProperties() override;
            vk::PhysicalDeviceFeatures getFeatures() override;

            vk::FormatProperties getFormatProperties(vk::Format format) override;
        protected:
            vk::PhysicalDevice device_;
        };

        class VulkanContext :public IContext
        {
            friend class VulkanRHI;
            friend class RHIVulkanSurfaceBuilder;
        public:
            void destroy() override;
            //vk::DebugUtilsMessengerEXT createDebugUtilsMessengerEXT() override;
            //void destroyDebugUtilsMessengerEXT(vk::DebugUtilsMessengerEXT) override;
            std::vector<RHIPtr<IPhysicalDevice>> enumeratePhysicalDevices() override;
        protected:
            vk::Instance instance_;
        };

        class VulkanRHI : public RHI2
        {
        public:
            ~VulkanRHI() override;
            virtual RHIPtr<IContext> createContext(const vk::InstanceCreateInfo& ci) override;

        };
    }
}

#endif