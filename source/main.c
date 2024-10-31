#include "VulkanTools.h"

const char *const deviceExtensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
const uint32_t deviceExtensionsCount = sizeof(deviceExtensions) / sizeof(const char *const);

int main(void) {
    struct VulkanTools vulkan = setup();

    while (!glfwWindowShouldClose(vulkan.window)) {
        glfwPollEvents();

        drawFrame(&vulkan);
    }

    cleanup(vulkan);

    return 0;
}
