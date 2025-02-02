#include <string.h>

#include "VulkanTools.h"
#include "state.h"

#include "model.h"
#include "modelBuilder.h"
#include "button.h"
#include "instanceBuffer.h"

#include "cohord.h"
#include "army.h"

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

void updateArmyStructure(struct button *button, struct army *army) {
    bool recalculate = false;

    for (uint16_t i = 0; i < army->cohortCount; i += 1) {
        if (army->cohort[i].info.instanceCount == 0) {
            army->cohort[i] = army->cohort[army->cohortCount - 1];

            if (button != NULL) {
                button->model->instance[i].pos[0] -= 2;
                button->info[i] = button->info[army->cohortCount - 1];
                button->model->instance[i] = button->model->instance[army->cohortCount - 1];

                button->model->instanceCount -= 1;
            }
            army->cohortCount -= 1;
            recalculate = true;
            i -= 1;
        }
    }

    if (button != NULL && recalculate) {
        *button = createArmyButtons(*button, army);
    }
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
        })), &vulkan->graphics),
        /*floor*/ createModels(object(objLoader((struct ModelBuilder) {
            .instanceCount = 1,
            .texturesQuantity = 1,
            .texturesPath = (const char *[]){ "textures/grass.jpg" },
            .modelPath = "models/my_floor.obj",
            .vertexShader = "shaders/vert.spv",
            .fragmentShader = "shaders/frag2.spv",
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        })), &vulkan->graphics),
        /*background*/ createModels(interface(objLoader((struct ModelBuilder) {
            .instanceCount = 1,
            .texturesQuantity = 1,
            .texturesPath = (const char *[]){ "textures/texture.jpg" },
            .modelPath = "models/my_model2d.obj",
            .vertexShader = "shaders/vert2d.spv",
            .fragmentShader = "shaders/frag2d.spv",
            .minDepth = 1.0f,
            .maxDepth = 1.0f
        })), &vulkan->graphics),
        /*buttons*/ createModels(interface(objLoader((struct ModelBuilder) {
            .instanceCount = 6,
            .texturesQuantity = 1,
            .texturesPath = (const char *[]){
                "textures/warrior1/high.jpg",
            },
            .modelPath = "models/my_model2d.obj",
            .vertexShader = "shaders/vert2d.spv",
            .fragmentShader = "shaders/frag2d.spv",
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        })), &vulkan->graphics),
        warriors(instanceCount, &vulkan->graphics),
        warriors(instanceCount, &vulkan->graphics),
        warriors(instanceCount, &vulkan->graphics),
        warriors(instanceCount, &vulkan->graphics),
        warriors(instanceCount, &vulkan->graphics),
        warriors(instanceCount, &vulkan->graphics),

        enemy(instanceCount, &vulkan->graphics),
        enemy(instanceCount, &vulkan->graphics),
        enemy(instanceCount, &vulkan->graphics),
        enemy(instanceCount, &vulkan->graphics),
        enemy(instanceCount, &vulkan->graphics),
        enemy(instanceCount, &vulkan->graphics),
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
        .cohortCount = 6,
        .cohort = (struct cohort[]) {
            warriorMovement((struct cohort) {
                .info = model[10],
                .center = { 8 * -20.0f, 0.0f, 0.0f },
                .formation = 3,
            }),
            warriorMovement((struct cohort) {
                .info = model[11],
                .center = { 8 * -20.0f, 8 * 10.0f, 0.0f },
                .formation = 3,
            }),
            warriorMovement((struct cohort) {
                .info = model[12],
                .center = { 8 * -20.0f, 8 * -10.0f, 0.0f },
                .formation = 3,
            }),
            warriorMovement((struct cohort) {
                .info = model[13],
                .center = { 8 * -30.0f, 0.0f, 0.0f },
                .formation = 3,
            }),
            warriorMovement((struct cohort) {
                .info = model[14],
                .center = { 8 * -30.0f, 8 * 10.0f, 0.0f },
                .formation = 3,
            }),
            warriorMovement((struct cohort) {
                .info = model[15],
                .center = { 8 * -30.0f, 8 * -10.0f, 0.0f },
                .formation = 3,
            })
        }
    });

    struct army army = setupArmy((struct army){
        .cohortCount = 6,
        .cohort = (struct cohort[]) {
            warriorMovement((struct cohort) {
                .info = model[4],
                .center = { 0.0f, 0.0f, 0.0f },
                .formation = 3,
            }),
            warriorMovement((struct cohort) {
                .info = model[5],
                .center = { 0.0f, 8 * 10.0f, 0.0f },
                .formation = 3,
            }),
            warriorMovement((struct cohort) {
                .info = model[6],
                .center = { 0.0f, 8 * -10.0f, 0.0f },
                .formation = 3,
            }),
            warriorMovement((struct cohort) {
                .info = model[7],
                .center = { 8 * -10.0f, 0.0f, 0.0f },
                .formation = 3,
            }),
            warriorMovement((struct cohort) {
                .info = model[8],
                .center = { 8 * -10.0f, 8 * 10.0f, 0.0f },
                .formation = 3,
            }),
            warriorMovement((struct cohort) {
                .info = model[9],
                .center = { 8 * -10.0f, 8 * -10.0f, 0.0f },
                .formation = 3,
            })
        }
    });

    [[maybe_unused]] struct army *armyArray[] = {
        &army,
        &enemyArmy
    };

    struct button button2 = createArmyButtons((struct button){
        .model = &model[3],
        .info = (struct ButtonInfo[6]) {}
    }, &army);

    vulkan->camera = initCamera();
    while (GAME == *state && !glfwWindowShouldClose(vulkan->window)) {
        glfwPollEvents();

        drawFrame(vulkan, sizeof(model) / sizeof(struct Model), model);

        bool isLeftMouseButtonClicked =  (KEY_PRESS | KEY_CHANGE) == getMouseState(vulkan->windowControl, GLFW_MOUSE_BUTTON_LEFT);
        bool isRightMouseButtonClicked = (KEY_PRESS | KEY_CHANGE) == getMouseState(vulkan->windowControl, GLFW_MOUSE_BUTTON_RIGHT);

        click = checkForClick(vulkan->window, &button);
        if (isLeftMouseButtonClicked && click != -1) {
            *state = button.info[click].newState;
        }
        else {
            moveCamera(vulkan->windowControl, vulkan->window, vulkan->camera.center, vulkan->camera.cameraPos, vulkan->camera.tilt, vulkan->deltaTime.deltaTime);
            if (isRightMouseButtonClicked && army.chosenOne != -1) {
                getCoordinate(army.cohort[army.chosenOne].center, *vulkan);
                army.cohort[army.chosenOne].enemyToFight = NULL;
            }

            moveArmy(vulkan->deltaTime.deltaTime, &army);
            moveArmy(vulkan->deltaTime.deltaTime, &enemyArmy);
            armyCollision(sizeof(armyArray) / sizeof(struct army *), armyArray, floor.instance[0]);

            armyAttack(&army, vulkan->deltaTime.deltaTime);
            armyAttack(&enemyArmy, vulkan->deltaTime.deltaTime);

            chooseCohort(&army, &button2, vulkan, isLeftMouseButtonClicked);
            chooseEnemy(&army, &enemyArmy, vulkan, isRightMouseButtonClicked);

            updateArmyStructure(&button2, &army);
            updateArmyStructure(NULL, &enemyArmy);
        }

        if (*state == PAUSE) {
            pause(vulkan, state, (sizeof(model) / sizeof(struct Model)) - 1, model + 1);
        }
        if (enemyArmy.cohortCount == 0) {
            *state = WIN;
        }
        if (army.cohortCount == 0) {
            *state = LOSE;
        }
    }

    freeArmy(&army);
    freeArmy(&enemyArmy);

    destroyModelArray(sizeof(model) / sizeof(struct Model), model, &vulkan->graphics);
}
