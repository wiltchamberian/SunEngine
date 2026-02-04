#include "VulkanDeviceBuilder.h"
#include <cassert>
#include <algorithm>
#include "nvvk/nvprint.hpp"

namespace Sun {

	VulkanDeviceBuilder::VulkanDeviceBuilder()
        :deviceCreateInfo_{}
    {
        deviceCreateInfo_.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	}

    void VulkanDeviceBuilder::addQueueCreateInfo(const VkDeviceQueueCreateInfo& queCreateInfo) {
        queCreateInfos_ .emplace_back(queCreateInfo);
    }

    void VulkanDeviceBuilder::setQueueFlagBits(int slot, VkQueueFlagBits flags) {

    }

	VkDevice VulkanDeviceBuilder::buildDevice(uint32_t major, uint32_t minor) {
        VkDevice vkDevice = VK_NULL_HANDLE;

        std::vector<VkQueueFamilyProperties> queFamilyProperties 
            = VulkanPhysicalDeviceBuilder::enumeratePhysicalDeviceQueueFamilyProperties(vkPhysicalDevice_);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::vector<float>                   priorities;
        std::vector<void*>                   featureStructs;
        bool queueFamilyGeneralPurpose = false;
        {
            for (auto& it : queFamilyProperties)
            {
                if ((it.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT))
                    == (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT))
                {
                    queueFamilyGeneralPurpose = true;
                }
                if (it.queueCount > priorities.size())
                {
                    // set all priorities equal
                    priorities.resize(it.queueCount, 1.0f);
                }
            }
            for (uint32_t i = 0; i < queFamilyProperties.size(); ++i)
            {
                VkDeviceQueueCreateInfo queueInfo{ VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
                queueInfo.queueFamilyIndex = i;
                queueInfo.queueCount = queFamilyProperties[i].queueCount;
                queueInfo.pQueuePriorities = priorities.data();

                queueCreateInfos.push_back(queueInfo);
            }
        }
        if (!queueFamilyGeneralPurpose)
        {
            LOGW("could not find queue that supports graphics, compute and transfer");
        }

        // allow all queues
        VkDeviceCreateInfo deviceCreateInfo{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
        deviceCreateInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

        VkResult result = vkCreateDevice(vkPhysicalDevice_, &deviceCreateInfo, nullptr, &vkDevice);
        if (VK_SUCCESS != result) {
            assert(false);
        }
        return vkDevice;
	}

    //从设备中搜索支持needFlags所有bit的设备，并返回其中之一
    Queue VulkanDeviceBuilder::findQueueFromDevice(VkQueueFlags needFlags, VkDevice device) {
        return Queue();
    }

    //从设备中搜索不相同的queue
    std::vector<Queue> VulkanDeviceBuilder::findDistinctQueuesFromDevice(const std::vector<VkQueueFlags>& flags, VkDevice device) {
        struct QueueScore
        {
            uint32_t score;  // the lower the score, the more 'specialized' it is
            uint32_t familyIndex;
            uint32_t queueIndex;
        };

        std::vector<VkQueueFamilyProperties> queFamilyProperties
            = VulkanPhysicalDeviceBuilder::enumeratePhysicalDeviceQueueFamilyProperties(vkPhysicalDevice_);

        std::vector<QueueScore> queueScores;
        for (uint32_t qF = 0; qF < queFamilyProperties.size(); ++qF)
        {
            const auto& queueFamily = queFamilyProperties[qF];

            QueueScore score{ 0, qF, 0 };
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                score.score++;
            }
            if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
            {
                score.score++;
            }
            if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
            {
                score.score++;
            }
            for (uint32_t qI = 0; qI < queueFamily.queueCount; ++qI)
            {
                score.queueIndex = qI;
                queueScores.emplace_back(score);
            }
        }

        // Sort the queues for specialization, highest specialization has lowest score
        std::sort(queueScores.begin(), queueScores.end(), [](const QueueScore& lhs, const QueueScore& rhs) {
            if (lhs.score < rhs.score)
                return true;
            if (lhs.score > rhs.score)
                return false;
            return lhs.queueIndex < rhs.queueIndex;
            });

        auto findQueue = [this, &queueScores, queFamilyProperties,device](VkQueueFlags needFlags) -> Queue {
            for (uint32_t q = 0; q < queueScores.size(); ++q)
            {
                const QueueScore& score = queueScores[q];
                auto& family = queFamilyProperties[score.familyIndex];
                if ((family.queueFlags & needFlags) == needFlags)
                {
                    Queue queue;
                    vkGetDeviceQueue(device, score.familyIndex, score.queueIndex, &queue.queue);
                    queue.familyIndex = score.familyIndex;
                    queue.queueIndex = score.queueIndex;
                    // we used this queue
                    queueScores.erase(queueScores.begin() + q);
                    return queue;
                }
            }
            return Queue();
        };

        std::vector<Queue> res;
        for (int i = 0; i < flags.size(); ++i) {
            Queue que = findQueue(flags[i]);
            res.emplace_back(que);
        }
        
        return res;
    }

    void VulkanDeviceBuilder::setDeviceQueueCreateInfo(const DeviceQueueInfo& queInfo) {
        deviceQueueInfo_ = queInfo;
    }

    void VulkanDeviceBuilder::setDeviceQueueCreateFlags(int index, VkDeviceQueueCreateFlags flags) {
        if (index + 1 > queCreateInfos_.size()) {
            resizeQueCreateInfos(index + 1);
        }
        queCreateInfos_[index].flags = flags;
    }

    void VulkanDeviceBuilder::resizeQueCreateInfos(int newSiz) {
        assert(newSiz >= 0);
        if (newSiz < 0) return;
        if (newSiz > queCreateInfos_.size()) {
            int d = newSiz - queCreateInfos_.size();
            int oldSiz = queCreateInfos_.size();
            queCreateInfos_.resize(newSiz);          
            for (int i = 0; i < d; ++i) {
                queCreateInfos_[oldSiz + i ] = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
            }
        }
        else {
            queCreateInfos_.resize(newSiz);
        }
    }
}