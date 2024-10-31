#include <malloc.h>
#include <string.h>
#include <stdio.h>

#include "VulkanTools.h"

#ifdef NDEBUG
static const bool enableValidationLayers = false;
#else
static const bool enableValidationLayers = true;
#endif

static const char *const validationLayers[] = {
    "VK_LAYER_KHRONOS_validation"
};
static const uint32_t validationLayersCount = sizeof(validationLayers) / sizeof(const char *);

static bool checkValidationLayerSupport(void) {
    uint32_t layerCount = 0;
    VkLayerProperties *avaibleLayers = NULL;
    uint32_t i = 0;
    uint32_t j = 0;
    bool layerFound = false;
    bool result = true;

    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    avaibleLayers = malloc(sizeof(VkLayerProperties) * layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, avaibleLayers);

    while (i < validationLayersCount && result == true) {
        layerFound = false;

        j = 0;
        while (j < layerCount && layerFound == false) {
            if (strcmp(validationLayers[i], avaibleLayers[j].layerName) == 0) {
                layerFound = true;
            }

            j += 1;
        }

        if (layerFound == false) {
            result = false;
        }

        i += 1;
    }

    free(avaibleLayers);

    return result;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, [[maybe_unused]] void *pUserData) {
    fprintf(stderr, "VL ");

    switch ((int)messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            fprintf(stderr, "VERBOSE: ");
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            fprintf(stderr, "INFO: ");
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            fprintf(stderr, "WARNING: ");
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            fprintf(stderr, "ERROR: ");
            break;
    }

    fprintf(stderr, "%s\n", pCallbackData->pMessage);

    return VK_FALSE;
}

static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger) {
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != NULL) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator) {
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func != NULL && enableValidationLayers) {
        func(instance, debugMessenger, pAllocator);
    }
}

static const char **getRequiredExtensions(uint32_t *extensionCount) {
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(extensionCount);
    const char **extensions = glfwExtensions;

    if (enableValidationLayers) {
        uint32_t new = *extensionCount + 1;
        extensions = malloc(sizeof(const char *) * new);

        memcpy(extensions, glfwExtensions, sizeof(const char *) * *extensionCount);
        extensions[*extensionCount] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME; // not sure how it works
        *extensionCount += 1;
    }

    return extensions;
}

VkInstance createInstance(VkDebugUtilsMessengerEXT *debugMessenger) {
    assert(true == checkValidationLayerSupport());

    VkInstance instance = NULL;

    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,
        .pApplicationName = "Hello Triangle",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0
    };

    uint32_t extensionCount = 0;
    const char **extensions = getRequiredExtensions(&extensionCount);

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity =  VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
//                          VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType =      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = debugCallback,
        .flags = 0,
        .pNext = NULL,
        .pUserData = NULL
    };

    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = enableValidationLayers ? &debugCreateInfo : NULL,
        .flags = 0,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = enableValidationLayers ? validationLayersCount : 0,
        .ppEnabledLayerNames = enableValidationLayers ? validationLayers : NULL,
        .enabledExtensionCount = extensionCount,
        .ppEnabledExtensionNames = extensions
    };

    MY_ASSERT(VK_SUCCESS == vkCreateInstance(&createInfo, NULL, &instance));
    MY_ASSERT(VK_SUCCESS == CreateDebugUtilsMessengerEXT(instance, &debugCreateInfo, NULL, debugMessenger));

    free(extensions);

    return instance;
}
