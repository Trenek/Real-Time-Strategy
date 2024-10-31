#include "VulkanTools.h"

#include "queueFamilyIndices.h"

extern const char *const deviceExtensions[];
extern const uint32_t deviceExtensionsCount;

VkDevice createLogicalDevice(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice, VkQueue *graphicsQueue, VkQueue *presentQueue) {
    VkDevice device = NULL;

    struct QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);

    int32_t queueNumber = 1;
    VkDeviceQueueCreateInfo queueCreateInfo[2] = {
        {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = indices.graphicsFamily.value,
            .queueCount = 1,
            .pQueuePriorities = (float[]) { 1.0f },
            .flags = 0,
            .pNext = NULL
        }
    };

    if (indices.graphicsFamily.value != indices.presentFamily.value) {
        queueNumber += 1;
        queueCreateInfo[1] = (VkDeviceQueueCreateInfo){
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = indices.presentFamily.value,
            .queueCount = 1,
            .pQueuePriorities = (float[]) { 1.0f },
            .flags = 0,
            .pNext = NULL
        };
    }

    VkPhysicalDeviceFeatures deviceFeatures = { 0 };

    VkDeviceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pQueueCreateInfos = queueCreateInfo,
        .queueCreateInfoCount = queueNumber,
        .pEnabledFeatures = &deviceFeatures,
        .enabledExtensionCount = deviceExtensionsCount,
        .ppEnabledExtensionNames = deviceExtensions,
        .pNext = NULL,
        .flags = 0
    };

    MY_ASSERT(VK_SUCCESS == vkCreateDevice(physicalDevice, &createInfo, NULL, &device));
    vkGetDeviceQueue(device, indices.graphicsFamily.value, 0, graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value, 0, presentQueue);

    return device;
}
