#include "VulkanTools.h"
#include "state.h"

#include "model.h"
#include "modelBuilder.h"
#include "button.h"
#include "instanceBuffer.h"

#include "modelFunctions.h"

void win(struct VulkanTools *vulkan, enum state *state) {
    const char *texturePaths[] = {
        "textures/win.jpg"
    };

    struct descriptor textureDesc;
    struct Textures *textures = loadTextures(&textureDesc, &vulkan->graphics, sizeof(texturePaths) / sizeof(const char *), texturePaths);

    [[maybe_unused]]
    const char *modelPath[] = {
        "models/my_model2d.obj",
    };

    struct actualModel actualModel[sizeof(modelPath) / sizeof(const char *)];
    
    objLoadModel(modelPath[0], &actualModel[0], vulkan->graphics.device, vulkan->graphics.physicalDevice, vulkan->graphics.surface);

    for (uint32_t j = 0; j < sizeof(actualModel) / sizeof(struct actualModel); j += 1)
    for (uint32_t i = 0; i < actualModel[j].meshQuantity; i += 1) {
        actualModel[j].mesh[i].vertexBuffer = createVertexBuffer(&actualModel[j].mesh[i].vertexBufferMemory, vulkan->graphics.device, vulkan->graphics.physicalDevice, vulkan->graphics.surface, vulkan->graphics.commandPool, vulkan->graphics.transferQueue, actualModel[j].mesh[i].verticesQuantity, actualModel[j].mesh[i].vertices);
        actualModel[j].mesh[i].indexBuffer = createIndexBuffer(&actualModel[j].mesh[i].indexBufferMemory, vulkan->graphics.device, vulkan->graphics.physicalDevice, vulkan->graphics.surface, vulkan->graphics.commandPool, vulkan->graphics.transferQueue, actualModel[j].mesh[i].verticesQuantity, actualModel[j].mesh[i].indicesQuantity, actualModel[j].mesh[i].indices);
    }

    struct Model model[] = {
        /*background*/ createModels((struct ModelBuilder) {
            .instanceCount = 1,
            .texturesQuantity = 1,
            .texturePointer = 0,
            .texture = &textureDesc,
            .modelPath = &actualModel[0],
            .vertexShader = "shaders/vert2d.spv",
            .fragmentShader = "shaders/frag2d.spv",
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        }, &vulkan->graphics),
    };

    *model[0].instance = (struct instance){
        .pos = { 0.0f, 0.0f, 0.0f },
        .rotation = { 0.0f, 0.0f, 0.0f },
        .scale = { 1.0f, 1.0f, 1.0f },
        .textureIndex = 0,
        .shadow = false
    };

    while (*state == WIN && !glfwWindowShouldClose(vulkan->window)) {
        glfwPollEvents();

        if ((KEY_PRESS | KEY_CHANGE) == getMouseState(vulkan->windowControl, GLFW_MOUSE_BUTTON_LEFT)) {
            *state = MAIN_MENU;
        }

        drawFrame(vulkan, sizeof(model) / sizeof(struct Model), model);
    }

    destroyModelArray(sizeof(model) / sizeof(struct Model), model, &vulkan->graphics);
    destroyActualModels(vulkan->graphics.device, sizeof(actualModel) / sizeof(struct actualModel), actualModel);
    unloadTextures(vulkan->graphics.device, sizeof(texturePaths) / sizeof(const char *), textures, textureDesc);
}
