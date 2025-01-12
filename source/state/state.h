#include <stdint.h>

#include "model.h"

enum state {
    MAIN_MENU,
    GAME,
    PAUSE,
    EXIT
};

void menu(struct VulkanTools *vulkan, enum state *state);
void game(struct VulkanTools *vulkan, enum state *state);
void pause(struct VulkanTools *vulkan, enum state *state, uint16_t modelQuantity, struct Model model[modelQuantity]);
