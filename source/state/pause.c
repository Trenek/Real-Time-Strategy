#include <string.h>
#include <stdint.h>

#include "VulkanTools.h"
#include "state.h"
#include "button.h"

#include "modelBuilder.h"
#include "modelFunctions.h"

void pause(struct VulkanTools *vulkan, enum state *state, uint16_t modelQuantity, struct Model model[modelQuantity]) {
    const char *texturesPath[] = {
        "textures/exit.jpg",
        "textures/pause.png"
    };

    struct descriptor textureDesc;
    struct Textures *texture = loadTextures(&textureDesc, &vulkan->graphics, sizeof(texturesPath) / sizeof(const char *), texturesPath);

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

    struct Model pauseModel[modelQuantity + 1];
    pauseModel[0] = /*buttons*/ createModels((struct ModelBuilder) {
        .instanceCount = 2,
        .texturesQuantity = 2,
        .texture = &textureDesc,
        .texturePointer = 0,
        .modelPath = &actualModel[0],
        .vertexShader = "shaders/vert2d.spv",
        .fragmentShader = "shaders/frag2d.spv",
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    }, &vulkan->graphics);

    memcpy(pauseModel + 1, model, sizeof(struct Model) * modelQuantity);

    struct button button = calculateButtonPos((struct button){
        .model = &pauseModel[0],
        .info = (struct ButtonInfo[]){
            [0] = {
                .x = 1.0f,
                .y = 1.0f,
                .xPos = -1,
                .yPos = 1,
                .scaleX = 0.125f,
                .scaleY = 0.125f,
                .newState = MAIN_MENU
            },
            [1] = {
                .x = 1.0f,
                .y = -1.0f,
                .xPos = -1,
                .yPos = -1,
                .scaleX = 0.125f,
                .scaleY = 0.125f,
                .newState = GAME
            }
        }
    });

    int click = 0;

    while (*state == PAUSE && !glfwWindowShouldClose(vulkan->window)) {
        glfwPollEvents();

        drawFrame(vulkan, sizeof(pauseModel) / sizeof(struct Model), pauseModel);
        click = checkForClick(vulkan->window, &button);
        if ((KEY_PRESS | KEY_CHANGE) == getMouseState(vulkan->windowControl, GLFW_MOUSE_BUTTON_LEFT) && click != -1) {
            *state = button.info[click].newState;
        }
    }

    destroyModelArray(1, pauseModel, &vulkan->graphics);
    destroyActualModels(vulkan->graphics.device, sizeof(actualModel) / sizeof(struct actualModel), actualModel);
    unloadTextures(vulkan->graphics.device, sizeof(texturesPath) / sizeof(const char *), texture, textureDesc);
}
