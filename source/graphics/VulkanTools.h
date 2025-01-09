#ifndef VULKAN_TOOLS
#define VULKAN_TOOLS

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "model.h"
#include "definitions.h"

#include "windowControl.h"

struct swapChain {
    VkImage *images;
    uint32_t imagesCount;
    VkFormat imageFormat;
    VkExtent2D extent;
    VkSwapchainKHR this;
};

struct VulkanTools {
    GLFWwindow *window;
    struct windowControl *windowControl;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice;
    VkQueue presentQueue;
    VkQueue graphicsQueue;
    VkQueue transferQueue;
    VkDevice device;

    struct swapChain swapChain;
    VkImageView *swapChainImageViews;

    VkRenderPass renderPass;

    VkFramebuffer *swapChainFramebuffers;

    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer[MAX_FRAMES_IN_FLIGHT];
    VkCommandPool transferCommandPool;

    VkBuffer uniformBuffers[MAX_FRAMES_IN_FLIGHT];
    VkDeviceMemory uniformBuffersMemory[MAX_FRAMES_IN_FLIGHT];
    void *uniformBuffersMapped[MAX_FRAMES_IN_FLIGHT];

    VkSemaphore imageAvailableSemaphore[MAX_FRAMES_IN_FLIGHT];
    VkSemaphore renderFinishedSemaphore[MAX_FRAMES_IN_FLIGHT];
    VkFence inFlightFence[MAX_FRAMES_IN_FLIGHT];

  // uint32_t modelQuantity;
  //  struct Model *model;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    bool *framebufferResized;

    VkSampleCountFlagBits msaaSamples;

    VkImage colorImage;
    VkDeviceMemory colorImageMemory;
    VkImageView colorImageView;

    vec3 cameraPos;
    vec3 center;
};

struct VulkanTools setup();
void recreateSwapChain(struct VulkanTools *vulkan);
void cleanup(struct VulkanTools vulkan);

void drawFrame(struct VulkanTools *vulkan, uint16_t modelQuantity, struct Model model[modelQuantity]);

#endif // !VULKAN_TOOLS
