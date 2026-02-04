#ifndef __VULKAN_DEVICE_BUILDER_H
#define __VULKAN_DEVICE_BUILDER_H

#include "vulkan/vulkan_core.h"
#include "VulkanPhysicalDeviceBuilder.h"
#include <vector>
#include <map>

namespace Sun {

	//创建设备需要的信息
	struct DeviceQueueInfo {
		//队列簇能力
		VkQueueFlags flags_;
		//是否支持显示
		VkBool32 presentSupport;
	};

	struct Queue
	{
		VkQueue  queue = VK_NULL_HANDLE;
		uint32_t familyIndex = ~0;
		uint32_t queueIndex = ~0;

		operator VkQueue() const { return queue; }
		operator uint32_t() const { return familyIndex; }
	};

	using QueueMap = std::map<VkQueueFlags, Queue>;

	class VulkanDeviceBuilder {
	public:
		VulkanDeviceBuilder();
		VkDevice buildDevice(uint32_t major,uint32_t minor);
		void setDeviceQueueCreateInfo(const DeviceQueueInfo& queInfo);

		void setQueueFlagBits(int slot, VkQueueFlagBits flags);
		void setDeviceQueueCreateFlags(int slot, VkDeviceQueueCreateFlags flags);
		//创建逻辑设备需要VkDeviceQueueCreateInfo
		void addQueueCreateInfo(const VkDeviceQueueCreateInfo& queCreateInfo);

		//从设备中搜索支持needFlags所有bit的设备，并返回其中之一
		Queue findQueueFromDevice(VkQueueFlags needFlags, VkDevice device);
		//从设备中搜索不相同的queue
		std::vector<Queue> findDistinctQueuesFromDevice(const std::vector<VkQueueFlags>& flags, VkDevice device);
	private:
		void resizeQueCreateInfos(int newSiz);
		VkInstance vkInstance_;
		VkPhysicalDevice vkPhysicalDevice_;
		DeviceQueueInfo deviceQueueInfo_;
		std::vector<VkDeviceQueueCreateInfo> queCreateInfos_;
		VkDeviceCreateInfo deviceCreateInfo_;
	};

}

#endif