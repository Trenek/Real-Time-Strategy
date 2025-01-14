#include <string.h>
#include <stdint.h>

#include "VulkanTools.h"
#include "state.h"
#include "button.h"

#include "modelBuilder.h"

void pause(struct VulkanTools *vulkan, enum state *state, uint16_t modelQuantity, struct Model model[modelQuantity]) {
    struct Model pauseModel[modelQuantity + 1];
    pauseModel[0] = /*buttons*/ createModels(interface(objLoader((struct ModelBuilder) {
        .instanceCount = 2,
        .texturesQuantity = 2,
        .texturesPath = (const char *[]){ "textures/exit.jpg", "textures/pause.png" },
        .modelPath = "models/my_model2d.obj",
        .vertexShader = "shaders/vert2d.spv",
        .fragmentShader = "shaders/frag2d.spv",
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    })), &vulkan->graphics);

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
}
