#include "VulkanTools.h"

VkSurfaceKHR createSurface(GLFWwindow *window, VkInstance instance) {
    VkSurfaceKHR surface = NULL;

    MY_ASSERT(VK_SUCCESS == glfwCreateWindowSurface(instance, window, NULL, &surface));

    return surface;
}
