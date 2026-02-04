#ifndef __SUN_RENDER_SOFTWARE_INTERFACE_H
#define __SUN_RENDER_SOFTWARE_INTERFACE_H

#include "RHI2.h"
#include "RHISurfaceBuilder.h"
#include <memory>

namespace Sun {

    using namespace rhi;

#define WIN32_VARS g_win32;

    struct PhysicalDeviceInfo {
        RHIPtr<IPhysicalDevice>                 device;
        vk::PhysicalDeviceProperties            props;
        vk::PhysicalDeviceMemoryProperties      memProps;
        vk::PhysicalDeviceFeatures              features;
        vk::SurfaceCapabilitiesKHR              surfaceCaps;
        std::vector<vk::SurfaceFormatKHR>       surfaceFormats;
        std::vector<vk::PresentModeKHR>         presentModes;
        std::vector<vk::QueueFamilyProperties>  queueFamilyProps;
        std::vector<vk::ExtensionProperties>    extensionProps;
    };

    struct SwapChainInfo {
        RHIPtr<ISwapchain> swapchain_;

        vk::Format format_;
        vk::PresentModeKHR presentMode_;
        vk::Extent2D extent_;
        vk::ImageUsageFlags imageUsage_;

        std::vector<RHIPtr<IImage>> images_;
        std::vector<RHIPtr<IImageView>> imageViews_;
        std::vector<RHIPtr<ISemaphore>> readSemaphores_;
        std::vector<RHIPtr<ISemaphore>> writtenSemaphores_;
    };

    class RSI {
    public:
        void initEnvironment();

        //设置开启校验层
        void setEnableValidationLayers(bool open);
        //设置校验层
        void setValidationLayers(const std::vector<std::string>& layers);
        //设置需要支持的物理设备扩展，在initEnvironment之前调用可生效
        void setRequiredDeviceExtensions(const std::vector<std::string>& extensions);
    protected:
        void createContext();
        void createSurface();
        /*
        * 首先会枚举所有物理设备信息,之后从中选择满足扩展支持且包含图形和显示队列簇的设备
        */
        void selectSuitablePhysicalDevice();
        void createLogicalDeviceAndQueues();

        void createSwapChain();
        //TODO:这个函数的内容应该以脚本的方式实现为可配置,当前为hardcode
        void createRenderPass();
        //TODO:同上
        void createGraphicsPipeline();

        std::unique_ptr<rhi::RHI2> rhi_;
        std::unique_ptr<ISurfaceBuilder> surfaceBuilder_;

        RHIPtr<IContext> context_;
        RHIPtr<ISurface> surface_;

    protected:
        static bool checkPhysicalDeviceExtensionSupport(const PhysicalDeviceInfo& gpu, const std::vector<std::string>& requiredExt);
        static vk::SurfaceFormatKHR chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats);
        static vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR>& modes);
        static vk::Extent2D chooseSurfaceExtent(const vk::SurfaceCapabilitiesKHR& caps);


        PhysicalDeviceInfo deviceInfo_;
        RHIPtr<IDevice> device_;
        RHIPtr<IQueue> graphicsQueue_;
        RHIPtr<IQueue> prensentQueue_;
        int		graphicsFamilyIdx_;
        int		presentFamilyIdx_;
        SwapChainInfo swapChainInfo_;
        RHIPtr<IRenderPass> renderPass_;

        std::vector<std::string> deviceExtensions_;
        std::vector<std::string> validationLayers_;
        bool    enableValidationLayers_ = false;
    };


}

#endif