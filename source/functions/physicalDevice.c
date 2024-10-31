#include <malloc.h>
#include <string.h>

#include "VulkanTools.h"
#include "swapChainSupportDetails.h"
#include "queueFamilyIndices.h"

extern const char *const deviceExtensions[];
extern const uint32_t deviceExtensionsCount;

static bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount = 0;
    VkExtensionProperties *avaibleExtensions = NULL;

    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);
    avaibleExtensions = malloc(sizeof(VkExtensionProperties) * extensionCount);
    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, avaibleExtensions);

    uint32_t i = 0;
    uint32_t j = 0;
    bool isFound = true;
    while (i < deviceExtensionsCount && isFound == true) {
        isFound = false;
        j = 0;

        while (isFound == false && j < extensionCount) {
            if (strcmp(deviceExtensions[i], avaibleExtensions[j].extensionName) == 0) {
                isFound = true;
            }

            j += 1;
        }

        i += 1;
    }

    free(avaibleExtensions);

    return isFound;
}

static bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    struct QueueFamilyIndices queueFamilies = findQueueFamilies(device, surface);
    struct SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, surface);

    bool swapChainSupportVariable = swapChainSupport.formatCount != 0 && swapChainSupport.presentModeCount != 0;

    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    free(swapChainSupport.formats);
    free(swapChainSupport.presentModes);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
        deviceFeatures.geometryShader &&
        checkDeviceExtensionSupport(device) &&
        swapChainSupportVariable &&
        queueFamilies.graphicsFamily.exists &&
        queueFamilies.presentFamily.exists;
}

VkPhysicalDevice pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    uint32_t deviceCount = 0;
    VkPhysicalDevice *devices;
    uint32_t i = 0;

    vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);
    assert(deviceCount != 0);

    devices = malloc(sizeof(VkPhysicalDevice) * deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

    while (i < deviceCount && physicalDevice == VK_NULL_HANDLE) {
        if (isDeviceSuitable(devices[i], surface)) {
            physicalDevice = devices[i];
        }
        i += 1;
    }

    assert(physicalDevice != NULL);
    free(devices);

    return physicalDevice;
}
