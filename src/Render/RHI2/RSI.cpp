#include "RSI.h"
#include "RHI2_Vulkan.h"
#include "RHI2_Opengl.h"
#include "RHIVulkanSurfaceBuilder.h"
#include "RHIOpenglSurfaceBuilder.h"
#include "RenderBluePrint.h"
#include "Common/Common.h"
#include "Common/WindowInfo.h"
#include <cassert>
#include <set>

using namespace Sun::rhi;

namespace Sun {

    void RSI::initEnvironment() {
        GRAPHICS_API_TYPE apiType = globalGetGraphics_API_Type();
        switch (apiType) {
            case GRAPHICS_API_TYPE::G_OPENGL:
            {
                rhi_.reset(new VulkanRHI());
                surfaceBuilder_.reset(new RHIVulkanSurfaceBuilder);
            }
            break;
            case GRAPHICS_API_TYPE::G_VULKAN:
            {
                rhi_.reset(new OpenglRHI());
                surfaceBuilder_.reset(new RHIOpenglSurfaceBuilder);
            }
            break;
            default:
            {
                assert(false);
            }
        }

        //加载dll
        
        createContext();

        createSurface();
    }

    void RSI::setEnableValidationLayers(bool open) {
        enableValidationLayers_ = open;
    }

    void RSI::setValidationLayers(const std::vector<std::string>& layers) {
        validationLayers_ = layers;
    }

    void RSI::setRequiredDeviceExtensions(const std::vector<std::string>& extensions) {
        deviceExtensions_ = extensions;
    }

    void RSI::createContext() {
        vk::ApplicationInfo applicationInfo("SunEngine", 1, "SunEngine 1.0", 1, VK_API_VERSION_1_2);

        vk::InstanceCreateInfo ci({}, &applicationInfo);

        context_ = rhi_->createContext(ci);
    }

    void RSI::createSurface() {
        //TODO 
        surface_ = surfaceBuilder_->buildSurface(context_, nullptr);
    }

    void RSI::selectSuitablePhysicalDevice() {
        
        std::vector<RHIPtr<IPhysicalDevice>> physicalDevices = context_->enumeratePhysicalDevices();
        assert(physicalDevices.size() > 0);
        int numDevices = physicalDevices.size();

        std::vector<PhysicalDeviceInfo> gpus;
        gpus.resize(numDevices);

        for (uint32_t i = 0; i < numDevices; ++i) {
            PhysicalDeviceInfo& gpu = gpus[i];
            gpu.device = physicalDevices[i];
            gpu.queueFamilyProps =  gpu.device->getQueueFamilyProperties();
            gpu.extensionProps = gpu.device->enumerateDeviceExtensionProperties();
            gpu.surfaceCaps = gpu.device->getSurfaceCapabilitiesKHR(surface_);
            gpu.surfaceFormats = gpu.device->getSurfaceFormatsKHR(surface_);
            gpu.presentModes = gpu.device->getSurfacePresentModesKHR(surface_);
            gpu.memProps = gpu.device->getMemoryProperties();
            gpu.props = gpu.device->getProperties();
            gpu.features = gpu.device->getFeatures();
        }

        //select one
        for (int i = 0; i < gpus.size(); ++i) {
            PhysicalDeviceInfo& gpu = gpus[i];

            int graphicsIdx = -1;
            int presentIdx = -1;

            if (gpu.features.samplerAnisotropy == VK_FALSE) {
                continue;
            }
            if (!checkPhysicalDeviceExtensionSupport(gpu, deviceExtensions_)) {
                continue;
            }
            if (gpu.surfaceFormats.size() == 0 ||gpu.presentModes.size()==0) {
                continue;
            }
            // Find graphics queue family
            for (int j = 0; j < gpu.queueFamilyProps.size(); ++j) {
                vk::QueueFamilyProperties& props = gpu.queueFamilyProps[j];
                if (props.queueCount == 0) {
                    continue;
                }
                if (props.queueFlags & vk::QueueFlagBits::eGraphics) {
                    graphicsIdx = j;
                    break;
                }
            }
            // Find present queue family
            for (int j = 0; j < gpu.queueFamilyProps.size(); ++j) {
                VkQueueFamilyProperties& props = gpu.queueFamilyProps[j];

                if (props.queueCount == 0) {
                    continue;
                }

                auto supportsPresent = gpu.device->getSurfaceSupportKHR(j, surface_);
                if (supportsPresent) {
                    presentIdx = j;
                    break;
                }
            } 

            if (graphicsIdx >= 0 && presentIdx >= 0) {
                graphicsFamilyIdx_ = graphicsIdx;
                presentFamilyIdx_ = presentIdx;
                deviceInfo_ = gpu;

                return;
            }
        }

        
        assert(false);
        return;
        
    }

