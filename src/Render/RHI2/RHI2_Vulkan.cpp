#include "RHI2_Vulkan.h"

namespace Sun {
namespace rhi {

    VulkanQueue::~VulkanQueue() {
    
    }

    /// <summary>
    /// /////////
    /// </summary>

    VulkanDeviceMemory::~VulkanDeviceMemory() {
        device_->freeMemory(this);
    }

    /// <summary>
    /// ////
    /// </summary>

    VulkanBuffer::~VulkanBuffer() {
        device_->destroyBuffer(this);
    }

    /// <summary>
    /// /
    /// </summary>

    VulkanFrameBuffer::~VulkanFrameBuffer() {
        device_->destroyFrameBuffer(this);
    }

    /// <summary>
    /// //////
    /// </summary>

    VulkanShaderModule::~VulkanShaderModule() {
        device_->destroyShaderModule(this);
    }

    /// <summary>
    /// ////////
    /// </summary>

    VulkanPipelineLayout::~VulkanPipelineLayout() {
        device_->destroyPipelineLayout(this);
    }

    /// <summary>
    /// ////////
    /// </summary>

    VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout() {
        device_->destroyDescriptorSetLayout(this);
    }

    /// <summary>
    /// /////////
    /// </summary>

    VulkanPipeline::~VulkanPipeline() {
        device_->destroyPipeline(this);
    }


    /// <summary>
    /// ////////////
    /// </summary>

    VulkanRenderPass::~VulkanRenderPass() {
        device_->destroyRenderPass(this);
    }

    /////////////

    VulkanImage::~VulkanImage() {
        device_->destroyImage(this);
    }

    RHIPtr<IImageView> VulkanImage::createImageView(const vk::ImageViewCreateInfo& ci) {
        vk::ImageViewCreateInfo cpy = ci;
        cpy.setImage(this->image_);
        return device_->createImageView(cpy);
    }

    ////////////////////

    VulkanImageView::~VulkanImageView() {
        device_->destroyImageView(this);
    }

    /////////////

    std::vector<RHIPtr<IImage>> VulkanSwapchain::getImagesKHR() {
        std::vector<vk::Image> vec = device_->device_.getSwapchainImagesKHR(swapChain_);
        std::vector<RHIPtr<IImage>> images;
        for (int i = 0; i < vec.size(); ++i) {
            images.emplace_back(new VulkanImage(vec[i]));
        }
        return images;
    }

    //////////////

    VulkanCommandPool::~VulkanCommandPool() {
        device_->destroyCommandPool(this);
    }

    std::vector<RHIPtr<ICommandBuffer>> VulkanCommandPool::allocateCommandBuffers(const vk::CommandBufferAllocateInfo& ai) {
        vk::CommandBufferAllocateInfo info = ai;
        info.setCommandPool(this->pool_);
        return device_->allocateCommandBuffers(ai);
    }

    /// <summary>
    /// ///////////////////////////////
    /// </summary>
    /// 
    VulkanDevice::~VulkanDevice() {
        device_.destroy();
    }

    RHIPtr<ICommandPool> VulkanDevice::createCommandPool(const vk::CommandPoolCreateInfo& ci) {
        RHIPtr<VulkanCommandPool> ptr(new VulkanCommandPool());
        ptr->pool_ = device_.createCommandPool(ci);
        return ptr;
    }

    std::vector<RHIPtr<ICommandBuffer>> VulkanDevice::allocateCommandBuffers(const vk::CommandBufferAllocateInfo& ai) {
        std::vector<RHIPtr<ICommandBuffer>> res;
        std::vector<vk::CommandBuffer> buffers = device_.allocateCommandBuffers(ai);
        for (auto& it : buffers) {
            RHIPtr<VulkanCommandBuffer> ptr(new VulkanCommandBuffer());
            ptr->cmdBuf_ = it;
            res.emplace_back(ptr);
        }
        return res;
    }

    RHIPtr<ISwapchain> VulkanDevice::createSwapChain(const vk::SwapchainCreateInfoKHR& ci) {
        RHIPtr<VulkanSwapchain> ptr(new VulkanSwapchain());
        ptr->swapChain_ = device_.createSwapchainKHR(ci);
        ptr->device_ = shared_from_this();
        return ptr;
    }

    RHIPtr<IImage> VulkanDevice::createImage(const vk::ImageCreateInfo& ci) {
        RHIPtr<VulkanImage> ptr(new VulkanImage());
        ptr->image_ = device_.createImage(ci);
        ptr->device_ = shared_from_this();
        return ptr;
    }

