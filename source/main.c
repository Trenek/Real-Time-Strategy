﻿#include <stdlib.h>
#include <time.h>

#include "VulkanTools.h"

#include "state.h"

int main(void) {
    struct VulkanTools vulkan = setup();
    void (* const state[])(struct VulkanTools *vulkan, enum state *state) = {
        [MAIN_MENU] = menu,
        [GAME] = game,
        [WIN] = win,
        [LOSE] = lose
    };
    enum state stateID = MAIN_MENU;

    srand(time(NULL));
    do {
        state[stateID](&vulkan, &stateID);
    } while (stateID != EXIT && !glfwWindowShouldClose(vulkan.window));

    cleanup(vulkan);

    return 0;
}
