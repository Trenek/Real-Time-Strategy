#include <string.h>

#include "VulkanTools.h"
#include "state.h"

#include "model.h"
#include "modelBuilder.h"
#include "button.h"
#include "instanceBuffer.h"

#include "cohord.h"
#include "army.h"

#include "modelFunctions.h"

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

    const char *texturePaths[] = {
        "textures/pause.png",
        "textures/grass.jpg",
        "textures/texture.jpg",

        "textures/warrior1/high.jpg", "textures/warrior1/mid.jpg", "textures/warrior1/low.jpg",
        "textures/warrior2/high.jpg", "textures/warrior2/mid.jpg", "textures/warrior2/low.jpg",
        "textures/warrior3/high.jpg", "textures/warrior3/mid.jpg", "textures/warrior3/low.jpg",

        "textures/enemyWarrior1/high.jpg", "textures/enemyWarrior1/mid.jpg", "textures/enemyWarrior1/low.jpg",
        "textures/enemyWarrior2/high.jpg", "textures/enemyWarrior2/mid.jpg", "textures/enemyWarrior2/low.jpg",
        "textures/enemyWarrior3/high.jpg", "textures/enemyWarrior3/mid.jpg", "textures/enemyWarrior3/low.jpg",
    };
    size_t texturesQuantity = sizeof(texturePaths) / sizeof(const char *);

    struct descriptor textureDesc;
    struct Textures *textureData = loadTextures(&textureDesc, &vulkan->graphics, texturesQuantity, texturePaths);

    [[maybe_unused]]
    const char *modelPath[] = {
        "models/my_model2d.obj",
        "models/my_floor.obj",
        "models/cylinder.glb"
    };

    struct actualModel actualModel[sizeof(modelPath) / sizeof(const char *)];
    
    objLoadModel(modelPath[0], &actualModel[0], vulkan->graphics.device, vulkan->graphics.physicalDevice, vulkan->graphics.surface);
    objLoadModel(modelPath[1], &actualModel[1], vulkan->graphics.device, vulkan->graphics.physicalDevice, vulkan->graphics.surface);
    gltfLoadModel(modelPath[2], &actualModel[2], vulkan->graphics.device, vulkan->graphics.physicalDevice, vulkan->graphics.surface);

    for (uint32_t j = 0; j < sizeof(actualModel) / sizeof(struct actualModel); j += 1)
    for (uint32_t i = 0; i < actualModel[j].meshQuantity; i += 1) {
        actualModel[j].mesh[i].vertexBuffer = createVertexBuffer(&actualModel[j].mesh[i].vertexBufferMemory, vulkan->graphics.device, vulkan->graphics.physicalDevice, vulkan->graphics.surface, vulkan->graphics.commandPool, vulkan->graphics.transferQueue, actualModel[j].mesh[i].verticesQuantity, actualModel[j].mesh[i].vertices);
        actualModel[j].mesh[i].indexBuffer = createIndexBuffer(&actualModel[j].mesh[i].indexBufferMemory, vulkan->graphics.device, vulkan->graphics.physicalDevice, vulkan->graphics.surface, vulkan->graphics.commandPool, vulkan->graphics.transferQueue, actualModel[j].mesh[i].verticesQuantity, actualModel[j].mesh[i].indicesQuantity, actualModel[j].mesh[i].indices);
    }

    struct Model model[] = {
        /*buttons*/ createModels((struct ModelBuilder) {
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
        /*floor*/ createModels((struct ModelBuilder) {
            .instanceCount = 1,
            .texturesQuantity = 1,
            .texturePointer = 1,
            .texture = &textureDesc,
            .modelPath = &actualModel[1],
            .vertexShader = "shaders/vert.spv",
            .fragmentShader = "shaders/frag.spv",
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        }, &vulkan->graphics),
        /*background*/ createModels((struct ModelBuilder) {
            .instanceCount = 1,
            .texturesQuantity = 1,
            .texturePointer = 2,
            .texture = &textureDesc,
            .modelPath = &actualModel[0],
            .vertexShader = "shaders/vert2d.spv",
            .fragmentShader = "shaders/frag2d.spv",
            .minDepth = 1.0f,
            .maxDepth = 1.0f
        }, &vulkan->graphics),
        /*buttons*/ createModels((struct ModelBuilder) {
            .instanceCount = 6,
            .texturesQuantity = 1,
            .texturePointer = 3,
            .texture = &textureDesc,
            .modelPath = &actualModel[0],
            .vertexShader = "shaders/vert2d.spv",
            .fragmentShader = "shaders/frag2d.spv",
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        }, &vulkan->graphics),
        warriors(instanceCount, &vulkan->graphics, &textureDesc, &actualModel[2]),
        warriors(instanceCount, &vulkan->graphics, &textureDesc, &actualModel[2]),
        warriors(instanceCount, &vulkan->graphics, &textureDesc, &actualModel[2]),
        warriors(instanceCount, &vulkan->graphics, &textureDesc, &actualModel[2]),
        warriors(instanceCount, &vulkan->graphics, &textureDesc, &actualModel[2]),
        warriors(instanceCount, &vulkan->graphics, &textureDesc, &actualModel[2]),

        enemy(instanceCount, &vulkan->graphics, &textureDesc, &actualModel[2]),
        enemy(instanceCount, &vulkan->graphics, &textureDesc, &actualModel[2]),
        enemy(instanceCount, &vulkan->graphics, &textureDesc, &actualModel[2]),
        enemy(instanceCount, &vulkan->graphics, &textureDesc, &actualModel[2]),
        enemy(instanceCount, &vulkan->graphics, &textureDesc, &actualModel[2]),
        enemy(instanceCount, &vulkan->graphics, &textureDesc, &actualModel[2]),
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
    destroyActualModels(vulkan->graphics.device, sizeof(actualModel) / sizeof(struct actualModel), actualModel);
    unloadTextures(vulkan->graphics.device, texturesQuantity, textureData, textureDesc);
}
