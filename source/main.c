#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "VulkanTools.h"

void moveCamera(GLFWwindow *window, vec3 direction, vec3 cameraPos, float *x, float *y) {
    static clock_t t = 0;
    clock_t newTime = clock();
    clock_t delta = newTime - t;
    t = newTime;
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

    static int o = 0;
    int p = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    if (GLFW_PRESS == p && GLFW_RELEASE == o) {
        if (GLFW_CURSOR_DISABLED == glfwGetInputMode(window, GLFW_CURSOR)) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            int width = 0;
            int height = 0;

            glfwGetWindowSize(window, &width, &height);

            glfwSetCursorPos(window, width / 2, height / 2);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwGetCursorPos(window, &prevXPos, &prevYPos);
        }
    }
    o = p;

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

void setPosition(struct ModelBuilder model, uint32_t num) {
    uint32_t z = 0;

    for (uint32_t i = 0; i < num; i += 1)
    for (uint32_t j = 0; j < num; j += 1) {
        model.instance[z].textureIndex = z % model.texturesQuantity;
        model.instance[z].pos[0] = 0.0f - i;
        model.instance[z].pos[1] = 0.0f - j;
        model.instance[z].pos[2] = 0.5f;
        //glm_vec3_dup(model.instance[z].pos, model.instance[z].rotation);
        //glm_vec3_normalize(model.instance[z].rotation);
        //glm_vec3_scale(model.instance[z].rotation, 2.0f, model.instance[z].rotation);
        model.instance[z].rotation[0] = 0.0f;
        model.instance[z].rotation[1] = 0.0f;
        model.instance[z].rotation[2] = 0.0f;
        glm_vec3_fill(model.instance[z].scale, 0.5f);
        model.instance[z].maxVelocity = 10e-1;
        glm_vec3_fill(model.instance[z].velocity, 0.0f);
        glm_vec3_fill(model.instance[z].acceleration, 0.0f);

        z += 1;
    }
}

float distance(vec3 cam, vec3 thing) {
    return glm_vec3_distance(cam, thing);
}

void swap(struct instance *a, struct instance *b) {
    struct instance c = *a;
    *a = *b;
    *b = c;
}

void insertionSort(uint32_t num, vec3 cameraPos, struct instance models[num]) {
    for (uint32_t i = 1; i < num; i += 1) {
        while (i > 0 && glm_vec3_distance(cameraPos, models[i].pos) < glm_vec3_distance(cameraPos, models[i - 1].pos)) {
            swap(&models[i - 1], &models[i]);

            i -= 1;
        }
    }
}

void giveAcceleration(vec2 actualCenter, uint32_t num, struct instance models[num]) {
    vec2 pos = { 0.0f, 0.0f };
    vec2 center;
    vec2 deltaX;
    uint32_t sqrtNum = (uint32_t)sqrt(num);

    static clock_t prev = 0; if (prev == 0) prev = clock();
    clock_t now = clock();
    float diff = (float)(now - prev) / CLOCKS_PER_SEC;

    if (diff != 0)
    for (uint32_t i = 0; i < num; i += 1) {
        glm_vec2_add(actualCenter, pos, center);

        glm_vec2_sub(center, models[i].pos, deltaX);
        float distance = glm_vec2_norm(deltaX);
        if (distance < models[i].scale[0] / 2) {
            glm_vec2_mul(models[i].velocity, models[i].velocity, models[i].acceleration);
            glm_vec2_scale(models[i].acceleration, -0.5f, models[i].acceleration);
            glm_vec2_div(models[i].acceleration, deltaX, models[i].acceleration);
            if (isinf(models[i].acceleration[0]) || isnan(models[i].acceleration[0])) models[i].acceleration[0] = 0.0f;
            if (isinf(models[i].acceleration[1]) || isnan(models[i].acceleration[1])) models[i].acceleration[1] = 0.0f;
        }
        else {
            vec2 vf2;

            glm_vec2_lerp((vec2) { 0.0f, 0.0f }, deltaX, models[i].maxVelocity / distance, vf2);
            glm_vec2_sub(vf2, models[i].velocity, vf2);
            glm_vec2_divs(vf2, diff, models[i].acceleration);
        }

        models[i].acceleration[2] = -9.81f;
        if (pos[0] == 2 * (sqrtNum - 1) * models[i].scale[0]) {
            pos[0] = 0;
            pos[1] += 2 * models[i].scale[0];
        }
        else {
            pos[0] += 2 * models[i].scale[0];
        }
    }

    prev = now;
}

void maxVelocity(vec2 v, float max) {
    float d = glm_vec2_norm(v);

    if (d > max) {
        glm_vec2_lerp((vec2) { 0.0f, 0.0f }, v, max / d, v);
    }
}

void calculateVelocity(uint32_t num, struct instance models[num]) {
    static clock_t prev = 0; if (prev == 0) prev = clock();
    clock_t current = clock();
    float delta = (float)(current - prev) / CLOCKS_PER_SEC;
    vec3 temp;

    for (uint32_t i = 0; i < num; i += 1) {
        glm_vec3_scale(models[i].acceleration, delta, temp);
        glm_vec3_add(models[i].velocity, temp, models[i].velocity);
        maxVelocity(models[i].velocity, models[i].maxVelocity);
    }

    prev = current;
}

