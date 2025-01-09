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
    struct instanceBuffer *instanceBuffer;

    uint32_t meshQuantity;
    struct Mesh *mesh;

    uint32_t texturesQuantity;
    struct Textures *texture;

    VkBuffer localMeshBuffers[MAX_FRAMES_IN_FLIGHT];
    VkDeviceMemory localMeshBuffersMemory[MAX_FRAMES_IN_FLIGHT];
    void *localMeshBuffersMapped[MAX_FRAMES_IN_FLIGHT];

    VkBuffer uniformModelBuffers[MAX_FRAMES_IN_FLIGHT];
    VkDeviceMemory uniformModelBuffersMemory[MAX_FRAMES_IN_FLIGHT];
    void *uniformModelBuffersMapped[MAX_FRAMES_IN_FLIGHT];

    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSets[MAX_FRAMES_IN_FLIGHT];
};

struct VulkanTools;
struct Model createModels(struct ModelBuilder modelBuilder, struct VulkanTools *vulkan);
void destroyModels(VkDevice device, struct Model model);
