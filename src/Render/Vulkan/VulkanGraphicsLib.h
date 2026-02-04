#ifndef __VULKAN_GRAPHICS_LIB_H
#define __VULKAN_GRAPHICS_LIB_H

#include "Render/GraphicsLib.h"
#include "Math/Vector3D.h"
#include "Math/Matrix4x4.h"
#include <array>

#define GLFW_INCLUDE_VULKAN
#include "vulkan/vulkan_core.h"

#include "nvvk/swapchain_vk.hpp"

namespace Sun {

	typedef struct VulkanInit_s
	{
		void* windowHandle;
		int width;
		int height;
		std::vector<const char*> extensions;

	}VulkanInit_t;

	struct QueueFamilyIndices {
		int graphicsFamily = -1;
		int presentFamily = -1;

		bool isComplete() {
			return graphicsFamily >= 0 && presentFamily >= 0;
		}
	};

	extern const std::string MODEL_PATH;
	extern const std::string TEXTURE_PATH;

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct UniformBufferObject {
		Matrix4x4 model;
		Matrix4x4 view;
		Matrix4x4 proj;
	};

	class VulkanGraphicsLib :public GraphicsLib {
	public:
		void initEnvironment() override;
		void bindVertexArray(unsigned int id) override;

		void recreateSwapChain();
		void setExtend(const VkExtent2D& extend);
		void cleanup();
		void drawFrame();
		void deviceWaitIdle();
	protected:
		//创建Vulkan实例
		void createInstance();
		//创建窗口,vulkan用surface来抽象窗口
		void createSurface();
		//创建物理设备
		void pickPhysicalDevice();
		//创建逻辑设备
		void createLogicalDevice();
		//创建交换链
		void createSwapChain(VkSwapchainKHR oldSwapChain);
		//创建渲染通道
		void createRenderPass();
		//创建纹理采样器
		void createTextureSampler();
		//创建描述符池
		void createDescriptorPool();
		//描述符集合
		void createDescriptorSet();
		//创建描述符布局
		void createDescriptorSetLayout();
		//创建图形管线
		void createGraphicsPipeline();
		//创建帧缓冲区
		void createFramebuffers();
		//创建命令池
		void createCommandPool();
		//创建纹理图像
		void createTextureImage();
		//创建图形视图
		void createTextureImageView();
		//创建图像
		void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		//创建顶点缓冲区
		void createVertexBuffer();
		//创建索引缓冲区
		void createIndexBuffer();
		//创建uniform缓冲
		void createUniformBuffer();
		//创建buffer
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		//copy buffer
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		//copy buffer to image
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		//创建深度资源
		void createDepthResources();
		//清空交换链
		void cleanupSwapChain();


		VkCommandBuffer beginSingleTimeCommands();
		void endSingleTimeCommands(VkCommandBuffer commandBuffer);
		//变换图像布局
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		//创建命令缓冲区数组
		void createCommandBuffers();
		//创建信号量
		void createSemaphores();

		void updateUniformBuffer();


		//查找顶点缓冲区内存了理性
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		//加载回调函数
		VkResult loadCallBack(const VkAllocationCallbacks* pAllocator);

