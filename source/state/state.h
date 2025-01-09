enum state {
    MAIN_MENU,
    GAME,
    EXIT
};

void menu(struct VulkanTools *vulkan, enum state *state);
void game(struct VulkanTools *vulkan, enum state *state);
