#include "state.h"
#include "GLFW/glfw3.h"

struct button {
    struct Model *model;
    struct ButtonInfo {
        struct Position {
            float x;
            float y;
            int xPos;
            int yPos;
            float scaleX;
            float scaleY;
        };
        int newState;
    } *info;
    int chosen;
};

struct button calculateButtonPos(struct button button);
int checkForClick(GLFWwindow *window, struct button *button);
