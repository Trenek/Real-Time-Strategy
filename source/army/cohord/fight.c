#include <string.h>

#include "cohord.h"

static int findAttackable(struct cohort *cohort, uint16_t this) {
    int result = -1;

    for (uint16_t i = 0; result == -1 && i < cohort->enemyToFight->info.instanceCount; i += 1) {
        result = cohort->isInRange(cohort, this, i) ? i : -1;
    }

    return result;
}

[[maybe_unused]]
static void swap(void *a, void *b, size_t size) {
    uint8_t temp[size];
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

static void swap2(void *actualMem, size_t sizeA, size_t sizeB) {
    uint8_t temp[sizeA];
    uint8_t *mem = actualMem;
    memcpy(temp, mem, sizeA);
    memcpy(mem, mem + sizeA, sizeB);
    memcpy(mem + sizeB, temp, sizeA);
}

void attackCohord(struct cohort *cohort, float deltaTime) {
    for (uint16_t i = 0; i < cohort->info.instanceCount; i += 1) {
        if (cohort->warrior[i].health / cohort->warrior[i].maxHealth > 0.66f) {
            cohort->info.instance[i].textureInc = 0;
        }
        else if (cohort->warrior[i].health / cohort->warrior[i].maxHealth > 0.33f) {
            cohort->info.instance[i].textureInc = 1;
        }
        else if (cohort->warrior[i].health / cohort->warrior[i].maxHealth > 0.00f) {
            cohort->info.instance[i].textureInc = 2;
        }
        else if (cohort->warrior[i].health <= 0.0f) {
            swap2(cohort->warrior + i, sizeof(struct warrior), sizeof(struct warrior) * (cohort->info.instanceCount - 1 - i));
            swap2(cohort->info.instance + i, sizeof(struct instance), sizeof(struct instance) * (cohort->info.instanceCount - 1 - i));
            cohort->info.instance[cohort->info.instanceCount - 1].pos[2] -= 100.0f;

            cohort->info.instanceCount -= 1;
            i -= 1;
        }
    }

    if (cohort->info.instanceCount != 0) {
        if (cohort->enemyToFight != NULL) {
            if (cohort->enemyToFight->enemyToFight == NULL) {
                cohort->enemyToFight->enemyToFight = cohort;
            }
            for (uint16_t i = 0; i < cohort->info.instanceCount; i += 1) {
                cohort->warrior[i].cooldown -= deltaTime;
                int attackable = findAttackable(cohort, i);
                if (cohort->warrior[i].cooldown <= 0.0f && attackable != -1) {
                    cohort->attack(cohort, i, attackable);

                    cohort->warrior[i].cooldown = cohort->warrior[i].fullCooldown;
                }
            }

            cohort->chooseDestination(cohort);
            if (cohort->enemyToFight->info.instanceCount == 0) {
                if (cohort->enemyToFight->enemyToFight == cohort) {
                    cohort->enemyToFight->enemyToFight = NULL;
                }
                cohort->enemyToFight = NULL;
            }
            memcpy(cohort->center, cohort->info.instance[0].pos, sizeof(vec3));
        }
    }
    else {
        if (cohort->enemyToFight->enemyToFight == cohort) {
            cohort->enemyToFight->enemyToFight = NULL;
        }
        cohort->enemyToFight = NULL;
        memcpy(cohort->center, cohort->info.instance[0].pos, sizeof(vec3));
    }
}