void calculateNewVelocity(vec2 newVelocity, struct instance a1, struct instance a2) {
    vec2 deltaV;
    vec2 deltaX;
    vec2 velocityChange;

    glm_vec2_sub(a1.velocity, a2.velocity, deltaV);
    glm_vec2_sub(a1.pos, a2.pos, deltaX);

    float dotProduct = glm_vec2_dot(deltaV, deltaX);
    float norm = glm_vec2_norm(deltaX);

    glm_vec2_scale(deltaX, dotProduct / (norm * norm), velocityChange);
    glm_vec2_sub(a1.velocity, velocityChange, newVelocity);
}

void collision(uint32_t num, struct instance models[num], struct instance floor) {
    for (uint32_t this = 0; this < num; this += 1) {
        for (uint32_t i = this + 1; i < num; i += 1) {
            float minDistance = (models[this].scale[0] + models[i].scale[0]) / 2.0f;
            float distance = glm_vec2_distance(models[i].pos, models[this].pos);

            if (distance < minDistance) {
                vec2 n; {
                    float ratio = 0.5f;
                    float delta = 0.5f * 0.75f * (distance - minDistance);

                    glm_vec2_sub(models[this].pos, models[i].pos, n);
                    glm_vec2_scale(n, ratio * delta / distance, n);
                }
                glm_vec2_sub(models[this].pos, n, models[this].pos);
                glm_vec2_add(models[i].pos, n, models[i].pos);

                vec2 a1;
                vec2 a2;

                calculateNewVelocity(a1, models[this], models[i]);
                calculateNewVelocity(a2, models[i], models[this]);

                glm_vec2_copy(a1, models[this].velocity);
                glm_vec2_copy(a2, models[i].velocity);
                maxVelocity(models[i].velocity, models[i].maxVelocity);
                maxVelocity(models[this].velocity, models[this].maxVelocity);
            }

        }
        if (floor.pos[2] + floor.scale[2] / 2 > models[this].pos[2]) {
            models[this].pos[2] = floor.pos[2] + floor.scale[2] / 2;
            models[this].velocity[2] = 0.0f;
        }
    }
}

void move(uint32_t num, struct instance models[num]) {
    static clock_t prev = 0; if (prev == 0) prev = clock();
    clock_t current = clock();
    float delta = (float)(current - prev) / CLOCKS_PER_SEC;
    vec3 temp2;

    for (uint32_t i = 0; i < num; i += 1) {
        glm_vec3_scale(models[i].velocity, delta, temp2);
        glm_vec3_add(models[i].pos, temp2, models[i].pos);
    }

    prev = current;
}

void getCoordinate(vec3 centerC, struct VulkanTools vulkan) {
    double a;
    double b;
    int height = 0;
    int width = 0;
    extern mat4 s;
    glfwGetCursorPos(vulkan.window, &a, &b);

    glfwGetWindowSize(vulkan.window, &width, &height);
    vec3 centerA = { a, b, 0.0f };
    vec3 centerB = { a, b, 1.0f };
    vec4 viewPort = { 0, 0, width, height };
    glm_unproject(centerA, s, viewPort, centerA);
    glm_unproject(centerB, s, viewPort, centerB);

    glm_vec3_lerp(centerA, centerB, fabs(centerA[2]) / fabs(centerB[2] - centerA[2]), centerC);
}

int main(void) {
    srand(time(NULL));

    uint32_t num = 20;
    uint32_t instanceCount = num * num;

    struct instance instance[instanceCount];

    struct ModelBuilder builderArray[] = {
        /*floor*/ objLoader((struct ModelBuilder) {
            .instanceCount = 1,
            .texturesQuantity = 1,
            .texturesPath = (const char *[]){ "textures/grass.jpg" },
            .modelPath = "models/my_floor.obj",
            .instance = &(struct instance){
                .pos = { 0.0f, 0.0f, -50.0f },
                .rotation = { 0.0f, 0.0f, 0.0f },
                .scale = { 100.0f, 100.0f, 100.0f },
                .textureIndex = 0
            },
            .fragmentShader = "shaders/frag2.spv"
        }),
        gltfLoader((struct ModelBuilder) {
            .instanceCount = instanceCount,
            .texturesQuantity = 3,
            .texturesPath = (const char *[]){ "textures/texture.jpg", "textures/texture2.jpg", "textures/texture4.jpg" },
            .modelPath = "models/cylinder.glb",
            .instance = instance,
            .fragmentShader = "shaders/frag.spv"
        })
    };

    setPosition(builderArray[1], num);

    struct VulkanTools vulkan = setup(sizeof(builderArray) / sizeof(struct ModelBuilder), builderArray);

    glm_vec3_fill(vulkan.cameraPos, 2.0f);
    float x = 0.0f;
    float y = 40.0f;
    vec3 center = { 0.0f, 0.0f, 0.0f };
    while (!glfwWindowShouldClose(vulkan.window)) {
        glfwPollEvents();

        if (GLFW_PRESS == glfwGetMouseButton(vulkan.window, GLFW_MOUSE_BUTTON_LEFT)) {
            getCoordinate(center, vulkan);
        }

        giveAcceleration(center, instanceCount, instance);
        calculateVelocity(instanceCount, instance);
        move(instanceCount, instance);
        collision(instanceCount, instance, *builderArray[0].instance);

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
