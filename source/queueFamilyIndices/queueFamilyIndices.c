#include "queueFamilyIndices.h"

bool familyEqual(struct value family1, struct value family2) {
    return family1.exists == family2.exists && family1.value == family2.value;
}

struct QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    struct QueueFamilyIndices indices = { 0 };

    uint32_t queueFamilyCount = 0;
    VkQueueFamilyProperties *queueFamilies = NULL;

    VkBool32 presentSupport = false;
    bool found = false;

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);
    queueFamilies = malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    uint32_t i = 0;
    while (i < queueFamilyCount && found == false) {
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily.value = i;
            indices.graphicsFamily.exists = true;
        }

        if (presentSupport) {
            indices.presentFamily.value = i;
            indices.presentFamily.exists = true;
        }

        if (indices.graphicsFamily.exists && indices.presentFamily.exists) {
            found = true;
        }

        i += 1;
    }

    free(queueFamilies);

    return indices;
}
