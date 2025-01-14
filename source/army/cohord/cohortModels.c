#include "VulkanTools.h"
#include "model.h"
#include "modelBuilder.h"

struct Model warriors(uint16_t instanceCount, struct GraphicsSetup *vulkan) {
    return createModels(object(gltfLoader((struct ModelBuilder) {
        .instanceCount = instanceCount,
        .texturesQuantity = 9,
        .texturesPath = (const char *[]){
            "textures/warrior1/high.jpg", "textures/warrior1/mid.jpg", "textures/warrior1/low.jpg",
            "textures/warrior2/high.jpg", "textures/warrior2/mid.jpg", "textures/warrior2/low.jpg",
            "textures/warrior3/high.jpg", "textures/warrior3/mid.jpg", "textures/warrior3/low.jpg",
        },
        .modelPath = "models/cylinder.glb",
        .vertexShader = "shaders/vert.spv",
        .fragmentShader = "shaders/frag.spv",
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    })), vulkan);
}

struct Model enemy(uint16_t instanceCount, struct GraphicsSetup *vulkan) {
    return createModels(object(gltfLoader((struct ModelBuilder) {
        .instanceCount = instanceCount,
        .texturesQuantity = 9,
        .texturesPath = (const char *[]){
            "textures/enemyWarrior1/high.jpg", "textures/enemyWarrior1/mid.jpg", "textures/enemyWarrior1/low.jpg",
            "textures/enemyWarrior2/high.jpg", "textures/enemyWarrior2/mid.jpg", "textures/enemyWarrior2/low.jpg",
            "textures/enemyWarrior3/high.jpg", "textures/enemyWarrior3/mid.jpg", "textures/enemyWarrior3/low.jpg",
        },
        .modelPath = "models/cylinder.glb",
        .vertexShader = "shaders/vert.spv",
        .fragmentShader = "shaders/frag.spv",
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    })), vulkan);
}
