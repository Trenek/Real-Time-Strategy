#include "cohord.h"

static bool warriorRange(struct cohort *cohort, uint16_t this, uint16_t that) {
    vec2 deltaX;

    glm_vec2_sub(cohort->info.instance[this].pos, cohort->enemyToFight->info.instance[that].pos, deltaX);

    return glm_vec2_norm(deltaX) <= 2 * cohort->info.instance[this].scale[0];
}

static void warriorAttack(struct cohort *cohort, uint16_t this, uint16_t that) {
    cohort->enemyToFight->warrior[that].health -= cohort->warrior[this].strength;
}

static void warriorDestination(struct cohort *cohort) {
    glm_vec3_dup(cohort->enemyToFight->info.instance[0].pos, cohort->fightPos);
}

struct cohort warriorMovement(struct cohort cohort) {
    cohort.isInRange = warriorRange;
    cohort.attack = warriorAttack;
    cohort.chooseDestination = warriorDestination;

    return cohort;
}

static void barbarianAttack(struct cohort *cohort, uint16_t this, uint16_t that) {
    cohort->enemyToFight->warrior[that].health -= 2 * cohort->warrior[this].strength;
    cohort->enemyToFight->warrior[this].health -= cohort->warrior[this].strength;
}

struct cohort barbarianMovement(struct cohort cohort) {
    cohort.isInRange = warriorRange;
    cohort.attack = barbarianAttack;
    cohort.chooseDestination = warriorDestination;

    return cohort;
}