    RHIPtr<IImageView> VulkanDevice::createImageView(const vk::ImageViewCreateInfo& ci) {
        RHIPtr<VulkanImageView> ptr(new VulkanImageView());
        ptr->imageView_ =  device_.createImageView(ci);
        ptr->device_ = shared_from_this();
        return ptr;
    }

   
    RHIPtr<IRenderPass> VulkanDevice::createRenderPass(const vk::RenderPassCreateInfo& createInfo) {
        RHIPtr<VulkanRenderPass> ptr(new VulkanRenderPass());
        ptr->renderPass_ = device_.createRenderPass(createInfo);
        ptr->device_ = shared_from_this();
        return ptr;
    }

    RHIPtr<IPipeline> VulkanDevice::createGraphicsPipeline(const vk::GraphicsPipelineCreateInfo& ci) {
        RHIPtr<VulkanPipeline> ptr(new VulkanPipeline());
        vk::Result   result;
        vk::Pipeline pipeline;
        std::tie(result, pipeline) = device_.createGraphicsPipeline(nullptr, ci);
        switch (result)
        {
        case vk::Result::eSuccess: break;
        case vk::Result::ePipelineCompileRequiredEXT:
            // something meaningfull here TODO
            break;
        default: assert(false);  // should never happen
        }
        ptr->pipeline_ = pipeline;
        ptr->device_ = shared_from_this();
        return ptr;
    }

    RHIPtr<IDescriptorSetLayout> VulkanDevice::createDescriptorSetLayout(const vk::DescriptorSetLayoutCreateInfo& ci) {
        RHIPtr<VulkanDescriptorSetLayout> ptr(new VulkanDescriptorSetLayout());
        ptr->dslayout_ = device_.createDescriptorSetLayout(ci);
        ptr->device_ = shared_from_this();
        return ptr;
    }

    RHIPtr<IPipelineLayout> VulkanDevice::createPipelineLayout(const vk::PipelineLayoutCreateInfo& ci) {
        RHIPtr<VulkanPipelineLayout> ptr(new VulkanPipelineLayout());
        ptr->layout_ = device_.createPipelineLayout(ci);
        ptr->device_ = shared_from_this();
        return ptr;
    }

    RHIPtr<IShaderModule> VulkanDevice::createShaderModule(const vk::ShaderModuleCreateInfo& ci) {
        RHIPtr<VulkanShaderModule> ptr(new VulkanShaderModule());
        ptr->shader_ = device_.createShaderModule(ci);
        ptr->device_ = shared_from_this();
        return ptr;
    }

    RHIPtr<IFrameBuffer> VulkanDevice::createFrameBuffer(const vk::FramebufferCreateInfo& ci) {
        RHIPtr<VulkanFrameBuffer> ptr(new VulkanFrameBuffer());
        ptr->frameBuffer_ = device_.createFramebuffer(ci);
        ptr->device_ = shared_from_this();
        return ptr;
    }

    RHIPtr<IBuffer> VulkanDevice::createBuffer(const vk::BufferCreateInfo& ci) {
        RHIPtr<VulkanBuffer> ptr(new VulkanBuffer());
        ptr->buffer_ = device_.createBuffer(ci);
        ptr->device_ = shared_from_this();
        return ptr;
    }

    RHIPtr<IDeviceMemory> VulkanDevice::allocMemory(const vk::MemoryAllocateInfo& ai) {
        RHIPtr<VulkanDeviceMemory> ptr(new VulkanDeviceMemory());
        ptr->memory_ = device_.allocateMemory(ai);
        ptr->device_ = shared_from_this();
        return ptr;
    }

    RHIPtr<IQueue> VulkanDevice::getQueue(uint32_t queueFamilyIndex, uint32_t queueIndex) {
        RHIPtr<VulkanQueue> ptr(new VulkanQueue());
        ptr->queue_ = device_.getQueue(queueFamilyIndex,queueIndex);
        ptr->device_ = shared_from_this();
        return ptr;
    }

    void VulkanDevice::destroySwapchain(VulkanSwapchain* swapChain) {
        device_.destroySwapchainKHR(swapChain->swapChain_);
    }

    void VulkanDevice::destroyCommandPool(VulkanCommandPool* pool) {
        device_.destroyCommandPool(pool->pool_);
    }

    void VulkanDevice::destroyRenderPass(VulkanRenderPass* renderPass) {    
        device_.destroyRenderPass(renderPass->renderPass_);
    }

    void VulkanDevice::destroyImage(VulkanImage* image) {
        device_.destroyImage(image->image_);
    }

    void VulkanDevice::destroyImageView(VulkanImageView* imageView) {
        device_.destroyImageView(imageView->imageView_);
    }

    void VulkanDevice::destroyPipeline(VulkanPipeline* pipeline) {
        device_.destroyPipeline(pipeline->pipeline_);
    }

    void VulkanDevice::destroyDescriptorSetLayout(VulkanDescriptorSetLayout* dslayout) {
        device_.destroyDescriptorSetLayout(dslayout->dslayout_);
    }

    void VulkanDevice::destroyPipelineLayout(VulkanPipelineLayout* layout) {
        device_.destroyPipelineLayout(layout->layout_);
    }

