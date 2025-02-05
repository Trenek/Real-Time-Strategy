#ifndef MODEL_BUILDER
#define MODEL_BUILDER
#include <vulkan/vulkan.h>

struct Model;

struct ModelBuilder {
    const char *modelPath;

    uint32_t texturesQuantity;
    const char **texturesPath;

    const char *vertexShader;
    const char *fragmentShader;

    uint32_t instanceCount;

    float minDepth;
    float maxDepth;

    void (*loadModel)(const char *objectPath, struct Model *model, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

    VkDescriptorSetLayout (*createDescriptorSetLayout)(VkDevice device);
    VkDescriptorPool (*createDescriptorPool)(VkDevice device);
    void (*bindBuffersToDescriptorSets)(VkDescriptorSet descriptorSets[], VkDevice device, VkBuffer uniformBuffers[], struct Model model);
};


#include "modelLoader.h"

VkDescriptorPool createTextureDescriptorPool(VkDevice device, uint32_t texturesCount);
VkDescriptorSetLayout createTextureDescriptorSetLayout(VkDevice device, uint32_t textureQuantity);
void bindTextureBuffersToDescriptorSets(VkDescriptorSet descriptorSets[], VkDevice device, uint32_t texturesQuantity, struct Textures *texture);

VkDescriptorPool createObjectDescriptorPool(VkDevice device);
VkDescriptorSetLayout createObjectDescriptorSetLayout(VkDevice device);
void bindObjectBuffersToDescriptorSets(VkDescriptorSet descriptorSets[], VkDevice device, struct Model model);

VkDescriptorPool createCameraDescriptorPool(VkDevice device);
VkDescriptorSetLayout createCameraDescriptorSetLayout(VkDevice device);
void bindCameraBuffersToDescriptorSets(VkDescriptorSet descriptorSets[], VkDevice device, VkBuffer uniformBuffers[]);

#endif
