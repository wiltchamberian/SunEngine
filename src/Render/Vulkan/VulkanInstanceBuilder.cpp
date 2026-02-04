#include "VulkanInstanceBuilder.h"
#include <stdexcept>
#include <cassert>

namespace Sun {

    namespace rhi {

        HContext VulkanInstanceBuilder::build() {
            VkInstance instance = VK_NULL_HANDLE;

            VkInstanceCreateInfo vkCi;
            vkCi.pNext = nullptr;
            vkCi.flags = 0;
            vkCi.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

            vkCi.enabledExtensionCount = extensions_.size();
            std::vector<const char*> vec;
            for (int i = 0; i < extensions_.size(); ++i) {
                vec.push_back(extensions_[i].data());
            }
            vkCi.ppEnabledExtensionNames = vec.data();

            vkCi.enabledLayerCount = layers_.size();
            std::vector<const char*> vec2;
            for (int i = 0; i < layers_.size(); ++i) {
                vec2.push_back(layers_[i].c_str());
            }
            vkCi.ppEnabledLayerNames = vec2.data();

            if (vkCreateInstance(&vkCi, nullptr, &instance) != VK_SUCCESS) {
                assert(false);
                throw std::runtime_error("failed to create instance!");
            }

            HContext context;
            context.backend = instance;
            return context;
        }

        void VulkanInstanceBuilder::destroy(HContext context) {
            vkDestroyInstance((VkInstance)context.backend, nullptr);
        }

        VulkanInstanceBuilder::VulkanInstanceBuilder()
        {
            appInfo_.applicationName = "App";
            appInfo_.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo_.engineName = "Engine";
            appInfo_.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo_.apiVersion = VK_API_VERSION_1_2;
        }

    }
}