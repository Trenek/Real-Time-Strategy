#include "model.h"
#include "bufferOperations.h"

#include "modelFunctions.h"

struct Textures loadTexture(const char *texturePath, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkCommandPool commandPool, VkQueue queue) {
    struct Textures result = { 0 };

    result.image = createTextureBuffer(&result.imageMemory, &result.mipLevels, texturePath, device, physicalDevice, surface, commandPool, queue);
    result.imageView = createTextureImageView(device, result.image, result.mipLevels);
    result.sampler = createTextureSampler(device, physicalDevice, result.mipLevels);

    return result;
}

struct Model createModels(struct ModelBuilder modelBuilder, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkRenderPass renderPass, VkSampleCountFlagBits msaaSamples, VkBuffer uniformBuffers[], VkCommandPool commandPool, VkQueue queue) {
    struct Model result = { 0 };

    result.descriptorSetLayout = createDescriptorSetLayout(device, modelBuilder.texturesQuantity);
    result.pipelineLayout = createPipelineLayout(device, result.descriptorSetLayout);
    result.graphicsPipeline = createGraphicsPipeline(modelBuilder.fragmentShader, device, renderPass, result.pipelineLayout, msaaSamples);

    result.texturesQuantity = modelBuilder.texturesQuantity;
    result.texture = malloc(result.texturesQuantity * sizeof(struct Textures));
    for (uint32_t i = 0; i < result.texturesQuantity; i += 1) {
        result.texture[i] = loadTexture(modelBuilder.texturesPath[i], device, physicalDevice, surface, commandPool, queue);
    }

    createStorageBuffer(modelBuilder.instanceCount * sizeof(struct instanceInfo), result.uniformModelBuffers, result.uniformModelBuffersMemory, result.uniformModelBuffersMapped, device, physicalDevice, surface);
    result.instanceInfo = malloc(modelBuilder.instanceCount * sizeof(struct instanceInfo));

    result.instanceCount = modelBuilder.instanceCount;
    result.instance = modelBuilder.instance;
    for (uint32_t i = 0; i < result.instanceCount; i += 1) {
        result.instanceInfo[i] = (struct instanceInfo) {
            .modelMatrix = { { 0 } },
            .textureIndex = 0
        };
    }

    modelBuilder.loadModel(modelBuilder.modelPath, &result, device, physicalDevice, surface);

    for (uint32_t i = 0; i < result.meshQuantity; i += 1) {
        result.mesh[i].vertexBuffer = createVertexBuffer(&result.mesh[i].vertexBufferMemory, device, physicalDevice, surface, commandPool, queue, result.mesh[i].verticesQuantity, result.mesh[i].vertices);
        result.mesh[i].indexBuffer = createIndexBuffer(&result.mesh[i].indexBufferMemory, device, physicalDevice, surface, commandPool, queue, result.mesh[i].verticesQuantity, result.mesh[i].indicesQuantity, result.mesh[i].indices);
    }

    result.descriptorPool = createDescriptorPool(result.texturesQuantity, device);
    createDescriptorSets(result.descriptorSets, device, result.descriptorPool, uniformBuffers, result.descriptorSetLayout, result);

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
