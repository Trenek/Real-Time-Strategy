#include "VulkanTools.h"
#include "model.h"
#include "modelBuilder.h"

struct Model warriors(uint16_t instanceCount, struct VulkanTools *vulkan) {
    return createModels(object(gltfLoader((struct ModelBuilder) {
        .instanceCount = instanceCount,
        .texturesQuantity = 3,
        .texturesPath = (const char *[]){
            "textures/texture.jpg",
            "textures/texture2.jpg",
            "textures/texture4.jpg"
        },
        .modelPath = "models/cylinder.glb",
        .vertexShader = "shaders/vert.spv",
        .fragmentShader = "shaders/frag.spv",
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    })), vulkan);
}
