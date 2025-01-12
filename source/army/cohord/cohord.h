#include "model.h"
#include "instanceBuffer.h"

struct cohort {
    struct Model info;
    vec3 center;
    int formation;

    vec2 *warriorPos;
    struct {
        vec2 pos;
    } *warrior;
    int currentFormation;
};

void giveAcceleration(float deltaTime, struct cohort this);

void calculateVelocity(float deltaTime, uint32_t num, struct instance models[num]);
void collision(uint32_t num, struct instance models[num], struct instance floor);
void move(float deltaTime, uint32_t num, struct instance models[num]);

void armyDisplacement(uint16_t displacementNum, uint16_t num, float scale, vec2 pos[num]);

void setPosition(struct Model model, uint32_t num, vec2 pos[num]);

void highlightCohort(struct cohort *cohort, bool isOn);
bool isClicked(struct VulkanTools *vulkan, struct cohort *this);

struct Model warriors(uint16_t instanceCount, struct VulkanTools *vulkan);