    void RSI::createLogicalDeviceAndQueues() {
        std::set<int> idx;
        idx.insert(graphicsFamilyIdx_);
        idx.insert(presentFamilyIdx_);

        const float priority[1] = { 1.0 };

        std::vector<vk::DeviceQueueCreateInfo> devqInfos;
        for (auto& it:idx ) {
            vk::DeviceQueueCreateInfo ci;
            ci.setQueueFamilyIndex(it);
            ci.setQueueCount(1);
            ci.setPQueuePriorities(priority);
            devqInfos.push_back(ci);
        }

        //这一段应当从配置文件中读取TODO
        vk::PhysicalDeviceFeatures deviceFeatures;
        deviceFeatures.textureCompressionBC = VK_TRUE;
        deviceFeatures.imageCubeArray = VK_TRUE;
        deviceFeatures.depthClamp = VK_TRUE;
        deviceFeatures.depthBiasClamp = VK_TRUE;
        deviceFeatures.depthBounds = deviceInfo_.features.depthBounds;
        deviceFeatures.fillModeNonSolid = VK_TRUE;

        vk::DeviceCreateInfo ci;
        ci.setQueueCreateInfoCount(devqInfos.size());
        ci.setPQueueCreateInfos(devqInfos.data());
        ci.setPEnabledFeatures(&deviceFeatures);
        std::vector<const char*> deviceExtension;
        for (int i = 0; i < deviceExtensions_.size(); ++i) {
            deviceExtension.push_back(deviceExtensions_[i].c_str());
        }
        ci.setPpEnabledExtensionNames(deviceExtension.data());
        ci.setEnabledExtensionCount(deviceExtension.size());

        if (enableValidationLayers_) {
            ci.enabledLayerCount = validationLayers_.size();
            std::vector<const char*> layers;
            for (int i = 0; i < validationLayers_.size(); ++i) {
                layers.push_back(validationLayers_[i].c_str());
            }
            ci.ppEnabledLayerNames = layers.data();
        }
        else {
            ci.enabledLayerCount = 0;
        }

        device_ = deviceInfo_.device->createDevice(ci);

        graphicsQueue_ = device_->getQueue(graphicsFamilyIdx_, 0);
        prensentQueue_ = device_->getQueue(presentFamilyIdx_, 0);

        return;
    }

    void RSI::createSwapChain() {
        vk::SurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(deviceInfo_.surfaceFormats);
        vk::PresentModeKHR presentMode = choosePresentMode(deviceInfo_.presentModes);
        vk::Extent2D extent = chooseSurfaceExtent(deviceInfo_.surfaceCaps);

        // Determine the number of VkImage's to use in the swap chain (we desire to
        // own only 1 image at a time, besides the images being displayed and
        // queued for display):
        uint32_t imageCount = deviceInfo_.surfaceCaps.minImageCount + 1;
        if (deviceInfo_.surfaceCaps.maxImageCount > 0 && imageCount > deviceInfo_.surfaceCaps.maxImageCount) {
            imageCount = deviceInfo_.surfaceCaps.maxImageCount;
        }

        vk::SwapchainCreateInfoKHR info = {};
        RHIPtr<VulkanSurface> vulkanSurface = std::dynamic_pointer_cast<VulkanSurface>(surface_);
        info.surface = vulkanSurface->surface_;
        info.minImageCount = imageCount;
        info.imageFormat = surfaceFormat.format;
        info.imageColorSpace = surfaceFormat.colorSpace;
        info.imageExtent = extent;
        info.imageArrayLayers = 1;
        info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc;
            /*VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT; */
        if (graphicsFamilyIdx_ != presentFamilyIdx_) {
            uint32_t indices[] = { (uint32_t)graphicsFamilyIdx_, (uint32_t)presentFamilyIdx_ };
            info.imageSharingMode = vk::SharingMode::eConcurrent;
            info.queueFamilyIndexCount = 2;
            info.pQueueFamilyIndices = indices;
        }
        else {
            info.imageSharingMode = vk::SharingMode::eExclusive;
            info.queueFamilyIndexCount = 0;
            info.pQueueFamilyIndices = nullptr;
        }
        info.preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
        info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        info.presentMode = presentMode;
        info.clipped = VK_TRUE;

        swapChainInfo_.format_ = surfaceFormat.format;
        swapChainInfo_.swapchain_ = device_->createSwapChain(info);
        swapChainInfo_.presentMode_ = info.presentMode;
        swapChainInfo_.extent_ = extent;

        swapChainInfo_.images_ = swapChainInfo_.swapchain_->getImagesKHR();
        swapChainInfo_.imageViews_.resize(swapChainInfo_.images_.size());
        for (int i = 0; i < swapChainInfo_.images_.size(); ++i) {
            vk::ImageViewCreateInfo ci;
            ci.viewType = vk::ImageViewType::e2D;
            ci.format = swapChainInfo_.format_;
            ci.components.r = vk::ComponentSwizzle::eR;
            ci.components.g = vk::ComponentSwizzle::eG;
            ci.components.b = vk::ComponentSwizzle::eB;
            ci.components.a = vk::ComponentSwizzle::eA;
            ci.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
            ci.subresourceRange.baseMipLevel = 0;
            ci.subresourceRange.levelCount = 1;
            ci.subresourceRange.baseArrayLayer = 0;
            ci.subresourceRange.levelCount = 1;
            swapChainInfo_.imageViews_[i] = swapChainInfo_.images_[i]->createImageView(ci);
            
        }
    }

