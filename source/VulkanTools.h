#ifndef VULKAN_TOOLS
#define VULKAN_TOOLS

#include <assert.h>
#include <stdlib.h>

#ifdef NDEBUG
    #define MY_ASSERT(x) if (!x) exit(0);
#else
    #define MY_ASSERT(x) assert(x)
#endif

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

struct swapChain {
    VkImage *images;
    uint32_t imagesCount;
    VkFormat imageFormat;
    VkExtent2D extent;
    VkSwapchainKHR this;
};

struct VulkanTools {
    GLFWwindow *window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice;
    VkQueue presentQueue;
    VkQueue graphicsQueue;
    VkDevice device;

    struct swapChain swapChain;
    VkImageView *swapChainImageViews;

    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkFramebuffer *swapChainFramebuffers;

    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;
};

struct VulkanTools setup(void);
void cleanup(struct VulkanTools vulkan);
void drawFrame(struct VulkanTools *vulkan);

#endif // !VULKAN_TOOLS
