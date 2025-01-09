#include "button.h"
#include "instanceBuffer.h"
#include "model.h"

void calculateButtonPos(struct button button) {
    for (uint16_t i = 0; i < button.model->instanceCount; i += 1) {
        button.model->instance[i] = (struct instance) {
            .pos = {
                button.info[i].x + button.info[i].xPos * button.info[i].scaleX,
                button.info[i].y - button.info[i].yPos * button.info[i].scaleY,
                0.0f
            },
            .rotation = { 0.0f, 0.0f, 0.0f },
            .scale = {
                button.info[i].scaleX,
                1.0f,
                button.info[i].scaleY
            },
            .textureIndex = i,
            .shadow = true
        };
    }
}

int checkForClick(GLFWwindow *window, struct button button) {
    double x = 0.0;
    double y = 0.0;
    int width = 0;
    int height = 0;
    int result = -1;

    glfwGetCursorPos(window, &x, &y);
    glfwGetWindowSize(window, &width, &height);

    for (uint16_t i = 0; i < button.model->instanceCount; i += 1) {
        struct instance *instance = &button.model->instance[i];
        instance->shadow =
            (instance->pos[0] - instance->scale[0] + 1) * (width / 2.0) <= x &&
            (instance->pos[0] + instance->scale[0] + 1) * (width / 2.0) >= x &&
            (instance->pos[1] - instance->scale[2] + 1) * (height / 2.0) <= y &&
            (instance->pos[1] + instance->scale[2] + 1) * (height / 2.0) >= y;

        if (instance->shadow) result = i;
    }

    return result;
}
