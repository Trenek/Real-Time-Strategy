#ifndef STATE_H
#define STATE_H

#include <stdint.h>

#include "model.h"

struct VulkanTools;
enum state {
    MAIN_MENU,
    GAME,
    PAUSE,
    WIN,
    LOSE,
    EXIT
};

void menu(struct VulkanTools *vulkan, enum state *state);
void game(struct VulkanTools *vulkan, enum state *state);
void pause(struct VulkanTools *vulkan, enum state *state, uint16_t modelQuantity, struct Model model[modelQuantity]);
void win(struct VulkanTools *vulkan, enum state *state);
void lose(struct VulkanTools *vulkan, enum state *state);

#endif
