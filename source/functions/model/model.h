#include <vulkan/vulkan.h>
#include <cglm.h>

#include "definitions.h"
#include "modelLoader.h"

struct Textures {
    const char *path;

    VkImage image;
    VkDeviceMemory imageMemory;
    uint32_t mipLevels;
    VkImageView imageView;
    VkSampler sampler;
};

struct Mesh {
    struct Vertex *vertices;
    uint16_t *indices;
    size_t verticesQuantity;
    size_t indicesQuantity;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
};

struct Model {
    uint32_t instanceCount;
    struct instance *instance;

    uint32_t meshQuantity;
    struct Mesh *mesh;

    uint32_t texturesQuantity;
    struct Textures *texture;

    VkBuffer uniformModelBuffers[MAX_FRAMES_IN_FLIGHT];
    VkDeviceMemory uniformModelBuffersMemory[MAX_FRAMES_IN_FLIGHT];
    void *uniformModelBuffersMapped[MAX_FRAMES_IN_FLIGHT];
    struct instanceInfo *localUniformModelBuffersMapped;

    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSets[MAX_FRAMES_IN_FLIGHT];

};

struct Model loadModels(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkCommandPool commandPool, VkQueue queue);
void unloadModels(VkDevice device, struct Model model);
