#include <string.h>

#include "button.h"
#include "instanceBuffer.h"
#include "model.h"

struct button calculateButtonPos(struct button button) {
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
            .textureIndex = i % button.model->texturesQuantity,
            .shadow = true
        };
    }

    button.chosen = -1;

    return button;
}

void swap(void *a, void *b, size_t size) {
    if (a != b) {
        uint8_t temp[size];
        memcpy(temp, a, size);
        memcpy(a, b, size);
        memcpy(b, temp, size);
    }
}

void swap2(void *ptr, size_t sizeA, size_t sizeB) {
    uint8_t *mem = ptr;
    uint8_t temp[sizeA];
    memcpy(temp, mem, sizeA);
    memcpy(mem, mem + sizeA, sizeB);
    memcpy(mem + sizeB, temp, sizeA);
}

int checkForClick(GLFWwindow *window, struct button *button) {
    double x = 0.0;
    double y = 0.0;
    int width = 0;
    int height = 0;
    int result = -1;

    glfwGetCursorPos(window, &x, &y);
    glfwGetWindowSize(window, &width, &height);

    for (uint16_t i = 0; result == -1 && i < button->model->instanceCount; i += 1) {
        struct instance *instance = &button->model->instance[i];
        instance->shadow =
            (instance->pos[0] - instance->scale[0] + 1) * (width / 2.0) <= x &&
            (instance->pos[0] + instance->scale[0] + 1) * (width / 2.0) >= x &&
            (instance->pos[1] - instance->scale[2] + 1) * (height / 2.0) <= y &&
            (instance->pos[1] + instance->scale[2] + 1) * (height / 2.0) >= y;

        if (instance->shadow) {
            result = i;
            i += 1;
            while (i < button->model->instanceCount) {
                button->model->instance[i].shadow = false;

                i += 1;
            }
        }
    }

    if (result == 0 && button->chosen == -1) {
        button->chosen = 0;
    }
    else if (result == 0);
    else if (result == -1) {
        if (button->chosen != -1) {
            swap2(button->model->instance, sizeof(struct instance), sizeof(struct instance) * button->chosen);
            swap2(button->info, sizeof(struct ButtonInfo), sizeof(struct ButtonInfo) * button->chosen);
        }

        button->chosen = -1;
    }
    else {
        if (button->chosen != -1) {
            swap2(button->model->instance, sizeof(struct instance), sizeof(struct instance) * button->chosen);
            swap2(button->info, sizeof(struct ButtonInfo), sizeof(struct ButtonInfo) * button->chosen);
        }

        if (result <= button->chosen) {
            result -= 1;
        }

        if (result != 0) {
            swap2(button->model->instance, sizeof(struct instance) * result, sizeof(struct instance));
            swap2(button->info, sizeof(struct ButtonInfo) * result, sizeof(struct ButtonInfo));
        }

        button->chosen = result;
    }

    return result;
}
