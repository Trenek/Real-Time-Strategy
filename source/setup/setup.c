#include "setup.h"

struct VulkanTools setup(void) {
    struct VulkanTools vulkan = { 0 };

    vulkan.window = createWindow();
    vulkan.instance = createInstance(&vulkan.debugMessenger);
    vulkan.surface = createSurface(vulkan.window, vulkan.instance);
    vulkan.physicalDevice = pickPhysicalDevice(vulkan.instance, vulkan.surface);
    vulkan.device = createLogicalDevice(vulkan.surface, vulkan.physicalDevice, &vulkan.graphicsQueue, &vulkan.presentQueue);
    vulkan.swapChain = createSwapChain(vulkan.window, vulkan.surface, vulkan.physicalDevice, vulkan.device);
    vulkan.swapChainImageViews = createImageViews(vulkan.device, vulkan.swapChain);

    vulkan.renderPass = createRenderPass(vulkan.device, vulkan.swapChain.imageFormat);
    vulkan.pipelineLayout = createPipelineLayout(vulkan.device);
    vulkan.graphicsPipeline = createGraphicsPipeline(vulkan.device, vulkan.swapChain.extent, vulkan.renderPass, vulkan.pipelineLayout);

    vulkan.swapChainFramebuffers = createFramebuffers(vulkan.device, vulkan.swapChainImageViews, vulkan.swapChain.imagesCount, vulkan.swapChain.extent, vulkan.renderPass);

    vulkan.commandPool = createCommandPool(vulkan.device, vulkan.physicalDevice, vulkan.surface);
    vulkan.commandBuffer = createCommandBuffer(vulkan.device, vulkan.commandPool);

    vulkan.imageAvailableSemaphore = createSemaphore(vulkan.device);
    vulkan.renderFinishedSemaphore = createSemaphore(vulkan.device);
    vulkan.inFlightFence = createFence(vulkan.device);

    return vulkan;
}

void cleanup(struct VulkanTools vulkan) {
    vkDeviceWaitIdle(vulkan.device);

    vkDestroySemaphore(vulkan.device, vulkan.imageAvailableSemaphore, NULL);
    vkDestroySemaphore(vulkan.device, vulkan.renderFinishedSemaphore, NULL);
    vkDestroyFence(vulkan.device, vulkan.inFlightFence, NULL);

    vkDestroyCommandPool(vulkan.device, vulkan.commandPool, NULL);

    destroyFramebuffers(vulkan.device, vulkan.swapChainFramebuffers, vulkan.swapChain.imagesCount);

    vkDestroyPipeline(vulkan.device, vulkan.graphicsPipeline, NULL);
    vkDestroyPipelineLayout(vulkan.device, vulkan.pipelineLayout, NULL);
    vkDestroyRenderPass(vulkan.device, vulkan.renderPass, NULL);

    destroyImageViews(vulkan.swapChainImageViews, vulkan.swapChain.imagesCount, vulkan.device);
    vkDestroySwapchainKHR(vulkan.device, vulkan.swapChain.this, NULL);
    vkDestroyDevice(vulkan.device, NULL);
    DestroyDebugUtilsMessengerEXT(vulkan.instance, vulkan.debugMessenger, NULL);
    vkDestroySurfaceKHR(vulkan.instance, vulkan.surface, NULL);
    vkDestroyInstance(vulkan.instance, NULL);
    destroyWindow(vulkan.window);
}
