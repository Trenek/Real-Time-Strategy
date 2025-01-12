#include <string.h>

#include "VulkanTools.h"
#include "state.h"

#include "model.h"
#include "modelBuilder.h"
#include "button.h"
#include "instanceBuffer.h"

#include "cohord.h"
#include "army.h"

void moveCamera(struct windowControl *windowControl, GLFWwindow *window, vec3 direction, vec3 cameraPos, vec2 cameraTilt);
void getCoordinate(vec3 centerC, struct VulkanTools vulkan);

struct button createArmyButtons(struct button button, struct army *army) {
    for (uint16_t i = 0; i < army->cohortCount; i += 1) {
        button.info[i] = (struct ButtonInfo) {
            .x = 0.12f * i - (army->cohortCount + 1) * 0.1f * 0.5f,
            .y = 0.75f,
            .xPos = 0.0f,
            .yPos = 0.0f,
            .scaleX = 0.1f,
            .scaleY = 0.2f,
            .newState = i,
        };
    }

    return calculateButtonPos(button);
}

void game(struct VulkanTools *vulkan, enum state *state) {
    uint32_t num = 10;
    uint32_t instanceCount = num * num;

    struct Model model[] = {
        /*buttons*/ createModels(interface(objLoader((struct ModelBuilder) {
            .instanceCount = 1,
            .texturesQuantity = 1,
            .texturesPath = (const char *[]){ "textures/pause.png" },
            .modelPath = "models/my_model2d.obj",
            .vertexShader = "shaders/vert2d.spv",
            .fragmentShader = "shaders/frag2d.spv",
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        })), vulkan),
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
        /*buttons*/ createModels(interface(objLoader((struct ModelBuilder) {
            .instanceCount = 12,
            .texturesQuantity = 1,
            .texturesPath = (const char *[]){
                "textures/pause.png",
            },
            .modelPath = "models/my_model2d.obj",
            .vertexShader = "shaders/vert2d.spv",
            .fragmentShader = "shaders/frag2d.spv",
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        })), vulkan),
        warriors(instanceCount, vulkan),
        warriors(instanceCount, vulkan),
        warriors(instanceCount, vulkan),
        warriors(instanceCount, vulkan),
        warriors(instanceCount, vulkan),
        warriors(instanceCount, vulkan),
        warriors(instanceCount, vulkan),
        warriors(instanceCount, vulkan),
        warriors(instanceCount, vulkan),
        warriors(instanceCount, vulkan),
        warriors(instanceCount, vulkan),
        warriors(instanceCount, vulkan),

        warriors(instanceCount, vulkan),
    };

    struct Model floor = model[1];
    *floor.instance = (struct instance){
        .pos = { 0.0f, 0.0f, -50.0f },
        .rotation = { 0.0f, 0.0f, 0.0f },
        .scale = { 10000.0f, 100.0f, 10000.0f },
        .textureIndex = 0
    };

    struct button button = calculateButtonPos((struct button){
        .model = &model[0],
        .info = (struct ButtonInfo[]){
            [0] = {
                .x = 1.0f,
                .y = -1.0f,
                .xPos = -1,
                .yPos = -1,
                .scaleX = 0.125f,
                .scaleY = 0.125f,
                .newState = PAUSE
            }
        }
    });

    int click = -1;

    *model[2].instance = (struct instance){
        .pos = { 0.0f, 0.0f, 0.5f },
        .rotation = { 0.0f, 0.0f, 0.0f },
        .scale = { 1.0f, 1.0f, 1.0f },
        .textureIndex = 0,
        .shadow = false
    };

    struct army enemyArmy = setupArmy((struct army){
        .cohortCount = 1,
        .cohort = (struct cohort[]) {
            {
                .info = model[16],
                .center = { 8 * 20.0f, 0.0f, 0.0f },
                .formation = 3,
                .currentFormation = 0,
            }
        }
    });

    struct army army = setupArmy((struct army){
        .cohortCount = 12,
        .cohort = (struct cohort[]) {
            {
                .info = model[4],
                .center = { 0.0f, 0.0f, 0.0f },
                .formation = 3,
                .currentFormation = 0,
                .enemyToFight = &enemyArmy.cohort[0]
            }, {
                .info = model[5],
                .center = { 0.0f, 8 * 10.0f, 0.0f },
                .formation = 3,
                .currentFormation = 0,
            }, {
                .info = model[6],
                .center = { 0.0f, 8 * -10.0f, 0.0f },
                .formation = 3,
                .currentFormation = 0,
            }, {
                .info = model[7],
                .center = { 8 * -10.0f, 0.0f, 0.0f },
                .formation = 3,
                .currentFormation = 0,
            }, {
                .info = model[8],
                .center = { 8 * -10.0f, 8 * 10.0f, 0.0f },
                .formation = 3,
                .currentFormation = 0,
            }, {
                .info = model[9],
                .center = { 8 * -10.0f, 8 * -10.0f, 0.0f },
                .formation = 3,
                .currentFormation = 0,
            }, {
                .info = model[10],
                .center = { 8 * -20.0f, 0.0f, 0.0f },
                .formation = 3,
                .currentFormation = 0,
            }, {
                .info = model[11],
                .center = { 8 * -20.0f, 8 * 10.0f, 0.0f },
                .formation = 3,
                .currentFormation = 0,
            }, {
                .info = model[12],
                .center = { 8 * -20.0f, 8 * -10.0f, 0.0f },
                .formation = 3,
                .currentFormation = 0,
            }, {
                .info = model[13],
                .center = { 8 * -30.0f, 0.0f, 0.0f },
                .formation = 3,
                .currentFormation = 0,
            }, {
                .info = model[14],
                .center = { 8 * -30.0f, 8 * 10.0f, 0.0f },
                .formation = 3,
                .currentFormation = 0,
            }, {
                .info = model[15],
                .center = { 8 * -30.0f, 8 * -10.0f, 0.0f },
                .formation = 3,
                .currentFormation = 0,
            }
        }
    });

    struct army *armyArray[] = {
        &army,
        &enemyArmy
    };

    struct button button2 = createArmyButtons((struct button){
        .model = &model[3],
        .info = (struct ButtonInfo[1]) {
            {
                { 0 },
                .newState = 0
            }
        }
    }, &army);

    glm_vec3_fill(vulkan->cameraPos, 2.0f);
    vulkan->cameraPos[0] = 80.0f;
    vulkan->cameraPos[1] = 0.0f;
    vulkan->cameraPos[2] = 80.0f;
    vec2 tilt = { -45.0f, 45.0f };

    while (GAME == *state && !glfwWindowShouldClose(vulkan->window)) {
        glfwPollEvents();

        drawFrame(vulkan, sizeof(model) / sizeof(struct Model), model);

        bool isLeftMouseButtonClicked = (KEY_PRESS | KEY_CHANGE) == getMouseState(vulkan->windowControl, GLFW_MOUSE_BUTTON_LEFT);

        chooseCohort(&army, &button2, vulkan, isLeftMouseButtonClicked);

        click = checkForClick(vulkan->window, &button);
        if (isLeftMouseButtonClicked && click != -1) {
            *state = button.info[click].newState;
        }
        else {
            moveCamera(vulkan->windowControl, vulkan->window, vulkan->center, vulkan->cameraPos, tilt);
            if (GLFW_PRESS == glfwGetMouseButton(vulkan->window, GLFW_MOUSE_BUTTON_RIGHT)) {
                getCoordinate(army.cohort[army.chosenOne].center, *vulkan);
            }

            moveArmy(vulkan->deltaTime.deltaTime, &army);
            moveArmy(vulkan->deltaTime.deltaTime, &enemyArmy);
            armyCollision(sizeof(armyArray) / sizeof(struct army *), armyArray, floor.instance[0]);
        }

        if (*state == PAUSE) {
            pause(vulkan, state, (sizeof(model) / sizeof(struct Model)) - 1, model + 1);
        }
    }

    vkDeviceWaitIdle(vulkan->device);

    freeArmy(&army);
    freeArmy(&enemyArmy);
    for (uint16_t i = 0; i < sizeof(model) / sizeof(struct Model); i += 1) {
        destroyModels(vulkan->device, model[i]);
    }
}
