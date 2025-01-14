#include "VulkanTools.h"

#include "model.h"
#include "cohord.h"
#include "instanceBuffer.h"

void setPosition(struct Model model, uint32_t num, struct warrior warrior[num]) {
    for (uint32_t z = 0; z < num; z += 1) {
        model.instance[z] = (struct instance) {
            .textureIndex = 3 * (z % (model.texturesQuantity / 3)),
            .textureInc = 0,
            .pos[2] = 0.5f
        };
        warrior[z].maxVelocity = 5.0f;
        warrior[z].fullCooldown = 5.0f;
        warrior[z].cooldown = 5.0f;
        warrior[z].health = 100.0f;
        warrior[z].maxHealth = 100.0f;
        warrior[z].strength = 5.0f;

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
