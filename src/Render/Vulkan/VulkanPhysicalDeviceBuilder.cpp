#include "VulkanPhysicalDeviceBuilder.h"
#include "nvvk/error_vk.hpp"
#include <stdexcept>
#include <cassert>

namespace Sun {

	VulkanPhysicalDeviceBuilder::VulkanPhysicalDeviceBuilder(VkInstance instance)
	:vkInstance_(instance){
	}

	VkPhysicalDevice VulkanPhysicalDeviceBuilder::buildDevice() {
		std::vector <VkPhysicalDevice> devices = enumerateCompatibleDevices(deviceExtensions_);
		if (devices.empty()) {
			assert(false);
			return VK_NULL_HANDLE;
		}
		return devices[0];
	}

	VulkanPhysicalDeviceBuilder& VulkanPhysicalDeviceBuilder::setDeviceExtensions(const std::vector<std::string>& extensions) {
		deviceExtensions_ = extensions;
		return *this;
	}

	std::vector<VkPhysicalDevice> VulkanPhysicalDeviceBuilder::enumeratePhysicalDevices() {
		return enumeratePhysicalDevices(vkInstance_);
	}

	std::vector<VkPhysicalDevice> VulkanPhysicalDeviceBuilder::enumeratePhysicalDevices(VkInstance instance) {
		assert(instance != VK_NULL_HANDLE);
		uint32_t deviceCount = 0;
		NVVK_CHECK(vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr));
		std::vector<VkPhysicalDevice> devices(deviceCount);
		NVVK_CHECK(vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data()));
		return devices;
	}

	std::vector<VkPhysicalDeviceGroupProperties> VulkanPhysicalDeviceBuilder::enumeratePhysicalDeviceGroups(){
		return enumeratePhysicalDeviceGroups(vkInstance_);
	}

	std::vector<VkPhysicalDeviceGroupProperties> VulkanPhysicalDeviceBuilder::enumeratePhysicalDeviceGroups(VkInstance instance) {
		assert(instance != VK_NULL_HANDLE);
		uint32_t  deviceGroupCount = 0;
		std::vector<VkPhysicalDeviceGroupProperties> groups;
		VkResult res = vkEnumeratePhysicalDeviceGroups(instance, &deviceGroupCount, nullptr);
		if (res != VK_SUCCESS) {
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
		}
		groups.resize(deviceGroupCount);
		res = vkEnumeratePhysicalDeviceGroups(instance, &deviceGroupCount, groups.data());
		return groups;
	}

	std::vector<VkExtensionProperties> VulkanPhysicalDeviceBuilder::enumerateDeviceExtensionProperties(VkPhysicalDevice vk) {
		std::vector<VkExtensionProperties> extensionProperties;

		uint32_t count = 0;
		VkResult res;
		res = vkEnumerateDeviceExtensionProperties(vk, nullptr, &count, nullptr);
		extensionProperties.resize(count);
		res = vkEnumerateDeviceExtensionProperties(vk, nullptr, &count, extensionProperties.data());
		extensionProperties.resize(std::min(extensionProperties.size(), size_t(count)));

		return extensionProperties;
	}

	std::vector<VkQueueFamilyProperties> VulkanPhysicalDeviceBuilder::enumeratePhysicalDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice) {
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
		return queueFamilies;
	}

	PhysicalDeviceInfo VulkanPhysicalDeviceBuilder::getPhysicalDeviceInfo(VkPhysicalDevice physicalDevice) {
		VkResult res;
		PhysicalDeviceInfo info;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &info.memoryProperties);

		uint32_t count;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, nullptr);
		info.queueProperties.resize(count);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, info.queueProperties.data());

		// for queries and device creation
		VkPhysicalDeviceFeatures2   features2{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2 };
		VkPhysicalDeviceProperties2 properties2{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2 };
		
#if 0
        Properties11Old             properties11old;
		Features11Old               features11old;

        //FIXME
		if (versionMajor == 1 && versionMinor == 1)
		{
			features2.pNext = &features11old.multiview;
			properties2.pNext = &properties11old.maintenance3;
		}
		else if (versionMajor == 1 && versionMinor >= 2)
		{
			features2.pNext = &info.features11;
			info.features11.pNext = &info.features12;
			info.features12.pNext = nullptr;

			info.properties12.driverID = VK_DRIVER_ID_NVIDIA_PROPRIETARY;
			info.properties12.supportedDepthResolveModes = VK_RESOLVE_MODE_MAX_BIT;
			info.properties12.supportedStencilResolveModes = VK_RESOLVE_MODE_MAX_BIT;

			properties2.pNext = &info.properties11;
			info.properties11.pNext = &info.properties12;
			info.properties12.pNext = nullptr;
		}

		vkGetPhysicalDeviceFeatures2(physicalDevice, &features2);
		vkGetPhysicalDeviceProperties2(physicalDevice, &properties2);

		info.properties10 = properties2.properties;
		info.features10 = features2.features;

		if (versionMajor == 1 && versionMinor == 1)
		{
			properties11old.write(info.properties11);
			features11old.write(info.features11);
		}
#endif
        return info;
	}

	std::vector<VkPhysicalDevice> VulkanPhysicalDeviceBuilder::enumerateCompatibleDevices(const std::vector<std::string>& extensions) {
		assert(vkInstance_ != VK_NULL_HANDLE);
		std::vector<VkPhysicalDevice>                compatibleDevices;
		std::vector<VkPhysicalDevice>                physicalDevices;

		uint32_t nbElems = 0;
		physicalDevices = enumeratePhysicalDevices();
		nbElems = static_cast<uint32_t>(physicalDevices.size());

		uint32_t compatible = 0;
		for (uint32_t elemId = 0; elemId < nbElems; ++elemId)
		{
			VkPhysicalDevice physicalDevice = physicalDevices[elemId];

			std::vector<VkExtensionProperties> vec = enumerateDeviceExtensionProperties(physicalDevice);
			
			if(checkContain(vec, extensions))
			{
				compatibleDevices.push_back(physicalDevice);
			}
		}

		return compatibleDevices;
	}

	bool VulkanPhysicalDeviceBuilder::checkContain(const std::vector<VkExtensionProperties>& properties, const std::vector<std::string>& names) {
		for (const auto& name : names)
		{
			bool found = false;
			for (const auto& property : properties)
			{
				if (strcmp(name.c_str(), property.extensionName) == 0)
				{
					found = true;
					break;
				}
			}
			if (!found) {
				return false;
			}
		}
		return true;
	}
}