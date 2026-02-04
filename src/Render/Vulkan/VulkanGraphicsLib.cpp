#include "VulkanGraphicsLib.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <set>
#include <cassert>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <array>
#include "MyVulkanWin32.h"
#include "Math/Hash.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "ThirdParty/tiny_obj_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "ThirdParty/stb_image.h"

namespace std {
    template<> struct hash<Sun::VulkanGraphicsLib::vertex> {
        size_t operator()(const Sun::VulkanGraphicsLib::vertex& vertex) const {
            return ((hash<Sun::vec3>()(vertex._pt) ^
                (hash<Sun::vec4>()(vertex._rgba) << 1)) >> 1) ^
                (hash<Sun::vec2>()(vertex._textureCoord) << 1);
        }
    };
}

namespace Sun {

    std::vector<char> readFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }
        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;

    }


    const std::vector<const char*> g_validationLayers = {
"VK_LAYER_LUNARG_standard_validation"
    };
#ifdef _DEBUG
    const bool g_enableValidationLayers = true;
#else
    const bool g_enableValidationLayers = false;
#endif

    const std::vector<const char*> g_deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    //标记是否开启设备扩展
    const bool g_enableDeviceExtensions = true;

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objType,
        uint64_t obj,
        size_t location,
        int32_t code,
        const char* layerPrefix,
        const char* msg,
        void* userData) {

        std::cerr << "validation layer: " << msg << std::endl;

        return VK_FALSE;
    }

	void VulkanGraphicsLib::initEnvironment() {
        //initStru = initStru_s; FIXME
        createInstance();
        setupDebugCallback();

        createSurface();
        //依赖surface,因此必须位于createSurface之后,选择一个物理设备(gpu)
        pickPhysicalDevice();
        //根据物理设备支持的队列簇，创建逻辑设备，并返回关联的队列簇
        createLogicalDevice();

        //依赖surface和physicDevice
        createSwapChain(VK_NULL_HANDLE);
        //createImageViews();
        createRenderPass();

        //这个对象其实就是对shader中uniform对象的描述，createGraphicsPipeline会用到它
        createDescriptorSetLayout();

        //图形管线其实就是整个shaderProgram的配置
        createGraphicsPipeline();
        //createFramebuffers();
        createCommandPool();

        createDepthResources();
        createFramebuffers();

        createTextureImage();
        createTextureImageView();
        createTextureSampler();

        loadModel();
        createVertexBuffer();
        createIndexBuffer();
        createUniformBuffer();
        //必须放在createGraphicsPipeline之后
        createDescriptorPool();
        createDescriptorSet();


        createCommandBuffers();
        createSemaphores();

	}

	void VulkanGraphicsLib::bindVertexArray(unsigned int id) {



	}

    void VulkanGraphicsLib::recreateSwapChain() {
        vkDeviceWaitIdle(vkDevice_);
        cleanupSwapChain();

        createSwapChain(VK_NULL_HANDLE/*vkSwapChain*/);
        //createImageViews();
        createRenderPass();
        createGraphicsPipeline();
        createDepthResources();
        createFramebuffers();
        createCommandBuffers();
    }

    void VulkanGraphicsLib::setExtend(const VkExtent2D& extend) {
        initStru_.width = extend.width;
        initStru_.height = extend.height;
    }

    void VulkanGraphicsLib::cleanup() {
        vkDestroySemaphore(vkDevice_, vkRenderFinishedSemaphore_, nullptr);
        vkDestroySemaphore(vkDevice_, vkImageAvailableSemaphore_, nullptr);

        vkDestroyCommandPool(vkDevice_, vkCommandPool_, nullptr);

        vkDestroyBuffer(vkDevice_, vkVertexBuffer_, nullptr);
        vkFreeMemory(vkDevice_, vkVertexBufferMemory_, nullptr);

        vkDestroyBuffer(vkDevice_, vkIndexBuffer_, nullptr);
        vkFreeMemory(vkDevice_, vkIndexBufferMemory_, nullptr);

        for (size_t i = 0; i < swapChainFramebuffers_.size(); i++) {
            vkDestroyFramebuffer(vkDevice_, swapChainFramebuffers_[i], nullptr);
        }
        //这两个函数对应createGraphicsPipeline
        vkDestroyPipeline(vkDevice_, vkGraphicsPipeline_, nullptr);
        vkDestroyPipelineLayout(vkDevice_, vkPipelineLayout_, nullptr);

        vkDestroyRenderPass(vkDevice_, vkRenderPass_, nullptr);
        for (size_t i = 0; i < swapChainImageViews_.size(); i++) {
            vkDestroyImageView(vkDevice_, swapChainImageViews_[i], nullptr);
        }
        vkDestroySwapchainKHR(vkDevice_, vkSwapChain_, nullptr);
        vkDestroyDescriptorSetLayout(vkDevice_, vkDescriptorSetLayout_, nullptr);

        vkDestroyBuffer(vkDevice_, vkUniformBuffer_, nullptr);
        vkFreeMemory(vkDevice_, vkUniformBufferMemory_, nullptr);

        vkDestroySampler(vkDevice_, vkTextureSampler_, nullptr);
        vkDestroyImageView(vkDevice_, vkTextureImageView_, nullptr);
        vkDestroyImage(vkDevice_, vkTextureImage_, nullptr);
        vkFreeMemory(vkDevice_, vkTextureImageMemory_, nullptr);

        vkDestroyDescriptorPool(vkDevice_, vkDescriptorPool_, nullptr);

        vkDestroyDevice(vkDevice_, nullptr);
        vkDestroySurfaceKHR(vkInstance_, vkSurface_, nullptr);
        unloadDebugCallback();
        vkDestroyInstance(vkInstance_, nullptr);
    }

    void VulkanGraphicsLib::drawFrame() {
        //在这里更新应用程序状态
       //TODO

        updateUniformBuffer();

        //等待帧
        vkQueueWaitIdle(vkPresentQueue_);

        //从交换链获取图像
        uint32_t imageIndex;
        //第三个参数表示当没有图像准备好时该函数的等待时间，
        VkResult result = vkAcquireNextImageKHR(vkDevice_, vkSwapChain_, std::numeric_limits<uint64_t>::max()
            , vkImageAvailableSemaphore_, VK_NULL_HANDLE, &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        //提交绘制指令
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { vkImageAvailableSemaphore_ };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers_[imageIndex];

        VkSemaphore signalSemaphores[] = { vkRenderFinishedSemaphore_ };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(vkGraphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        //呈现图像 （需要 指定呈现前等待的信号量，要呈现哪些交换链，以及每个交换链的哪个图片)
        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;   //指定发起呈现图像请求前需要等待的信号量
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { vkSwapChain_ };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains; //交换链数组
        presentInfo.pImageIndices = &imageIndex; //其实也是一个数组，长度必须和swapchainCount对应，表示对应交换链要呈现的图像索引
        presentInfo.pResults = nullptr; // Optional
        result = vkQueuePresentKHR(vkPresentQueue_, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
            recreateSwapChain();
        }
        else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
    }

    void VulkanGraphicsLib::deviceWaitIdle() {
        vkDeviceWaitIdle(vkDevice_);
    }

	//创建Vulkan实例
	void VulkanGraphicsLib::createInstance() {
        VkApplicationInfo vkAppInfo = {};
        vkAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        vkAppInfo.pApplicationName = "Sun Engine";
        vkAppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        vkAppInfo.pEngineName = "Sun Engine";
        vkAppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        vkAppInfo.apiVersion = VK_API_VERSION_1_2;

        VkInstanceCreateInfo vkCreateInfo = {};
        vkCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        vkCreateInfo.pApplicationInfo = &vkAppInfo;
        vkCreateInfo.enabledLayerCount = 0;

        std::vector<const char*> extensions = getRequiredExtensions();
        vkCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        vkCreateInfo.ppEnabledExtensionNames = extensions.data();

        if (vkCreateInstance(&vkCreateInfo, nullptr, &vkInstance_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
	}

    void VulkanGraphicsLib::createSurface() {
        vkSurface_ = createPlatformSurface(initStru_.windowHandle, vkInstance_);
    }

    void VulkanGraphicsLib::pickPhysicalDevice() {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(vkInstance_, &deviceCount, nullptr);
        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(vkInstance_, &deviceCount, devices.data());

        for (const auto& device : devices) {
            //isDevicesSuitalbe检查设备是否有图形和显示的队列簇，以及是否支持所有自定义扩展
            if (isDeviceSuitable(device)) {
                vkPhysicalDevice_ = device;
                break;
            }
        }

        if (vkPhysicalDevice_ == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }
    }

    void VulkanGraphicsLib::createLogicalDevice() {
        //我们需要并且硬件设备支持的队列簇索引
        QueueFamilyIndices indices = findQueueFamilies(vkPhysicalDevice_);

        //标记需要的不同类型的队列簇 并 创建逻辑设备
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };
        float queuePriority = 1.0f;
        for (int queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures = {};
        deviceFeatures.samplerAnisotropy = VK_TRUE; //设备支持各项异性采样

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();//&queueCreateInfo;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());//1;
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledLayerCount = 0;
        createInfo.enabledExtensionCount = 0;
        if (g_enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(g_validationLayers.size());
            createInfo.ppEnabledLayerNames = g_validationLayers.data();
        }
        if (g_enableDeviceExtensions) {
            createInfo.enabledExtensionCount = static_cast<uint32_t>(g_deviceExtensions.size());
            createInfo.ppEnabledExtensionNames = g_deviceExtensions.data();
        }

        if (vkCreateDevice(vkPhysicalDevice_, &createInfo, nullptr, &vkDevice_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        //第三个参数0表示该函数会创建一个队列
        vkGetDeviceQueue(vkDevice_, indices.graphicsFamily, 0, &vkGraphicsQueue_);
        vkGetDeviceQueue(vkDevice_, indices.presentFamily, 0, &vkPresentQueue_);
    }

    void VulkanGraphicsLib::createSwapChain(VkSwapchainKHR oldSwapChain) {
        querySurfaceSupport(vkPhysicalDevice_, vkSurface_);

        //选择交换链的图片格式（像素格式)
        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat();
        //选择交换链的窗图片的显示格式(双缓冲等)
        VkPresentModeKHR presentMode = chooseSwapPresentMode();
        //选择图片宽高
        VkExtent2D extent = chooseSwapExtent();

        //选择交换链长度（缓冲图片个数)
        uint32_t imageCount = vkSurfCap_.minImageCount + 1;
        //如果有最大个数限制，并且 最大个数小于希望个数，则令希望个数等于 最大个数
        if (vkSurfCap_.maxImageCount > 0 && imageCount > vkSurfCap_.maxImageCount) {
            imageCount = vkSurfCap_.maxImageCount;
        }

        //构造交换链创建信息
        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = vkSurface_;  //交换链需关联窗口
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        //指定如何处理跨多个队列簇的交换链图像
        QueueFamilyIndices indices = findQueueFamilies(vkPhysicalDevice_);
        uint32_t queueFamilyIndices[] = { (uint32_t)indices.graphicsFamily, (uint32_t)indices.presentFamily };
        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }
        //createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

        createInfo.preTransform = vkSurfCap_.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; //指定alpha通道是否应用与与其他的窗体系统进行混合操
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        //交换链实际就是窗口缓冲buf,当窗口大小变化时,
        //会重新分配交换链，此时需要一个指向老的交换链的引用
        //用来释放内存
        createInfo.oldSwapchain = oldSwapChain;

        //创建交换链
        if (vkCreateSwapchainKHR(vkDevice_, &createInfo, nullptr, &vkSwapChain_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain!");
        }

        //获取交换链包含的图片句柄
        vkGetSwapchainImagesKHR(vkDevice_, vkSwapChain_, &imageCount, nullptr);
        vkSwapChainImages_.resize(imageCount);
        vkGetSwapchainImagesKHR(vkDevice_, vkSwapChain_, &imageCount, vkSwapChainImages_.data());

        vkSurfFormat_ = surfaceFormat;
        vkPresentMode_ = presentMode;
        vkSCExtent_ = extent;

        swapChainImageViews_.resize(vkSwapChainImages_.size());
        //为交换链的每个图像创建一个ImageView
        for (int i = 0; i < vkSwapChainImages_.size(); ++i) {
            swapChainImageViews_[i] = createImageView(vkSwapChainImages_[i], vkSurfFormat_.format, VK_IMAGE_ASPECT_COLOR_BIT);
        }
    }

    void VulkanGraphicsLib::createRenderPass() {
        /*********************************附件配置*********************************/
        //深度缓冲附件
        VkAttachmentDescription depthAttachment = {};
        depthAttachment.format = findDepthFormat();
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        //颜色缓冲附件
        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = vkSurfFormat_.format;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;//无多重采样
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; //绘前清空（清空为默认色)，个人理解含义是这么配置了gpu就这么处理
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; //绘制后保留，用于展示
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; //renderpass开始时的初始的内存布局
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;  //renderpass结束后自动转化为的布局
        //以上附件存入数组
        std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
        /******************************************************************************/

        /*************************子通道配置 --begin *****************************/
        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

        //颜色附件引用
        VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0; // 对应attachments中的colorAttachment
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        
        //深度附件引用
        VkAttachmentReference depthAttachmentRef = {};
        depthAttachmentRef.attachment = 1; //对应attachments中的depthAttachment
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        //该附件在该子通道中的索引将被直接对应着色器中的layout的索引
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef; //深度附件只能有一个，因此无count参数
        /***********************************************************************/

        /********************************子通道依赖配置***********************************/
        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0; //目标子通道索引，这里只有一个子通道，因此是0
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        /***********************************************************************/

        /********************************根据附件和子通道创建renderPass*************************************/
        //综上，可定义若干附件，若干子通道，若干子通道依赖，每个子通道通过附件引用（一个附件引用只能引用一个附件)引用若干附件；
        //每个子通道依赖引用两个子通道 （个人理解，若有子通道a,b,c,则依赖应该是ab,bc)
        //最后一个renderPassCreateInfo把以上信息合并构建renderPass
        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(vkDevice_, &renderPassInfo, nullptr, &vkRenderPass_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass!");
        }
        /******************************************************************************************/
    }

    void VulkanGraphicsLib::createTextureSampler() {
        VkSamplerCreateInfo samplerInfo = {};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

        samplerInfo.anisotropyEnable = VK_TRUE; //开启各向异性采样（性能换效果)
        samplerInfo.maxAnisotropy = 16;   //计算最终颜色的最大文素采样数量

        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK; //边缘寻址下对应的颜色（非任意)

        samplerInfo.unnormalizedCoordinates = VK_FALSE; //非归一化坐标

        samplerInfo.compareEnable = VK_FALSE;   //如果开启比较功能，那么纹素首先和值进行比较，并且比较后的值用于过滤操作。。主要用在阴影纹理映射的percentage-closer filtering 即百分比近似过滤器。我们会在未来的章节中看到。
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        if (vkCreateSampler(vkDevice_, &samplerInfo, nullptr, &vkTextureSampler_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }


    }

    std::vector<const char*> VulkanGraphicsLib::getRequiredExtensions()
    {
        std::vector<const char*> extensions;

        unsigned int glfwExtensionCount = 0;
        const char** glfwExtensions;

        //glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        /*for (unsigned int i = 0; i < glfwExtensionCount; i++) {
            extensions.push_back(glfwExtensions[i]);
        }*/
        extensions = initStru_.extensions; //from glfw 

        if (g_enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
            //extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    void VulkanGraphicsLib::setupDebugCallback()
    {
        VkDebugReportCallbackCreateInfoEXT vkDbgCreateInfo = {};
        vkDbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        vkDbgCreateInfo.flags = -VK_DEBUG_REPORT_ERROR_BIT_EXT | -VK_DEBUG_REPORT_WARNING_BIT_EXT;
        vkDbgCreateInfo.pfnCallback = debugCallback;

        auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(vkInstance_, "vkCreateDebugReportCallbackEXT");
        VkResult result = VK_ERROR_EXTENSION_NOT_PRESENT;
        if (func != nullptr) {
            result = func(vkInstance_, &vkDbgCreateInfo, nullptr, &vkCallback_);
        }

        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug callback!");
        }
    }

    void VulkanGraphicsLib::unloadDebugCallback()
    {
        auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(vkInstance_, "vkDestroyDebugReportCallbackEXT");
        if (func != nullptr) {
            func(vkInstance_, vkCallback_, nullptr);
        }
    }

    bool VulkanGraphicsLib::isDeviceSuitable(VkPhysicalDevice device) {
        bool ok = true;

        ////获取设备基本属性
        //VkPhysicalDeviceProperties deviceProperties = {};
        //vkGetPhysicalDeviceProperties(device, &deviceProperties);

        ////获取设备特征
        VkPhysicalDeviceFeatures deviceFeatures = {};
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        //找到支持图形和显示的队列簇
        QueueFamilyIndices indices = findQueueFamilies(device);

        bool extensionsSupported = checkDeviceExtensionSupport(device, g_deviceExtensions);

        return indices.isComplete() && extensionsSupported && deviceFeatures.samplerAnisotropy;

        return ok;
    }

    QueueFamilyIndices VulkanGraphicsLib::findQueueFamilies(VkPhysicalDevice physicalDevice) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        //从该物理设备支持的队列簇中找出一个满足图形处理的队列簇 和 一个支持显示的队列簇，索引存入indices;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, vkSurface_, &presentSupport);
            if (queueFamily.queueCount > 0 && presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.isComplete()) {
                break;
            }

            i++;
        }

        return indices;
    }

    //检查物理设备是否支持 列表中的拓展
    bool VulkanGraphicsLib::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice, const std::vector<const char*>& extensions) {
        //枚举出设备支持的所有扩展
        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(extensions.begin(), extensions.end());
        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    void VulkanGraphicsLib::querySurfaceSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
        vkSurfCap_ = {};
        vkSurfFormatKHRVec_.clear();
        vkPresentModeKHRVec_.clear();

        //获取交换链支持的窗口参数（宽高，范围等)
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &vkSurfCap_);

        //获取交换链支持的窗口格式（像素格式等)
        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
        if (formatCount != 0) {
            vkSurfFormatKHRVec_.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, vkSurfFormatKHRVec_.data());
        }

        //获取交换链支持的显示模式（双缓冲，三缓冲等)
        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
        if (presentModeCount != 0) {
            vkPresentModeKHRVec_.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, vkPresentModeKHRVec_.data());
        }

        return;
    }

    //选择像素格式
    VkSurfaceFormatKHR VulkanGraphicsLib::chooseSwapSurfaceFormat() {
        //如果任意支持，则选择期望的;
        if (vkSurfFormatKHRVec_.size() == 1 && vkSurfFormatKHRVec_[0].format == VK_FORMAT_UNDEFINED) {
            return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
        }
        //如果不能自由选择，则遍历列表选择偏向性格式
        for (const auto& availableFormat : vkSurfFormatKHRVec_) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }
        return vkSurfFormatKHRVec_[0];
    }

    //选择显示模式
    VkPresentModeKHR VulkanGraphicsLib::chooseSwapPresentMode() {
        //return VK_PRESENT_MODE_FIFO_KHR;

        //优先检查三级缓冲模式
        //for (const auto& availablePresentMode : vkPresentModeKHRVec_) {
        //    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
        //        return availablePresentMode;
        //    }
        //}

        //双缓冲模式
        for (const auto& availablePresentMode : vkPresentModeKHRVec_) {
            if (availablePresentMode == VK_PRESENT_MODE_FIFO_KHR) {
                return availablePresentMode;
            }
        }

        //立即模式
        for (const auto& availablePresentMode : vkPresentModeKHRVec_) {
            if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_IMMEDIATE_KHR;
    }

    //选择分辨率
    VkExtent2D VulkanGraphicsLib::chooseSwapExtent() {
        if (vkSurfCap_.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return vkSurfCap_.currentExtent;
        }
        else {
            VkExtent2D actualExtent = { initStru_.width, initStru_.height };

            actualExtent.width = std::max(vkSurfCap_.minImageExtent.width, std::min(vkSurfCap_.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(vkSurfCap_.minImageExtent.height, std::min(vkSurfCap_.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    void VulkanGraphicsLib::createDescriptorPool() {

        std::array<VkDescriptorPoolSize, 2> poolSizes = {};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = 1;
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = 1;

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());;
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = 1;

        if (vkCreateDescriptorPool(vkDevice_, &poolInfo, nullptr, &vkDescriptorPool_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }

    void VulkanGraphicsLib::createDescriptorSet() {
        VkDescriptorSetLayout layouts[] = { vkDescriptorSetLayout_ };
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = vkDescriptorPool_;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = layouts;

        if (vkAllocateDescriptorSets(vkDevice_, &allocInfo, &vkDescriptorSet_) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor set!");
        }

        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = vkUniformBuffer_;
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo = {};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = vkTextureImageView_;
        imageInfo.sampler = vkTextureSampler_;

        std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = vkDescriptorSet_;
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;
        descriptorWrites[0].pImageInfo = nullptr; // Optional
        descriptorWrites[0].pTexelBufferView = nullptr; // Optional

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = vkDescriptorSet_;
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;


        vkUpdateDescriptorSets(vkDevice_, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

    //个人理解，其实就是配置shader中的uniform变量
    void VulkanGraphicsLib::createDescriptorSetLayout() {
        VkDescriptorSetLayoutBinding uboLayoutBinding = {};
        uboLayoutBinding.binding = 0; //对应shader中的"layout (binding = 0) "
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //表示是一个uniform类型
        uboLayoutBinding.descriptorCount = 1; //对应shader中uniform数组的长度（如果是数组的话,单个变量就是1)
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; //标记描述符在着色器程序的哪个阶段使用,这里是顶点着色器
        uboLayoutBinding.pImmutableSamplers = nullptr; //图像采样相关

        VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
        samplerLayoutBinding.binding = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; //sampler类型
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        samplerLayoutBinding.pImmutableSamplers = nullptr;

        std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(vkDevice_, &layoutInfo, nullptr, &vkDescriptorSetLayout_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
    }

    //创建图形管线，对应opengl的shaderProgram，但可配置的信息更多
    void VulkanGraphicsLib::createGraphicsPipeline()
    {
#if 1
        /******************************配置着色器********************/
        auto vertShaderCode = readFile("shaders/vert.spv");
        auto fragShaderCode = readFile("shaders/frag.spv");

        VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";
        //vertShaderStageInfo.pSpecializationInfo;//for 设置常量

        VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        //管线着色器步骤创建信息数组（只是对shader的wrapper，附加上 主函数，类型信息)
        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
        /*******************************************************************/


        /*******************************配置顶点缓冲布局信息，对应opengl的VertexLayout*****************************/
        //对应opengl的布局信息中的 stride
        VkVertexInputBindingDescription bindingDescription = vertex::getBindingDescription();
        //数组每个元素对应opengl的layout中单个顶点的格式描述
        std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions = vertex::getAttributeDescriptions();

        //将上面两个信息组合一下
        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Optional
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data(); // Optional
        /**************************************************************


        /*************************配置绘制的topology*************************/
        //个人理解表示怎么绘制三角形（对应opengl的GL_TRIANGLES等)
        //或者对应D3D的IA着色器
        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;
        /*********************************************************************************/


        /****************************视窗和裁剪,对应opengl的glViewport**************************************/
        VkPipelineViewportStateCreateInfo viewportState = {};
        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)vkSCExtent_.width;
        viewport.height = (float)vkSCExtent_.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        //裁剪矩形
        VkRect2D scissor = {};
        scissor.offset = { 0, 0 };
        scissor.extent = vkSCExtent_;

        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.pScissors = &scissor;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.viewportCount = 1;
        /******************************************************************************/

        /*****************************************光栅化配置******************************************/
        //对应opengl那些设置线宽，面剔除，多边形渲染方式，深度检测开关等函数
        VkPipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;//开启之后将禁止光栅化
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;//填充，描边和描点三种
        rasterizer.lineWidth = 1.0f;//描述线宽，大于1.0的需要开启GPU的wideline特性支持
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;  //面剔除
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;//VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE; //光栅化可以通过添加常量或者基于片元的斜率来更改深度值
        rasterizer.depthBiasConstantFactor = 0.0f; // Optional
        rasterizer.depthBiasClamp = 0.0f; // Optional
        rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

        /***********************************配置多重采样参数（该配置未多重采样)**************************/
        //对应opengl的多重采样配置
        VkPipelineMultisampleStateCreateInfo multisampling = {};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f; // Optional
        multisampling.pSampleMask = nullptr; // Optional
        multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
        multisampling.alphaToOneEnable = VK_FALSE; // Optional

        /***********************************深度测试和模板测试***********************************/
        //对应opengl的深度测试和模板测试配置
        VkPipelineDepthStencilStateCreateInfo depthStencil = {};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f; // Optional
        depthStencil.maxDepthBounds = 1.0f; // Optional
        depthStencil.stencilTestEnable = VK_FALSE;  //模板用
        depthStencil.front = {}; // Optional
        depthStencil.back = {}; // Optional

        /************************************颜色混合*************************************/
        //计算公式为:
        //finalColor.rgb= srcColorBlendFactor* newColor.rgb <colorBlendOp> dstColorBlendFactor* oldColor.rgb
        //finalColor.a = srcAlphaBlendFactor * newColor.a <alphaBlendOp> dstAlphaBlendFactor * oldColor.a
        //(srcBlendFactor * dst <blendOp> dstBlendFactor * src)& colorWriteMask
        //finalColor = blendEnable? finalColor:newColor;
        //finalColor = finalColor&colorWriteMask

        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

        //VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
        ////colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        //colorBlendAttachment.blendEnable = VK_TRUE;
        //colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        //colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        //colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        //colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        //colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        //colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        //以上等价为常规的alpha blending : finalColor.rgb = newAlpha * newColor + (1 - newAlpha) * oldColor;
        //finalColor.a = newAlpha.a;

        VkPipelineColorBlendStateCreateInfo colorBlending = {};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f; // Optional
        colorBlending.blendConstants[1] = 0.0f; // Optional
        colorBlending.blendConstants[2] = 0.0f; // Optional
        colorBlending.blendConstants[3] = 0.0f; // Optional
        /***********************************************************************************/

        /**************************************动态修改******************************************/
        //意思是这些枚举信息对应的变量可以在运行时动态修改
        VkDynamicState dynamicStates[] = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_LINE_WIDTH
        };
        VkPipelineDynamicStateCreateInfo dynamicState = {};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = 2;
        dynamicState.pDynamicStates = dynamicStates;
        /************************************************************************/


        /****************************************创建管道布局********************************/
        //对应opengl的一些uniform等变量的配置
        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1; // Optional
        pipelineLayoutInfo.pSetLayouts = &vkDescriptorSetLayout_; // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

        if (vkCreatePipelineLayout(vkDevice_, &pipelineLayoutInfo, nullptr, &vkPipelineLayout_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
        /************************************************************************/


        /******************************创建管线缓存***********************************/
        //意义不明，基本默认创建一下就好
        VkPipelineCache pipelineCache;
        VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
        pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        if (vkCreatePipelineCache(vkDevice_, &pipelineCacheCreateInfo, NULL, &pipelineCache) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }
        /************************************************************************/

        /******************************创建图形管线***********************************/
        //以上信息全部整合提交，也就是创建图形管线，但是这个函数需要修改，因为renderPass也需要提交
        //可见图形管线包含了renderPass，所以以上代码应该封装为函数
        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        //着色器创建信息数组长度
        pipelineInfo.stageCount = 2;
        //着色器创建信息数组
        pipelineInfo.pStages = shaderStages;

        //顶点的布局信息(如果是mesh shader则忽略)
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        //绘制三角形的方法，对应D3D的IA,opengl的GL_TRIANGLS
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        //描述管线怎么剪裁视窗（如果不是光栅化管线则忽略)
        pipelineInfo.pViewportState = &viewportState;
        //光栅化配置信息
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = nullptr; // Optional
        pipelineInfo.pTessellationState = nullptr;
        pipelineInfo.layout = vkPipelineLayout_;
        //renderPass可简单理解为一堆附件，内含多个subPass，表示这些附件的使用关系，
        pipelineInfo.renderPass = vkRenderPass_;
        //指定renderPass中的subPass的索引，表示这个管线将使用这个subPass
        pipelineInfo.subpass = 0;

        //basePipelineHandle，个人理解，可以利用现有的管线创建新管线
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex = -1; // Optional

        if (vkCreateGraphicsPipelines(vkDevice_, pipelineCache /*VK_NULL_HANDLE*/, 1, &pipelineInfo, nullptr, &vkGraphicsPipeline_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }

        //释放内存
        vkDestroyShaderModule(vkDevice_, fragShaderModule, nullptr);
        vkDestroyShaderModule(vkDevice_, vertShaderModule, nullptr);
#endif

    }

    //交换链创建了三张gpu图片，每张图片对应一个gpu ImageView句柄，
    //同时还创建了一张深度图片（以及关联的imageView)
    //现在创建3个framebuffer，每个framebuffer关联两个附件，其中之1是对应序号的gpuImageView
    //另一个是相同的depthView
    void VulkanGraphicsLib::createFramebuffers()
    {
        swapChainFramebuffers_.resize(swapChainImageViews_.size());

        for (size_t i = 0; i < swapChainImageViews_.size(); i++) {
            /*           VkImageView attachments[] = {
                           swapChainImageViews[i]
                       };*/
            std::array<VkImageView, 2> attachments = {
                swapChainImageViews_[i],
                vkDepthImageView_
            };

            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = vkRenderPass_;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());;
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = vkSCExtent_.width;
            framebufferInfo.height = vkSCExtent_.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(vkDevice_, &framebufferInfo, nullptr, &swapChainFramebuffers_[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }

    void VulkanGraphicsLib::createCommandPool()
    {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(vkPhysicalDevice_);

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
        poolInfo.flags = 0; // Optional
        //VK_COMMAND_POOL_CREATE_TRANSIENT_BIT: 提示命令缓冲区非常频繁的重新记录新命令(可能会改变内存分配行为)
        // VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT:允许命令缓冲区单独重新记录，没有这个标志，所有的命令缓冲区都必须一起重置
        if (vkCreateCommandPool(vkDevice_, &poolInfo, nullptr, &vkCommandPool_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool!");
        }
    }

    void VulkanGraphicsLib::createTextureImage() {
        int texWidth, texHeight, texChannels;
        //stbi_uc* pixels = stbi_load("textures/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        stbi_uc* pixels = stbi_load(TEXTURE_PATH.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        mipLevels_ = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
        if (!pixels) {
            throw std::runtime_error("failed to load texture image!");
        }

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(vkDevice_, stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(vkDevice_, stagingBufferMemory);

        stbi_image_free(pixels);

        createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkTextureImage_, vkTextureImageMemory_);

        transitionImageLayout(vkTextureImage_, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED/*VK_IMAGE_LAYOUT_PREINITIALIZED*/, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        copyBufferToImage(stagingBuffer, vkTextureImage_, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

        transitionImageLayout(vkTextureImage_, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);



        vkDestroyBuffer(vkDevice_, stagingBuffer, nullptr);
        vkFreeMemory(vkDevice_, stagingBufferMemory, nullptr);

    }

    void VulkanGraphicsLib::createTextureImageView() {
        vkTextureImageView_ = createImageView(vkTextureImage_, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
    }

    //创建图像视图
    VkImageView VulkanGraphicsLib::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(vkDevice_, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture image view!");
        }

        return imageView;

    }

    //创建gpu的图片(纹理)
    void VulkanGraphicsLib::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;  //ty change //VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.flags = 0; // Optional

        if (vkCreateImage(vkDevice_, &imageInfo, nullptr, &image) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(vkDevice_, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(vkDevice_, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate image memory!");
        }

        vkBindImageMemory(vkDevice_, image, imageMemory, 0);
    }

    void VulkanGraphicsLib::createVertexBuffer() {
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(vkDevice_, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(vkDevice_, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkVertexBuffer_, vkVertexBufferMemory_);

        copyBuffer(stagingBuffer, vkVertexBuffer_, bufferSize);

        vkDestroyBuffer(vkDevice_, stagingBuffer, nullptr);
        vkFreeMemory(vkDevice_, stagingBufferMemory, nullptr);
    }

    void VulkanGraphicsLib::createIndexBuffer() {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(vkDevice_, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (size_t)bufferSize);
        vkUnmapMemory(vkDevice_, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkIndexBuffer_, vkIndexBufferMemory_);

        copyBuffer(stagingBuffer, vkIndexBuffer_, bufferSize);

        vkDestroyBuffer(vkDevice_, stagingBuffer, nullptr);
        vkFreeMemory(vkDevice_, stagingBufferMemory, nullptr);
    }

    void VulkanGraphicsLib::createUniformBuffer() {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vkUniformBuffer_, vkUniformBufferMemory_);
    }

    void VulkanGraphicsLib::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size; //byteSize
        bufferInfo.usage = usage; //指定该buffer用于顶点处理（也可以支持多个掩码)
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; //专有

        //个人理解，创建buffer的时候没有分配内存，因为在创建之后才指定了内存需求，这和opengl的genBuffer很有可能
        //是类似的
        if (vkCreateBuffer(vkDevice_, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create vertex buffer!");
        }

        VkMemoryRequirements memRequirements = {};
        vkGetBufferMemoryRequirements(vkDevice_, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(vkDevice_, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate vertex buffer memory!");
        }
        vkBindBufferMemory(vkDevice_, buffer, bufferMemory, 0);
    }

    void VulkanGraphicsLib::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkBufferCopy copyRegion = {};
        copyRegion.srcOffset = 0; // Optional
        copyRegion.dstOffset = 0; // Optional
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        endSingleTimeCommands(commandBuffer);
    }

    void VulkanGraphicsLib::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkBufferImageCopy region = {};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;

        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = {
            width,
            height,
            1
        };

        vkCmdCopyBufferToImage(
            commandBuffer,
            buffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
        );

        endSingleTimeCommands(commandBuffer);
    }

    void VulkanGraphicsLib::createDepthResources() {

        VkFormat depthFormat = findDepthFormat();

        createImage(vkSCExtent_.width, vkSCExtent_.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vkDepthImage_, vkDepthImageMemory_);
        vkDepthImageView_ = createImageView(vkDepthImage_, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

        //transitionImageLayout(depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);


    }

    void VulkanGraphicsLib::cleanupSwapChain() {

        vkDestroyImageView(vkDevice_, vkDepthImageView_, nullptr);
        vkDestroyImage(vkDevice_, vkDepthImage_, nullptr);
        vkFreeMemory(vkDevice_, vkDepthImageMemory_, nullptr);

        for (size_t i = 0; i < swapChainFramebuffers_.size(); i++) {
            vkDestroyFramebuffer(vkDevice_, swapChainFramebuffers_[i], nullptr);
        }

        vkFreeCommandBuffers(vkDevice_, vkCommandPool_, static_cast<uint32_t>(commandBuffers_.size()), commandBuffers_.data());

        vkDestroyPipeline(vkDevice_, vkGraphicsPipeline_, nullptr);
        vkDestroyPipelineLayout(vkDevice_, vkPipelineLayout_, nullptr);
        vkDestroyRenderPass(vkDevice_, vkRenderPass_, nullptr);

        for (size_t i = 0; i < swapChainImageViews_.size(); i++) {
            vkDestroyImageView(vkDevice_, swapChainImageViews_[i], nullptr);
        }

        vkDestroySwapchainKHR(vkDevice_, vkSwapChain_, nullptr);
    }

    VkCommandBuffer VulkanGraphicsLib::beginSingleTimeCommands() {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = vkCommandPool_;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(vkDevice_, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
    }

    void VulkanGraphicsLib::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(vkGraphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(vkGraphicsQueue_);

        vkFreeCommandBuffers(vkDevice_, vkCommandPool_, 1, &commandBuffer);
    }

    //变换图像布局
    void VulkanGraphicsLib::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkImageMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;

        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        barrier.srcAccessMask = 0; // TODO
        barrier.dstAccessMask = 0; // TODO

        vkCmdPipelineBarrier(
            commandBuffer,
            0 /* TODO */, 0 /* TODO */,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );
        /// <summary>

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        }
        else {
            throw std::invalid_argument("unsupported layout transition!");
        }

        //深度相关，这种逻辑写法不太对，耦合太厉害
        if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

            if (hasStencilComponent(format)) {
                barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }
        }
        else {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        }

        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

        endSingleTimeCommands(commandBuffer);
    }

    VkShaderModule VulkanGraphicsLib::createShaderModule(const std::vector<char>& code)
    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();

        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
        VkShaderModule shaderModule;
        if (vkCreateShaderModule(vkDevice_, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }
        return shaderModule;
    }

    //从输入的format中寻找对应tiling的参数，符合features的format
    VkFormat VulkanGraphicsLib::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {

        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice_, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            }
            else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }
        throw std::runtime_error("failed to find supported format!");
        return VkFormat();
    }

    VkFormat VulkanGraphicsLib::findDepthFormat() {
        return findSupportedFormat(
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );
    }

    bool VulkanGraphicsLib::hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

    uint32_t VulkanGraphicsLib::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevice_, &memProperties);
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }

    void VulkanGraphicsLib::createCommandBuffers()
    {
        //分配命令缓冲区
        commandBuffers_.resize(swapChainFramebuffers_.size());

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = vkCommandPool_;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)commandBuffers_.size();

        if (vkAllocateCommandBuffers(vkDevice_, &allocInfo, commandBuffers_.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        //启动命令缓冲区
        for (size_t i = 0; i < commandBuffers_.size(); i++) {
            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            //命令缓冲区将在执行一次后立即重新记录
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
            beginInfo.pInheritanceInfo = nullptr; // Optional

            vkBeginCommandBuffer(commandBuffers_[i], &beginInfo);

            //启动渲染通道
            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = vkRenderPass_;
            renderPassInfo.framebuffer = swapChainFramebuffers_[i];
            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = vkSCExtent_;

            std::array<VkClearValue, 2> clearValues = {};
            clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
            clearValues[1].depthStencil = { 1.0f, 0 };

            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers_[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindDescriptorSets(commandBuffers_[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout_, 0, 1, &vkDescriptorSet_, 0, nullptr);

            vkCmdBindPipeline(commandBuffers_[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vkGraphicsPipeline_);


            VkBuffer vertexBuffers[] = { vkVertexBuffer_ };
            VkDeviceSize offsets[] = { 0 };
            vkCmdBindVertexBuffers(commandBuffers_[i], 0, 1, vertexBuffers, offsets);
            vkCmdBindIndexBuffer(commandBuffers_[i], vkIndexBuffer_, 0, VK_INDEX_TYPE_UINT32);

            //vkCmdDraw(commandBuffers[i], static_cast<uint32_t>(vertices.size()), 1, 0, 0);
            vkCmdDrawIndexed(commandBuffers_[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

            vkCmdEndRenderPass(commandBuffers_[i]);

            if (vkEndCommandBuffer(commandBuffers_[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        }


    }

    void VulkanGraphicsLib::createSemaphores()
    {
        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        if (vkCreateSemaphore(vkDevice_, &semaphoreInfo, nullptr, &vkImageAvailableSemaphore_) != VK_SUCCESS ||
            vkCreateSemaphore(vkDevice_, &semaphoreInfo, nullptr, &vkRenderFinishedSemaphore_) != VK_SUCCESS) {
            throw std::runtime_error("failed to create semaphores!");
        }

    }

    void VulkanGraphicsLib::updateUniformBuffer() {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() / 1000.0f;


        UniformBufferObject ubo = {};
#if 0 //FIX ME
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        ubo.proj = glm::perspective(glm::radians(45.0f), vkSCExtent.width / (float)vkSCExtent.height, 0.1f, 10.0f);
        //GLM最初是为OpenGL设计的，它的裁剪坐标的Y是反转的
        ubo.proj[1][1] *= -1;
#endif

        void* data;
        vkMapMemory(vkDevice_, vkUniformBufferMemory_, 0, sizeof(ubo), 0, &data);
        memcpy(data, &ubo, sizeof(ubo));
        vkUnmapMemory(vkDevice_, vkUniformBufferMemory_);
    }

    void VulkanGraphicsLib::loadModel() {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string err;
        std::string warn;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str())) {
            throw std::runtime_error(err);
        }

        //清空顶点 缓存和 索引缓存
        vertices.clear();
        indices.clear();
        std::unordered_map<vertex, uint32_t> uniqueVertices = {};
        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                vertex vertex = {};

                vertex._pt[0] = attrib.vertices[3 * index.vertex_index + 0];
                vertex._pt[1] = attrib.vertices[3 * index.vertex_index + 1];
                vertex._pt[2] = attrib.vertices[3 * index.vertex_index + 2];

                vertex._textureCoord[0] = attrib.texcoords[2 * index.texcoord_index];
                vertex._textureCoord[1] = 1.0 - attrib.texcoords[2 * index.texcoord_index + 1];

                vertex._rgba[0] = 1.0f;
                vertex._rgba[1] = 1.0f;
                vertex._rgba[2] = 1.0f;

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }
                indices.push_back(uniqueVertices[vertex]);

                //vertices.push_back(vertex);
                //indices.push_back(indices.size());
            }
        }
    }
}