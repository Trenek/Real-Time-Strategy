#include "VulkanTools.h"
#include "state.h"

#include "model.h"
#include "modelBuilder.h"
#include "button.h"
#include "instanceBuffer.h"

void win(struct VulkanTools *vulkan, enum state *state) {
    struct Model model[] = {
        /*background*/ createModels(objLoader((struct ModelBuilder) {
            .instanceCount = 1,
            .texturesQuantity = 1,
            .texturesPath = (const char *[]){ "textures/win.jpg" },
            .modelPath = "models/my_model2d.obj",
            .vertexShader = "shaders/vert2d.spv",
            .fragmentShader = "shaders/frag2d.spv",
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        }), &vulkan->graphics),
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
}
