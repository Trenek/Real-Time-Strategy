#include "model.h"
#include "instanceBuffer.h"

struct warrior {
    vec2 pos;

    vec3 velocity;
    vec3 acceleration;
    float maxVelocity;
};

struct cohort {
    struct Model info;
    vec3 center;
    int formation;

    struct warrior *warrior;
    int currentFormation;
    struct cohort *enemyToFight;
};

void giveAcceleration(float deltaTime, struct cohort this);

void calculateVelocity(float deltaTime, uint32_t num, struct warrior models[num]);
void collision(uint32_t num, struct instance models[num], struct warrior warrior[num], struct instance floor);
void move(float deltaTime, uint32_t num, struct instance models[num], struct warrior warrior[num]);

void armyDisplacement(uint16_t displacementNum, uint16_t num, float scale, struct warrior warrior[num]);

void setPosition(struct Model model, uint32_t num, struct warrior warrior[num]);

void highlightCohort(struct cohort *cohort, bool isOn);
bool isClicked(struct VulkanTools *vulkan, struct cohort *this);

struct Model warriors(uint16_t instanceCount, struct VulkanTools *vulkan);
