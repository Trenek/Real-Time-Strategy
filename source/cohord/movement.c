#include <time.h>

#include <cglm.h>

#include "instanceBuffer.h"
#include "model.h"

#include "cohord.h"

void giveAcceleration(struct cohort this) {
    vec2 center;
    vec2 deltaX;

    static clock_t prev = 0; if (prev == 0) prev = clock();
    clock_t now = clock();
    float diff = (float)(now - prev) / CLOCKS_PER_SEC;

    if (diff != 0)
    for (uint32_t i = 0; i < this.info.instanceCount; i += 1) {
        glm_vec2_add(this.center, this.warriorPos[i], center);

        glm_vec2_sub(center, this.info.instance[i].pos, deltaX);
        float distance = glm_vec2_norm(deltaX);
        if (distance < this.info.instance[i].scale[0] / 2) { // vf2 = vi2 + 2ad
            glm_vec2_mul(this.info.instance[i].velocity, this.info.instance[i].velocity, this.info.instance[i].acceleration);
            glm_vec2_scale(this.info.instance[i].acceleration, -0.5f, this.info.instance[i].acceleration);
            glm_vec2_div(this.info.instance[i].acceleration, deltaX, this.info.instance[i].acceleration);
            if (isinf(this.info.instance[i].acceleration[0]) || isnan(this.info.instance[i].acceleration[0])) this.info.instance[i].acceleration[0] = 0.0f;
            if (isinf(this.info.instance[i].acceleration[1]) || isnan(this.info.instance[i].acceleration[1])) this.info.instance[i].acceleration[1] = 0.0f;
        }
        else { // vf = vi + at
            vec2 vf2; {
                glm_vec2_lerp((vec2) { 0.0f, 0.0f }, deltaX, this.info.instance[i].maxVelocity / distance, vf2);
            }
            glm_vec2_sub(vf2, this.info.instance[i].velocity, vf2);
            glm_vec2_divs(vf2, 1, this.info.instance[i].acceleration);
        }

        this.info.instance[i].acceleration[2] = -9.81f;
    }

    prev = now;
}

static void maxVelocity(vec2 v, float max) {
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

static void calculateNewVelocity(vec2 newVelocity, struct instance a1, struct instance a2) {
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
                    float responseCoef = 0.75f;
                    float delta = 0.5f * responseCoef * (distance - minDistance);

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

void setPosition(struct Model model, uint32_t num, vec2 pos[num]) {
    for (uint32_t z = 0; z < num; z += 1) {
        model.instance[z] = (struct instance) {
            .textureIndex = z % model.texturesQuantity,
            .pos[2] = 0.5f,
            .maxVelocity = 10e-1
        };

        glm_vec2_copy(pos[z], model.instance[z].pos);
        glm_vec3_fill(model.instance[z].rotation, 0.0f);
        glm_vec3_fill(model.instance[z].scale, 0.5f);
        glm_vec3_fill(model.instance[z].velocity, 0.0f);
        glm_vec3_fill(model.instance[z].acceleration, 0.0f);
    }
}
