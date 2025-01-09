#include "VulkanTools.h"
#include "state.h"

#include "model.h"
#include "modelBuilder.h"
#include "button.h"
#include "instanceBuffer.h"

#include "cohord.h"

void moveCamera(struct windowControl *windowControl, GLFWwindow *window, vec3 direction, vec3 cameraPos, vec2 cameraTilt);
void getCoordinate(vec3 centerC, struct VulkanTools vulkan);

void game(struct VulkanTools *vulkan, enum state *state) {
    uint32_t num = 20;
    uint32_t instanceCount = num * num;

    struct Model model[] = {
        /*floor*/ createModels(object(objLoader((struct ModelBuilder) {
            .instanceCount = 1,
            .texturesQuantity = 1,
            .texturesPath = (const char *[]){ "textures/grass.jpg" },
            .modelPath = "models/my_floor.obj",
            .vertexShader = "shaders/vert.spv",
            .fragmentShader = "shaders/frag2.spv",
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        })), vulkan),
        /*objects*/ createModels(object(gltfLoader((struct ModelBuilder) {
            .instanceCount = instanceCount,
            .texturesQuantity = 3,
            .texturesPath = (const char *[]){ "textures/texture.jpg", "textures/texture2.jpg", "textures/texture4.jpg" },
            .modelPath = "models/cylinder.glb",
            .vertexShader = "shaders/vert.spv",
            .fragmentShader = "shaders/frag.spv",
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        })), vulkan),
        /*buttons*/ createModels(interface(objLoader((struct ModelBuilder) {
            .instanceCount = 2,
            .texturesQuantity = 1,
            .texturesPath = (const char *[]){ "textures/exit.jpg" },
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
            .minDepth = 1.0f,
            .maxDepth = 1.0f
        })), vulkan),
    };

    vec2 pos[instanceCount];

    struct Model floor = model[0];
    *floor.instance = (struct instance){
        .pos = { 0.0f, 0.0f, -50.0f },
        .rotation = { 0.0f, 0.0f, 0.0f },
        .scale = { 100.0f, 100.0f, 100.0f },
        .textureIndex = 0
    };

    struct button button = {
        .model = &model[2],
        .info = (struct ButtonInfo[]){
            [0] = {
                .x = 1.0f,
                .y = 1.0f,
                .xPos = -1,
                .yPos = 1,
                .scaleX = 0.125f,
                .scaleY = 0.125f,
                .newState = MAIN_MENU
            }
        }
    };

    int click = -1;

    calculateButtonPos(button);

    *model[3].instance = (struct instance){
        .pos = { 0.0f, 0.0f, 0.5f },
        .rotation = { 0.0f, 0.0f, 0.0f },
        .scale = { 1.0f, 1.0f, 1.0f },
        .textureIndex = 0,
        .shadow = false
    };

    struct cohort cohort = {
        .info = model[1],
        .center = { 0.0f, 0.0f, 0.0f },
        .formation = 3,
        .currentFormation = 0,
        .warriorPos = pos
    };

    glm_vec3_fill(vulkan->cameraPos, 2.0f);
    vec2 tilt = { 0.0f, 40.0f };

    armyDisplacement(cohort.formation, cohort.info.instanceCount, cohort.warriorPos);
    setPosition(model[1], cohort.info.instanceCount, cohort.warriorPos);
    while (GAME == *state && !glfwWindowShouldClose(vulkan->window)) {
        glfwPollEvents();

        drawFrame(vulkan, sizeof(model) / sizeof(struct Model), model);
        click = checkForClick(vulkan->window, button);
        if (click != -1 && (KEY_PRESS | KEY_CHANGE) == getMouseState(vulkan->windowControl, GLFW_MOUSE_BUTTON_LEFT)) {
            *state = button.info[click].newState;
        }
        else {
            moveCamera(vulkan->windowControl, vulkan->window, vulkan->center, vulkan->cameraPos, tilt);
            if (GLFW_PRESS == glfwGetMouseButton(vulkan->window, GLFW_MOUSE_BUTTON_RIGHT)) {
                getCoordinate(cohort.center, *vulkan);
            }

            giveAcceleration(cohort);

            calculateVelocity(cohort.info.instanceCount, cohort.info.instance);
            move(cohort.info.instanceCount, cohort.info.instance);
            collision(cohort.info.instanceCount, cohort.info.instance, floor.instance[0]);

            armyDisplacement(2, cohort.info.instanceCount, cohort.warriorPos);
        }
    }

    vkDeviceWaitIdle(vulkan->device);
    destroyModels(vulkan->device, model[0]);
    destroyModels(vulkan->device, model[1]);
    destroyModels(vulkan->device, model[2]);
    destroyModels(vulkan->device, model[3]);
}