    void RSI::createRenderPass() {
        //创建附件描述
        vk::AttachmentDescription depthAttachment = RenderBluePrint::depthAttachment;
        depthAttachment.format = deviceInfo_.device->chooseSupportedFormat(
            RenderBluePrint::depthFormatInput.format,
            RenderBluePrint::depthFormatInput.tiling,
            RenderBluePrint::depthFormatInput.features);

        auto colorAttachment = RenderBluePrint::colorAttachment;
        colorAttachment.format = swapChainInfo_.format_;

        std::array<vk::AttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

        //创建子通道
        vk::SubpassDescription subpass = {};
        subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        vk::AttachmentReference colorRef = {};
        colorRef.attachment = 0;
        colorRef.layout = vk::ImageLayout::eColorAttachmentOptimal;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorRef;
        vk::AttachmentReference depthRef = {};
        depthRef.attachment = 1;
        depthRef.layout = vk::ImageLayout::eDepthAttachmentOptimal;
        subpass.pDepthStencilAttachment = &depthRef;

        //创建子通道依赖
        vk::SubpassDependency depend = {};
        depend.srcSubpass = VK_SUBPASS_EXTERNAL;
        depend.dstSubpass = 0;
        depend.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        depend.srcAccessMask = {};
        depend.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        depend.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;

        //创建渲染通道
        vk::RenderPassCreateInfo ci = {};
        ci.attachmentCount = attachments.size();
        ci.pAttachments = attachments.data();
        ci.subpassCount = 1;
        ci.pSubpasses = &subpass;
        ci.dependencyCount = 1;
        ci.pDependencies = &depend;

        renderPass_ = device_->createRenderPass(ci);
    }

    void RSI::createGraphicsPipeline() {



    }

    bool RSI::checkPhysicalDeviceExtensionSupport(const PhysicalDeviceInfo& gpu, const std::vector<std::string>& requiredExt) {
        int required = requiredExt.size();
        int available = 0;

        for (int i = 0; i < required; ++i) {
            for (int j = 0; j < gpu.extensionProps.size(); ++j) {
                if (requiredExt[i] == gpu.extensionProps[j].extensionName) {
                    available++;
                    break;
                }
            }
        }

        return available == required;
    }

    vk::SurfaceFormatKHR RSI::chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats) {
        vk::SurfaceFormatKHR result;

        //支持任意格式，则选择期望格式 （TODO，何为期望格式，配置文件)
        if (formats.size() == 1 && formats[0].format == vk::Format::eUndefined) {
            result.format = vk::Format::eB8G8R8A8Unorm;
            result.colorSpace = vk::ColorSpaceKHR::eVkColorspaceSrgbNonlinear;
            return result;
        }
        //如果不能自由选择，则遍历列表选择偏向性格式
        for (int i = 0; i < formats.size(); ++i) {
            const vk::SurfaceFormatKHR& fmt = formats[i];
            if (fmt.format == vk::Format::eB8G8R8A8Unorm && fmt.colorSpace == vk::ColorSpaceKHR::eVkColorspaceSrgbNonlinear) {
                return fmt;
            }
        }

        return formats[0];
    }

    vk::PresentModeKHR RSI::choosePresentMode(const std::vector<vk::PresentModeKHR>& modes) {
        //优先检查三级缓冲模式
        for (const auto& availablePresentMode : modes) {
            if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
                return availablePresentMode;
            }
        }

        //双缓冲模式
        for (const auto& availablePresentMode : modes) {
            if (availablePresentMode == vk::PresentModeKHR::eFifo) {
                return availablePresentMode;
            }
        }

        return vk::PresentModeKHR::eImmediate;
    }

    vk::Extent2D RSI::chooseSurfaceExtent(const vk::SurfaceCapabilitiesKHR& caps) {
        VkExtent2D extent;

        if (caps.currentExtent.width == std::numeric_limits<uint32_t>::max()) {
            extent.width = g_winInfo.nativeScreenWidth;
            extent.height = g_winInfo.nativeScreenHeight;
        }
        else {
            extent = caps.currentExtent;
        }

        return extent;

    }

}