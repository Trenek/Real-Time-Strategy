#include "VulkanTools.h"
#include "state.h"

#include "model.h"
#include "modelBuilder.h"
#include "button.h"
#include "instanceBuffer.h"

void menu(struct VulkanTools *vulkan, enum state *state) {
    struct Model model[] = {
        /*buttons*/ createModels(interface(objLoader((struct ModelBuilder) {
            .instanceCount = 2,
            .texturesQuantity = 2,
            .texturesPath = (const char *[]){ "textures/start.png", "textures/exit.jpg" },
            .modelPath = "models/my_model2d.obj",
            .vertexShader = "shaders/vert2d.spv",
            .fragmentShader = "shaders/frag2d.spv",
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        })), vulkan),
        /*background*/ createModels(interface(objLoader((struct ModelBuilder) {
            .instanceCount = 1,
            .texturesQuantity = 1,
            .texturesPath = (const char *[]){ "textures/texture.jpg" },
            .modelPath = "models/my_model2d.obj",
            .vertexShader = "shaders/vert2d.spv",
            .fragmentShader = "shaders/frag2d.spv",
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        })), vulkan),
    };

    struct button button = calculateButtonPos((struct button){
        .model = &model[0],
        .info = (struct ButtonInfo[]) {
            [0] = {
                .x = 0.0f,
                .y = 0.0f,
                .xPos = 0,
                .yPos = 0,
                .scaleX = 0.125f,
                .scaleY = 0.125f,
                .newState = GAME
            },
            [1] = {
                .x = 0.0f,
                .y = 0.3f,
                .xPos = 0,
                .yPos = 0,
                .scaleX = 0.125f,
                .scaleY = 0.125f,
                .newState = EXIT
            }
        }
    });

    int click = -1;

    *model[1].instance = (struct instance){
        .pos = { 0.0f, 0.0f, 0.0f },
        .rotation = { 0.0f, 0.0f, 0.0f },
        .scale = { 1.0f, 1.0f, 1.0f },
        .textureIndex = 0,
        .shadow = false
    };

    while (*state == MAIN_MENU && !glfwWindowShouldClose(vulkan->window)) {
        glfwPollEvents();

        drawFrame(vulkan, sizeof(model) / sizeof(struct Model), model);
        click = checkForClick(vulkan->window, &button);
        if ((KEY_PRESS | KEY_CHANGE) == getMouseState(vulkan->windowControl, GLFW_MOUSE_BUTTON_LEFT) && click != -1) {
            *state = button.info[click].newState;
        }

    }

    vkDeviceWaitIdle(vulkan->device);
    destroyModels(vulkan->device, model[0]);
    destroyModels(vulkan->device, model[1]);
}
