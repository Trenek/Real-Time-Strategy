#include "VulkanTools.h"

#include "model.h"
#include "modelBuilder.h"
#include "bufferOperations.h"

#include "modelFunctions.h"
#include "instanceBuffer.h"

struct Textures loadTexture(const char *texturePath, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkCommandPool commandPool, VkQueue queue) {
    struct Textures result = { 0 };

    result.image = createTextureBuffer(&result.imageMemory, &result.mipLevels, texturePath, device, physicalDevice, surface, commandPool, queue);
    result.imageView = createTextureImageView(device, result.image, result.mipLevels);
    result.sampler = createTextureSampler(device, physicalDevice, result.mipLevels);

    return result;
}

struct Model createModels(struct ModelBuilder modelBuilder, struct VulkanTools *vulkan) {
    struct Model result = { 0 };

    result.descriptorSetLayout = modelBuilder.createDescriptorSetLayout(vulkan->device, modelBuilder.texturesQuantity); // type
    result.pipelineLayout = createPipelineLayout(vulkan->device, result.descriptorSetLayout);
    result.graphicsPipeline = createGraphicsPipeline(modelBuilder.vertexShader, modelBuilder.fragmentShader, modelBuilder.minDepth, modelBuilder.maxDepth, vulkan->device, vulkan->renderPass, result.pipelineLayout, vulkan->msaaSamples);

    result.texturesQuantity = modelBuilder.texturesQuantity;
    result.texture = malloc(result.texturesQuantity * sizeof(struct Textures));
    for (uint32_t i = 0; i < result.texturesQuantity; i += 1) {
        result.texture[i] = loadTexture(modelBuilder.texturesPath[i], vulkan->device, vulkan->physicalDevice, vulkan->surface, vulkan->commandPool, vulkan->transferQueue);
    }

    createStorageBuffer(modelBuilder.instanceCount * sizeof(struct instanceBuffer), result.uniformModelBuffers, result.uniformModelBuffersMemory, result.uniformModelBuffersMapped, vulkan->device, vulkan->physicalDevice, vulkan->surface);
    result.instanceBuffer = malloc(modelBuilder.instanceCount * sizeof(struct instanceBuffer));

    result.instanceCount = modelBuilder.instanceCount;
    result.instance = malloc(sizeof(struct instance) * result.instanceCount);
    for (uint32_t i = 0; i < result.instanceCount; i += 1) {
        result.instanceBuffer[i] = (struct instanceBuffer) {
            .modelMatrix = { { 0 } },
            .textureIndex = 0
        };
    }

    modelBuilder.loadModel(modelBuilder.modelPath, &result, vulkan->device, vulkan->physicalDevice, vulkan->surface);

    for (uint32_t i = 0; i < result.meshQuantity; i += 1) {
        result.mesh[i].vertexBuffer = createVertexBuffer(&result.mesh[i].vertexBufferMemory, vulkan->device, vulkan->physicalDevice, vulkan->surface, vulkan->commandPool, vulkan->transferQueue, result.mesh[i].verticesQuantity, result.mesh[i].vertices);
        result.mesh[i].indexBuffer = createIndexBuffer(&result.mesh[i].indexBufferMemory, vulkan->device, vulkan->physicalDevice, vulkan->surface, vulkan->commandPool, vulkan->transferQueue, result.mesh[i].verticesQuantity, result.mesh[i].indicesQuantity, result.mesh[i].indices);
    }

    result.descriptorPool = modelBuilder.createDescriptorPool(result.texturesQuantity, vulkan->device);
    createDescriptorSets(result.descriptorSets, vulkan->device, result.descriptorPool, result.descriptorSetLayout);
    modelBuilder.bindBuffersToDescriptorSets(result.descriptorSets, vulkan->device, vulkan->uniformBuffers, result);

    return result;
}

void unloadTexture(VkDevice device, struct Textures texture) {
    vkDestroySampler(device, texture.sampler, NULL);
    vkDestroyImageView(device, texture.imageView, NULL);

    vkDestroyImage(device, texture.image, NULL);
    vkFreeMemory(device, texture.imageMemory, NULL);
}

void unloadModel(struct Vertex *vertices, uint16_t *indices) {
    free(vertices);
    free(indices);
}

void destroyModels(VkDevice device, struct Model model) {
    free(model.instance);

    for (uint32_t i = 0; i < model.meshQuantity; i += 1) {
        destroyBuffer(device, model.mesh[i].indexBuffer, model.mesh[i].indexBufferMemory);
        destroyBuffer(device, model.mesh[i].vertexBuffer, model.mesh[i].vertexBufferMemory);
    }

    for (uint32_t i = 0; i < model.texturesQuantity; i += 1) {
        unloadTexture(device, model.texture[i]);
    }
    free(model.texture);

    for (uint32_t i = 0; i < model.meshQuantity; i += 1) {
        unloadModel(model.mesh[i].vertices, model.mesh[i].indices);
    }
    free(model.mesh);

    destroyStorageBuffer(device, model.uniformModelBuffers, model.uniformModelBuffersMemory);
    destroyStorageBuffer(device, model.localMeshBuffers, model.localMeshBuffersMemory);

    vkDestroyPipeline(device, model.graphicsPipeline, NULL);
    vkDestroyPipelineLayout(device, model.pipelineLayout, NULL);
    vkDestroyDescriptorPool(device, model.descriptorPool, NULL);
    vkDestroyDescriptorSetLayout(device, model.descriptorSetLayout, NULL);
}
