#include <stdint.h>
#include <string.h>
#include <cglm.h>

#include "VulkanTools.h"

#include "cohord.h"
#include "army.h"
#include "button.h"

struct army setupArmy(struct army this) {
    this.chosenOne = -1;

    for (uint16_t i = 0; i < this.cohortCount; i += 1) {
        this.cohort[i].warrior = malloc(sizeof(struct warrior) * this.cohort[i].info.instanceCount);
        armyDisplacement(this.cohort[i].formation, this.cohort[i].info.instanceCount, 2.0f, this.cohort[i].warrior);
        setPosition(this.cohort[i].info, this.cohort[i].info.instanceCount, this.cohort[i].warrior);
        for (uint16_t j = 0; j < this.cohort[i].info.instanceCount; j += 1) {
            glm_vec2_add(this.cohort[i].info.instance[j].pos, this.cohort[i].center, this.cohort[i].info.instance[j].pos);
        }
    }

    return this;
}

void freeArmy(struct army *this) {
    for (uint16_t i = 0; i < this->cohortCount; i += 1) {
        free(this->cohort[i].warrior);
    }
}

void moveArmy(float deltaTime, struct army *this) {
    for (uint16_t i = 0; i < this->cohortCount; i += 1) {
        giveAcceleration(deltaTime, this->cohort[i]);

        calculateVelocity(deltaTime, this->cohort[i].info.instanceCount, this->cohort[i].warrior);
        move(deltaTime, this->cohort[i].info.instanceCount, this->cohort[i].info.instance, this->cohort[i].warrior);

        armyDisplacement(this->cohort[i].formation, this->cohort[i].info.instanceCount, this->cohort[i].info.instance->scale[0], this->cohort[i].warrior);
    }
}

static uint16_t getWarriorsCount(struct army *this) {
    uint16_t result = 0;

    for (uint32_t i = 0; i < this->cohortCount; i += 1) {
        result += this->cohort[i].info.instanceCount;
    }

    return result;
}

static void copyWarriors(struct instance *warriors, struct warrior *warriorData, struct army *this) {
    for (uint16_t i = 0; i < this->cohortCount; i += 1) {
        memcpy(warriors, this->cohort[i].info.instance, this->cohort[i].info.instanceCount * sizeof(struct instance));
        memcpy(warriorData, this->cohort[i].warrior, this->cohort[i].info.instanceCount * sizeof(struct warrior));

        warriors += this->cohort[i].info.instanceCount;
        warriorData += this->cohort[i].info.instanceCount;
    }
}

static void copyBackWarriors(struct instance *warriors, struct warrior *warriorData, struct army *this) {
    for (uint16_t i = 0; i < this->cohortCount; i += 1) {
        memcpy(this->cohort[i].info.instance, warriors, this->cohort[i].info.instanceCount * sizeof(struct instance));
        memcpy(this->cohort[i].warrior, warriorData, this->cohort[i].info.instanceCount * sizeof(struct warrior));

        warriors += this->cohort[i].info.instanceCount;
        warriorData += this->cohort[i].info.instanceCount;
    }
}

void armyCollision(uint16_t armyQuantity, struct army *this[static armyQuantity], struct instance floor) {
    uint16_t warriorCount[armyQuantity];
    uint16_t warriorSum = 0;
    uint16_t a = 0;

    for (uint16_t i = 0; i < armyQuantity; i += 1) {
        warriorCount[i] = getWarriorsCount(this[i]);
        warriorSum += warriorCount[i];
    }

    struct instance warriors[warriorSum];
    struct warrior warriorsData[warriorSum];

    for (uint16_t i = 0; i < armyQuantity; i += 1) {
        copyWarriors(warriors + a, warriorsData + a, this[i]);
        a += warriorCount[i];
    }

    collision(warriorSum, warriors, warriorsData, floor);

    a = 0;
    for (uint16_t i = 0; i < armyQuantity; i += 1) {
        copyBackWarriors(warriors + a, warriorsData + a, this[i]);
        a += warriorCount[i];
    }
}

void chooseCohort(struct army *army, struct button *button, struct VulkanTools *vulkan, bool isLeftMouseButtonClicked) {
    int click = -1;

    for (uint16_t i = 0; i < army->cohortCount; i += 1) {
        highlightCohort(&army->cohort[i], false);
    }

    click = checkForClick(vulkan->window, button);

    if (button->chosen != -1 && army->chosenOne != button->chosen) {
        highlightCohort(&army->cohort[button->chosen], true);
        click = button->chosen;
    }
    else for (uint16_t i = 0; click == -1 && i < army->cohortCount; i += 1) {
        if (isClicked(vulkan, &army->cohort[i])) {
            highlightCohort(&army->cohort[i], true);
            click = i;
        }
    }
    if (isLeftMouseButtonClicked) {
        army->chosenOne = click;
    }

    if (army->chosenOne != -1) {
        highlightCohort(&army->cohort[army->chosenOne], true);
    }
}
