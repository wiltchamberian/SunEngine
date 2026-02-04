#ifndef __SUN_VULKAN_DEVICE_BUILDER_H
#define __SUN_VULKAN_DEVICE_BUILDER_H

#include "vulkan/vulkan_core.h"
#include <vector>
#include <string>

namespace Sun {

	//该结构体持有设备的所有核心功能信息
	struct PhysicalDeviceInfo {
		VkPhysicalDeviceMemoryProperties     memoryProperties{};
		std::vector<VkQueueFamilyProperties> queueProperties;

		VkPhysicalDeviceFeatures         features10{};
		VkPhysicalDeviceVulkan11Features features11{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES };
		VkPhysicalDeviceVulkan12Features features12{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };

		VkPhysicalDeviceProperties         properties10{};
		VkPhysicalDeviceVulkan11Properties properties11{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES };
		VkPhysicalDeviceVulkan12Properties properties12{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES };
	};

	class VulkanPhysicalDeviceBuilder {
	public:
		VulkanPhysicalDeviceBuilder(VkInstance instance);
		VkPhysicalDevice buildDevice();
		VulkanPhysicalDeviceBuilder& setDeviceExtensions(const std::vector<std::string>& extensions);
	public:
		std::vector<VkPhysicalDevice> enumeratePhysicalDevices();
		static std::vector<VkPhysicalDevice> enumeratePhysicalDevices(VkInstance instance);
		std::vector<VkPhysicalDeviceGroupProperties> enumeratePhysicalDeviceGroups();
		static std::vector<VkPhysicalDeviceGroupProperties> enumeratePhysicalDeviceGroups(VkInstance instance);
		//获取设备扩展属性
		static std::vector<VkExtensionProperties> enumerateDeviceExtensionProperties(VkPhysicalDevice vk);
		//获取设备队列簇信息
		static std::vector<VkQueueFamilyProperties> enumeratePhysicalDeviceQueueFamilyProperties(VkPhysicalDevice vk);
		//获取设备信息
		PhysicalDeviceInfo getPhysicalDeviceInfo(VkPhysicalDevice vkHandle);
		//枚举和扩展兼容的物理设备
		std::vector<VkPhysicalDevice> enumerateCompatibleDevices(const std::vector<std::string>& extensions);
	private:
		bool checkContain(const std::vector<VkExtensionProperties>& vec,const std::vector<std::string>& names);
		VkInstance vkInstance_;
		std::vector<std::string> deviceExtensions_;
	};

}

#endif