#include <time.h>
#include <string.h>

#include <cglm.h>
#include <GLFW/glfw3.h>

#include "instanceBuffer.h"
#include "VulkanTools.h"
#include "model.h"

#include "cohord.h"

void giveAcceleration(float deltaTime, struct cohort this) {
    vec2 center;
    vec2 deltaX;

    if (deltaTime != 0)
    for (uint32_t i = 0; i < this.info.instanceCount; i += 1) {
        glm_vec2_add(this.enemyToFight == NULL ? this.center : this.enemyToFight->center, this.warrior[i].pos, center);

        glm_vec2_sub(center, this.info.instance[i].pos, deltaX);
        float distance = glm_vec2_norm(deltaX);
        if (distance < this.info.instance[i].scale[0] / 2) { // vf2 = vi2 + 2ad
            glm_vec2_mul(this.warrior[i].velocity, this.warrior[i].velocity, this.warrior[i].acceleration);
            glm_vec2_scale(this.warrior[i].acceleration, -0.5f, this.warrior[i].acceleration);
            glm_vec2_div(this.warrior[i].acceleration, deltaX, this.warrior[i].acceleration);
            if (isinf(this.warrior[i].acceleration[0]) || isnan(this.warrior[i].acceleration[0])) this.warrior[i].acceleration[0] = 0.0f;
            if (isinf(this.warrior[i].acceleration[1]) || isnan(this.warrior[i].acceleration[1])) this.warrior[i].acceleration[1] = 0.0f;
        }
        else { // vf = vi + at
            vec2 vf2; {
                glm_vec2_lerp((vec2) { 0.0f, 0.0f }, deltaX, this.warrior[i].maxVelocity / distance, vf2);
            }
            glm_vec2_sub(vf2, this.warrior[i].velocity, vf2);
            glm_vec2_divs(vf2, 1, this.warrior[i].acceleration);
        }

        this.warrior[i].acceleration[2] = -9.81f;
    }
}

static void maxVelocity(vec2 v, float max) {
    float d = glm_vec2_norm(v);

    if (d > max) {
        glm_vec2_lerp((vec2) { 0.0f, 0.0f }, v, max / d, v);
    }
}

void calculateVelocity(float deltaTime, uint32_t num, struct warrior models[num]) {
    vec3 temp;

    for (uint32_t i = 0; i < num; i += 1) {
        glm_vec3_scale(models[i].acceleration, deltaTime, temp);
        glm_vec3_add(models[i].velocity, temp, models[i].velocity);
        maxVelocity(models[i].velocity, models[i].maxVelocity);
    }
}

static void calculateNewVelocity(vec2 newVelocity, struct instance a1, struct instance a2, struct warrior w1, struct warrior w2) {
    vec2 deltaV;
    vec2 deltaX;
    vec2 velocityChange;

    glm_vec2_sub(w1.velocity, w2.velocity, deltaV);
    glm_vec2_sub(a1.pos, a2.pos, deltaX);

    float dotProduct = glm_vec2_dot(deltaV, deltaX);
    float norm = glm_vec2_norm(deltaX);

    glm_vec2_scale(deltaX, dotProduct / (norm * norm), velocityChange);
    glm_vec2_sub(w1.velocity, velocityChange, newVelocity);
}

void actualCollision(struct instance *this, struct instance *that, struct warrior *thisWarrior, struct warrior *thatWarrior) {
    float minDistance = (this->scale[0] + that->scale[0]) / 2.0f;
    float distance = glm_vec2_distance(that->pos, this->pos);

    if (distance < minDistance) {
        vec2 n; {
            float ratio = 0.5;
            float responseCoef = 0.75f;
            float delta = 0.5f * responseCoef * (distance - minDistance);

            glm_vec2_sub(this->pos, that->pos, n);
            glm_vec2_scale(n, ratio * delta / distance, n);
        }
        glm_vec2_sub(this->pos, n, this->pos);
        glm_vec2_add(that->pos, n, that->pos);

        vec2 a1;
        vec2 a2;

        calculateNewVelocity(a1, *this, *that, *thisWarrior, *thatWarrior);
        calculateNewVelocity(a2, *that, *this, *thatWarrior, *thisWarrior);

        glm_vec2_copy(a1, thisWarrior->velocity);
        glm_vec2_copy(a2, thatWarrior->velocity);
        maxVelocity(thatWarrior->velocity, thatWarrior->maxVelocity);
        maxVelocity(thisWarrior->velocity, thisWarrior->maxVelocity);
    }
}

void collision(uint32_t num, struct instance models[num], struct warrior warrior[num], struct instance floor) {
    for (uint32_t this = 0; this < num; this += 1) {
        for (uint32_t that = this + 1; that < num; that += 1) {
            actualCollision(&models[this], &models[that], &warrior[this], &warrior[that]);
        }

        if (floor.pos[2] + floor.scale[1] / 2 > models[this].pos[2]) {
            models[this].pos[2] = floor.pos[2] + floor.scale[1] / 2;
            warrior[this].velocity[2] = 0.0f;
        }
    }
}

void move(float deltaTime, uint32_t num, struct instance models[num], struct warrior warrior[num]) {
    vec3 temp2;

    for (uint32_t i = 0; i < num; i += 1) {
        glm_vec3_scale(warrior[i].velocity, deltaTime, temp2);
        glm_vec3_add(models[i].pos, temp2, models[i].pos);
    }
}

void setPosition(struct Model model, uint32_t num, struct warrior warrior[num]) {
    for (uint32_t z = 0; z < num; z += 1) {
        model.instance[z] = (struct instance) {
            .textureIndex = z % model.texturesQuantity,
            .pos[2] = 0.5f
        };
        warrior[z].maxVelocity = 5.0f;

        glm_vec2_copy(warrior[z].pos, model.instance[z].pos);
        glm_vec3_fill(model.instance[z].rotation, 0.0f);
        glm_vec3_fill(model.instance[z].scale, 2.0f);
        glm_vec3_fill(warrior[z].velocity, 0.0f);
        glm_vec3_fill(warrior[z].acceleration, 0.0f);
    }
}

void getCoordinate(vec3 result, struct VulkanTools vulkan);
bool isClicked(struct VulkanTools *vulkan, struct cohort *this) {
    bool result = false;
    vec3 pos;

    getCoordinate(pos, *vulkan);

    vec2 diff;
    for (uint16_t i = 0; result == false &&  i < this->info.instanceCount; i += 1) {
        glm_vec2_sub(pos, this->info.instance[i].pos, diff);
        if (fabsf(glm_vec2_norm(diff)) < glm_vec2_norm(this->info.instance[i].scale)) {
            result = true;
        }
    }

    return result;
}

void highlightCohort(struct cohort *cohort, bool isOn) {
    for (uint16_t i = 0; i < cohort->info.instanceCount; i += 1) {
        cohort->info.instance[i].shadow = isOn;
    }
}