		void loadModel();
	private:
		std::vector<const char*> getRequiredExtensions();
		void setupDebugCallback();
		void unloadDebugCallback();
		bool isDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice);
		//检查物理设备是否支持 列表中的拓展
		bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice , const std::vector<const char*>& extensions);
		//查询窗口支持的 窗口像素格式，显示模式，和分辨率
		void querySurfaceSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
		//选择像素格式
		VkSurfaceFormatKHR chooseSwapSurfaceFormat();
		//选择显示模式
		VkPresentModeKHR chooseSwapPresentMode();
		//选择分辨率
		VkExtent2D chooseSwapExtent();
		//创建图像视图
		VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags);
		//查找支持的格式
		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat findDepthFormat();
		VkShaderModule createShaderModule(const std::vector<char>& code);
		bool hasStencilComponent(VkFormat format);
	protected:
		//vulkan实例，全局唯一
		VkInstance vkInstance_;
		//选中的硬件设备(gpu等)
		VkPhysicalDevice vkPhysicalDevice_ = VK_NULL_HANDLE;
		//从硬件设备派生出的逻辑设备，常用于函数默认输入参数
		VkDevice vkDevice_;
		//Vulkan扩展，对窗口的抽象，因为和平台有关，所以有KHR后缀，其创建逻辑平台相关，因此位于别处
		VkSurfaceKHR vkSurface_;
		
		//图形队列
		VkQueue vkGraphicsQueue_;
		//显示队列
		VkQueue vkPresentQueue_;
		//交换链
		VkSwapchainKHR vkSwapChain_;

		//渲染通道
		VkRenderPass vkRenderPass_;

		//交换链关联的图像
		std::vector<VkImage> vkSwapChainImages_;
		//交换链关联的图像的视图
		std::vector<VkImageView> swapChainImageViews_;
		//帧缓冲区
		std::vector<VkFramebuffer> swapChainFramebuffers_;
		//选中的像素格式
		VkSurfaceFormatKHR vkSurfFormat_;
		//选中的显示模式
		VkPresentModeKHR vkPresentMode_;
		//选中的分辨率
		VkExtent2D vkSCExtent_;
		
		//窗口支持的基础曲面能力（宽度范围，高度范围等)
		VkSurfaceCapabilitiesKHR vkSurfCap_;
		//窗口支持的surface格式（像素格式，颜色空间) 用户从中任意选择一个作为创建参数
		std::vector<VkSurfaceFormatKHR> vkSurfFormatKHRVec_;
		//窗口支持的显示模式
		std::vector<VkPresentModeKHR> vkPresentModeKHRVec_;

		//描述符集合布局
		VkDescriptorSetLayout vkDescriptorSetLayout_;
		//描述符池
		VkDescriptorPool vkDescriptorPool_;
		//描述符集合
		VkDescriptorSet vkDescriptorSet_;

		//管道布局（可以指定uniform，即shaders中的全局变量)
		VkPipelineLayout vkPipelineLayout_;
		//图形管线
		VkPipeline vkGraphicsPipeline_;
		
		//命令池
		VkCommandPool vkCommandPool_;
		//命令缓冲区数组
		std::vector<VkCommandBuffer> commandBuffers_;

		VkSemaphore vkImageAvailableSemaphore_;
		VkSemaphore vkRenderFinishedSemaphore_;

		//debug回调句柄
		VkDebugReportCallbackEXT vkCallback_;
		VulkanInit_s initStru_;

		//////////////////这些代码只是测试用，后续要移除////////////////////
	public:
		struct vertex {
			template<typename _T>
			friend class std::hash;
			vec3 _pt;
			vec4 _rgba; 
			vec2 _textureCoord;
			vec3 _normal;

			bool operator==(const vertex& other) const {
				return (_pt[0] == other._pt[0] && _pt[1] == other._pt[1] && _pt[2] == other._pt[2]) &&
					(_rgba[0] == other._rgba[0] && _rgba[1] == other._rgba[1] && _rgba[2] == other._rgba[2]) &&
					(_textureCoord == other._textureCoord);
			}

			static VkVertexInputBindingDescription getBindingDescription() {
				VkVertexInputBindingDescription bindingDescription = {};
				bindingDescription.binding = 0; //
				bindingDescription.stride = sizeof(vertex);  //数组中相邻元素字节间隔
				bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

				return bindingDescription;
			}
			static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() {
				std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions = {};

				attributeDescriptions[0].binding = 0;
				attributeDescriptions[0].location = 0; //对应顶点着色器的location
				attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[0].offset = offsetof(vertex, _pt);

				attributeDescriptions[1].binding = 0;
				attributeDescriptions[1].location = 1; //对应顶点着色器的location
				attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
				attributeDescriptions[1].offset = offsetof(vertex, _rgba);

				attributeDescriptions[2].binding = 0;
				attributeDescriptions[2].location = 2; //对应顶点着色器的location
				attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
				attributeDescriptions[2].offset = offsetof(vertex, _textureCoord);

				attributeDescriptions[3].binding = 0;
				attributeDescriptions[3].location = 3; //对应顶点着色器的location
				attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[3].offset = offsetof(vertex, _normal);

				return attributeDescriptions;
			}
		};

		std::vector<vertex> vertices = {
			{vec3{-0.5f, -0.5f, 0.0f},vec4{1.0f, 0.0f, 0.0f ,1.0f},{1,0},{0,0,0}},
			{{0.5f, -0.5f,0.0f}, {0.0f, 1.0f, 0.0f ,1.0f},{0,0},{0,0,0}},
			{{0.5f, 0.5f,0.0f}, {0.0f, 0.0f, 1.0f,1.0f},{0,1},{0,0,0}},
			{{-0.5f, 0.5f,0.0f}, {1.0f, 1.0f, 1.0f,1.0f},{1,1},{0,0,0}},

			{{-0.5f, -0.5f, -0.5f},{1.0f, 0.0f, 0.0f ,1.0f},{1,0},{0,0,0}},
			{{0.5f, -0.5f,-0.5f}, {0.0f, 1.0f, 0.0f ,1.0f},{0,0},{0,0,0}},
			{{0.5f, 0.5f,-0.5f}, {0.0f, 0.0f, 1.0f,1.0f},{0,1},{0,0,0}},
			{{-0.5f, 0.5f,-0.5f}, {1.0f, 1.0f, 1.0f,1.0f},{1,1},{0,0,0}},
		};
		std::vector<uint32_t> indices = {
			0, 1, 2, 2, 3, 0 ,
			4, 5, 6, 6, 7, 4
		};
	protected:
		VkBuffer vkVertexBuffer_;
		VkDeviceMemory vkVertexBufferMemory_;
		VkBuffer vkIndexBuffer_;
		VkDeviceMemory vkIndexBufferMemory_;
		VkBuffer vkUniformBuffer_;
		VkDeviceMemory vkUniformBufferMemory_;
		uint32_t mipLevels_;
		VkImage vkTextureImage_;
		VkDeviceMemory vkTextureImageMemory_;

		VkImageView vkTextureImageView_;
		VkSampler vkTextureSampler_;

		VkImage vkDepthImage_;
		VkDeviceMemory vkDepthImageMemory_;
		VkImageView vkDepthImageView_;
	};


}

#endif