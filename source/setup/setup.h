#include "VulkanTools.h"

GLFWwindow *createWindow(void);
VkInstance createInstance(VkDebugUtilsMessengerEXT *debugMessenger);
VkSurfaceKHR createSurface(GLFWwindow *window, VkInstance instance);
VkPhysicalDevice pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
VkDevice createLogicalDevice(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice, VkQueue *graphicsQueue, VkQueue *presentQueue);
struct swapChain createSwapChain(GLFWwindow *window, VkSurfaceKHR surface, VkPhysicalDevice physicalDevice, VkDevice device);
VkImageView *createImageViews(VkDevice device, struct swapChain swapChain);
VkRenderPass createRenderPass(VkDevice device, VkFormat swapChainImageFormat);
VkPipelineLayout createPipelineLayout(VkDevice device);
VkPipeline createGraphicsPipeline(VkDevice device, VkExtent2D swapChainExtent, VkRenderPass renderPass, VkPipelineLayout pipelineLayout);
VkFramebuffer *createFramebuffers(VkDevice device, VkImageView *swapChainImageViews, uint32_t swapChainImagesCount, VkExtent2D swapChainExtent, VkRenderPass renderPass);
VkCommandPool createCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
VkCommandBuffer createCommandBuffer(VkDevice device, VkCommandPool commandPool);
VkSemaphore createSemaphore(VkDevice device);
VkFence createFence(VkDevice device);

void destroyWindow(GLFWwindow *window);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);
void destroyImageViews(VkImageView *swapChainImageViews, uint32_t swapChainImagesCount, VkDevice device);
void destroyFramebuffers(VkDevice device, VkFramebuffer *swapChainFramebuffers, uint32_t swapChainImagesCount);