    void VulkanDevice::destroyShaderModule(VulkanShaderModule* shader) {
        device_.destroyShaderModule(shader->shader_);
    }

    void VulkanDevice::destroyFrameBuffer(VulkanFrameBuffer* frameBuffer) {
        device_.destroyFramebuffer(frameBuffer->frameBuffer_);
    }

    void VulkanDevice::destroyBuffer(VulkanBuffer* buffer) {
        device_.destroyBuffer(buffer->buffer_);
    }

    void VulkanDevice::freeMemory(VulkanDeviceMemory* memory) {
        device_.freeMemory(memory->memory_);
    }

    /// <summary>
    /// //////////
    /// </summary>
    /// <returns></returns>

    std::vector<vk::QueueFamilyProperties>  VulkanPhysicalDevice::getQueueFamilyProperties() {
        return device_.getQueueFamilyProperties();
    }

    RHIPtr<IDevice> VulkanPhysicalDevice::createDevice(const vk::DeviceCreateInfo& ci) {
        RHIPtr<VulkanDevice> ptr(new VulkanDevice());
        ptr->device_ = device_.createDevice(ci);
        return ptr;
    }

    VkBool32 VulkanPhysicalDevice::getSurfaceSupportKHR(uint32_t queueFamilyIndex, RHIPtr<ISurface> surface) {
        RHIPtr<VulkanSurface> sur = std::dynamic_pointer_cast<VulkanSurface>(surface);
        return device_.getSurfaceSupportKHR(queueFamilyIndex, sur->surface_);
    }

    std::vector<vk::SurfaceFormatKHR> VulkanPhysicalDevice::getSurfaceFormatsKHR(RHIPtr<ISurface> surface) {
        RHIPtr<VulkanSurface> sur = std::dynamic_pointer_cast<VulkanSurface>(surface);
        return device_.getSurfaceFormatsKHR(sur->surface_);
    }

    vk::SurfaceCapabilitiesKHR VulkanPhysicalDevice::getSurfaceCapabilitiesKHR(RHIPtr<ISurface> surface) {
        RHIPtr<VulkanSurface> sur = std::dynamic_pointer_cast<VulkanSurface>(surface);
        return device_.getSurfaceCapabilitiesKHR(sur->surface_);
    }

    std::vector<vk::PresentModeKHR>  VulkanPhysicalDevice::getSurfacePresentModesKHR(RHIPtr<ISurface> surface) {
        RHIPtr<VulkanSurface> sur = std::dynamic_pointer_cast<VulkanSurface>(surface);
        return device_.getSurfacePresentModesKHR(sur->surface_);

    }

    std::vector<vk::ExtensionProperties> VulkanPhysicalDevice::enumerateDeviceExtensionProperties() {
        return device_.enumerateDeviceExtensionProperties();
    }

    vk::PhysicalDeviceMemoryProperties VulkanPhysicalDevice::getMemoryProperties() {
        return device_.getMemoryProperties();
    }

    vk::PhysicalDeviceProperties VulkanPhysicalDevice::getProperties() {
        return device_.getProperties();
    }

    vk::PhysicalDeviceFeatures VulkanPhysicalDevice::getFeatures() {
        return device_.getFeatures();
    }

    vk::FormatProperties VulkanPhysicalDevice::getFormatProperties(vk::Format format) {
        return device_.getFormatProperties(format);
    }

    /// <summary>
    /// //////////////////////////////////////////
    /// </summary>

    void VulkanContext::destroy() {
        instance_.destroy();
    }

    //vk::DebugUtilsMessengerEXT VulkanContext::createDebugUtilsMessengerEXT() {
    //    return vk::su::createDebugUtilsMessengerEXT(instance_);
    //}

    //void VulkanContext::destroyDebugUtilsMessengerEXT(vk::DebugUtilsMessengerEXT ext) {
    //    instance_.destroyDebugUtilsMessengerEXT(ext);
    //}

    std::vector<RHIPtr<IPhysicalDevice>> VulkanContext::enumeratePhysicalDevices() {
        std::vector<vk::PhysicalDevice> vec = instance_.enumeratePhysicalDevices();
        std::vector<RHIPtr<IPhysicalDevice>> res;
        for (int i = 0; i < vec.size(); ++i) {
            RHIPtr<VulkanPhysicalDevice> ptr(new VulkanPhysicalDevice);
            ptr->device_ = vec[i];
            res.push_back(ptr);
        }
        return res;
    }

    /// ////////////////////////////////////

    VulkanRHI::~VulkanRHI() {

    }

    RHIPtr<IContext> VulkanRHI::createContext(const vk::InstanceCreateInfo& ci) {
        RHIPtr<VulkanContext> context(new VulkanContext());
        context->instance_ = vk::createInstance(ci);
        return context;
    }

}
}