#include "model.h"
#include "instanceBuffer.h"

struct warrior {
    vec2 pos;

    vec3 velocity;
    vec3 acceleration;
    float maxVelocity;

    float health;
    float maxHealth;
    float strength;
    float cooldown;
    float fullCooldown;
};

struct cohort {
    struct Model info;
    vec3 center;
    vec3 fightPos;
    int formation;

    struct warrior *warrior;
    struct cohort *enemyToFight;

    struct figherRange {
        bool (*isInRange)(struct cohort *cohort, uint16_t this, uint16_t that);
    };
    struct fighterAttack {
        void (*attack)(struct cohort *cohort, uint16_t this, uint16_t that);
    };
    struct fighterDestination {
        void (*chooseDestination)(struct cohort *cohort);
    };
};

void giveAcceleration(float deltaTime, struct cohort this);

void calculateVelocity(float deltaTime, uint32_t num, struct warrior models[num]);
void collision(uint32_t num, struct instance models[num], struct warrior warrior[num], struct instance floor);
void move(float deltaTime, uint32_t num, struct instance models[num], struct warrior warrior[num]);

void armyDisplacement(uint16_t displacementNum, uint16_t num, float scale, struct warrior warrior[num]);

void setPosition(struct Model model, uint32_t num, struct warrior warrior[num]);

void highlightCohort(struct cohort *cohort, bool isOn);
bool isClicked(struct VulkanTools *vulkan, struct cohort *this);

struct Model warriors(uint16_t instanceCount, struct GraphicsSetup *vulkan);
struct Model enemy(uint16_t instanceCount, struct GraphicsSetup *vulkan);

void attackCohord(struct cohort *cohort, float deltaTime);

struct cohort warriorMovement(struct cohort cohort);
struct cohort barbarianMovement(struct cohort cohort);
