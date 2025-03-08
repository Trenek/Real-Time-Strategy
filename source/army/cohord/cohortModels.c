#include "model.h"
#include "modelBuilder.h"

struct Model warriors(uint16_t instanceCount, struct GraphicsSetup *vulkan, struct descriptor *textureDescriptor, struct actualModel *actualModel) {
    return createModels((struct ModelBuilder) {
        .instanceCount = instanceCount,
        .texturesQuantity = 9,
        .texturePointer = 3,
        .texture = textureDescriptor,
        .modelPath = actualModel,
        .vertexShader = "shaders/vert.spv",
        .fragmentShader = "shaders/frag.spv",
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    }, vulkan);
}

struct Model enemy(uint16_t instanceCount, struct GraphicsSetup *vulkan, struct descriptor *textureDescriptor, struct actualModel *actualModel) {
    return createModels((struct ModelBuilder) {
        .instanceCount = instanceCount,
        .texturesQuantity = 9,
        .texturePointer = 12,
        .texture = textureDescriptor,
        .modelPath = actualModel,
        .vertexShader = "shaders/vert.spv",
        .fragmentShader = "shaders/frag.spv",
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    }, vulkan);
}
