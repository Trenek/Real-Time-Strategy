#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "VulkanTools.h"

void update1(struct instance *this) {
    [[maybe_unused]] double time = clock() / (double)CLOCKS_PER_SEC;

    glm_mat4_identity(this->instanceInfo->modelMatrix);
    glm_translate(this->instanceInfo->modelMatrix, this->pos);
    glm_rotate(this->instanceInfo->modelMatrix, time * glm_rad(90.0f), this->pos);
    glm_scale_uni(this->instanceInfo->modelMatrix, 0.5f);
}

void update2(struct instance *this) {
    double time = clock() / (double)CLOCKS_PER_SEC;

    glm_mat4_identity(this->instanceInfo->modelMatrix);
    glm_translate(this->instanceInfo->modelMatrix, this->pos);
    glm_rotate(this->instanceInfo->modelMatrix, time * glm_rad(90.0f), (vec3) { 1, 1, 1 });
    glm_scale_uni(this->instanceInfo->modelMatrix, 0.05f);
}

void update3(struct instance *this) {
    glm_mat4_identity(this->instanceInfo->modelMatrix);
  //  glm_translate(this->instanceInfo->modelMatrix, this->pos);

    glm_scale_uni(this->instanceInfo->modelMatrix, 1.0f);
}

void moveCamera(GLFWwindow *window, vec3 direction, vec3 cameraPos, float *x, float *y) {
    static clock_t t = 0;
    clock_t delta = clock() - t;
    t = clock();
    mat4 id = {
        [0][0] = 1.0f,
        [1][1] = 1.0f,
        [2][2] = 1.0f,
        [3][3] = 1.0f,
    };
    vec3 initialDirection = {
        -1.0f,
        -1.0f,
        0.0f
    };
    static double prevXPos = 0;
    static double prevYPos = 0;

    if (GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
        glfwSetCursorPos(window, 0, 0);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwGetCursorPos(window, &prevXPos, &prevYPos);
    }

    if (GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if (GLFW_CURSOR_DISABLED == glfwGetInputMode(window, GLFW_CURSOR)) {
        double xpos;
        double ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (prevXPos == 0 && prevYPos == 0);
        else {
            *x -= (xpos - prevXPos) * 10 * delta / CLOCKS_PER_SEC;
            *y += (ypos - prevYPos) * 10 * delta / CLOCKS_PER_SEC;
        }
        prevXPos = xpos;
        prevYPos = ypos;
    }


    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT)) *x += glm_rad(4500) * delta / CLOCKS_PER_SEC;
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT)) *x -= glm_rad(4500) * delta / CLOCKS_PER_SEC;
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP)) *y -= glm_rad(4500) * delta / CLOCKS_PER_SEC;
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN)) *y += glm_rad(4500) * delta / CLOCKS_PER_SEC;

    if (*y > 80.0f) *y = 80.0f;
    if (*y < -80.0f) *y = -80.0f;

    glm_rotate(id, glm_rad(*x), (vec3) { 0.0f, 0.0f, 1.0f });
    glm_rotate(id, glm_rad(*y), (vec3) { -initialDirection[1], initialDirection[0], 0.0f });
    glm_mat4_mulv3(id, initialDirection, 1.0f, direction);

    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W)) {
        cameraPos[0] += direction[0] * 4.0f * delta / CLOCKS_PER_SEC;
        cameraPos[1] += direction[1] * 4.0f * delta / CLOCKS_PER_SEC;
    }
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S)) {
        cameraPos[0] -= direction[0] * 4.0f * delta / CLOCKS_PER_SEC;
        cameraPos[1] -= direction[1] * 4.0f * delta / CLOCKS_PER_SEC;
    }
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A)) {
        cameraPos[0] -= direction[1] * 4.0f * delta / CLOCKS_PER_SEC;
        cameraPos[1] += direction[0] * 4.0f * delta / CLOCKS_PER_SEC;
    }
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D)) {
        cameraPos[0] += direction[1] * 4.0f * delta / CLOCKS_PER_SEC;
        cameraPos[1] -= direction[0] * 4.0f * delta / CLOCKS_PER_SEC;
    }

    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_SPACE)) {
        cameraPos[2] += 5.0f * delta / CLOCKS_PER_SEC;
    }
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
        cameraPos[2] -= 5.0f * delta / CLOCKS_PER_SEC;
    }

    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_END)) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(void) {
    srand(time(NULL));

    uint32_t l = 20;

    uint32_t instanceTextureIndex[l * l * l];
    void (*update[l * l * l])(struct instance *this);
    vec3 pos[l * l * l];

    struct ModelBuilder builder1 = objLoader((struct ModelBuilder){
        .instanceCount = l * l * l,
        .instanceTextureIndex = instanceTextureIndex,
        .texturesQuantity = 3,
        .texturesPath = (const char *[]){ "textures/texture2.jpg", "textures/git.gif", "textures/texture4.jpg" },
        .modelPath = "models/my_model.obj",
        .update = update,
        .pos = pos,
        .fragmentShader = "shaders/frag.spv"
    });

    int z = 0;
    for (uint32_t i = 0; i < l; i += 1) {
        for (uint32_t j = 0; j < l; j += 1) {
            for (uint32_t k = 0; k < l; k += 1) {
                builder1.instanceTextureIndex[z] = z % builder1.texturesQuantity;
                builder1.update[z] = z % 2 == 0 ? update1 : update1;
                builder1.pos[z][0] = 0.0f - i;
                builder1.pos[z][1] = 0.0f - j;
                builder1.pos[z][2] = 0.0f - k;

                z += 1;
            }
        }

    }

    [[maybe_unused]]
    struct ModelBuilder builder2 = objLoader((struct ModelBuilder){
        .instanceCount = 3,
        .instanceTextureIndex = (uint32_t[]) { 0, 0, 0 },
        .texturesQuantity = 1,
        .texturesPath = (const char *[]){ "textures/preview.png" },
        .modelPath = "models/preview.obj",
        .update = (void (*[])(struct instance *this)) { update2, update2, update2 },
        .pos = (vec3[]) {
            { 1.0f, 0.0f, 1.0f },
            { 1.0f, 1.0f, 1.0f },
            { 0.0f, 1.0f, 1.0f }
        },
        .fragmentShader = "shaders/frag2.spv"
    });

    [[maybe_unused]]
    struct ModelBuilder builder3 = gltfLoader((struct ModelBuilder){
        .instanceCount = 3,
        .instanceTextureIndex = (uint32_t[]) { 0, 0, 0 },
        .texturesQuantity = 1,
        .texturesPath = (const char *[]){ "textures/preview.png" },
        .modelPath = "models/tutorial04.gltf",
        .update = (void (*[])(struct instance *this)) { update3, update3, update3 },
        .pos = (vec3[]) {
            { 1.0f, 0.0f, 1.0f },
            { 1.0f, 1.0f, 1.0f },
            { 0.0f, 1.0f, 1.0f }
        },
        .fragmentShader = "shaders/frag2.spv"
    });

    [[maybe_unused]]
    struct ModelBuilder builder4 = gltfLoader((struct ModelBuilder){
        .instanceCount = 3,
        .instanceTextureIndex = (uint32_t[]) { 0, 0, 0 },
        .texturesQuantity = 1,
        .texturesPath = (const char *[]){ "textures/preview.png" },
        .modelPath = "models/studnia.glb",
        .update = (void (*[])(struct instance *this)) { update2, update2, update2 },
        .pos = (vec3[]) {
            { 1.0f, 0.0f, 1.0f },
            { 1.0f, 1.0f, 1.0f },
            { 0.0f, 1.0f, 1.0f }
        },
        .fragmentShader = "shaders/frag2.spv"
    });

    struct ModelBuilder builderArray[] = {
        builder1,
      //  builder2,
      //  builder3,
      //  builder4
    };

    struct VulkanTools vulkan = setup(sizeof(builderArray) / sizeof(struct ModelBuilder), builderArray);

    vulkan.cameraPos[0] = 2.0f;
    vulkan.cameraPos[1] = 2.0f;
    vulkan.cameraPos[2] = 2.0f;

    float x = 0.0f;
    float y = 40.0f;
    while (!glfwWindowShouldClose(vulkan.window)) {
        glfwPollEvents();

        moveCamera(vulkan.window, vulkan.center, vulkan.cameraPos, &x, &y);
        switch (drawFrame(&vulkan)) {
            case VK_SUCCESS:
                break;
            case VK_SUBOPTIMAL_KHR:
            case VK_ERROR_OUT_OF_DATE_KHR:
                *vulkan.framebufferResized = true;
                break;
            default:
                fprintf(stderr, "Oh no");
                glfwSetWindowShouldClose(vulkan.window, GLFW_TRUE);
                break;
        }

        if (*vulkan.framebufferResized) {
            *vulkan.framebufferResized = false;

            recreateSwapChain(&vulkan);
        }
    }

    cleanup(vulkan);

    return 0;
}
