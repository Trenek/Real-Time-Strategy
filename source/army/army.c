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
        this.cohort[i].warriorPos = malloc(sizeof(vec2) * this.cohort[i].info.instanceCount);
        armyDisplacement(this.cohort[i].formation, this.cohort[i].info.instanceCount, 2.0f, this.cohort[i].warriorPos);
        setPosition(this.cohort[i].info, this.cohort[i].info.instanceCount, this.cohort[i].warriorPos);
        for (uint16_t j = 0; j < this.cohort[i].info.instanceCount; j += 1) {
            glm_vec2_add(this.cohort[i].info.instance[j].pos, this.cohort[i].center, this.cohort[i].info.instance[j].pos);
        }
    }

    return this;
}

void freeArmy(struct army *this) {
    for (uint16_t i = 0; i < this->cohortCount; i += 1) {
        free(this->cohort[i].warriorPos);
    }
}

void moveArmy(float deltaTime, struct army *this) {
    for (uint16_t i = 0; i < this->cohortCount; i += 1) {
        giveAcceleration(deltaTime, this->cohort[i]);

        calculateVelocity(deltaTime, this->cohort[i].info.instanceCount, this->cohort[i].info.instance);
        move(deltaTime, this->cohort[i].info.instanceCount, this->cohort[i].info.instance);

        armyDisplacement(this->cohort[i].formation, this->cohort[i].info.instanceCount, this->cohort[i].info.instance->scale[0], this->cohort[i].warriorPos);
    }
}

static uint16_t getWarriorsCount(struct army *this) {
    uint16_t result = 0;

    for (uint32_t i = 0; i < this->cohortCount; i += 1) {
        result += this->cohort[i].info.instanceCount;
    }

    return result;
}

static void copyWarriors(struct instance *warriors, struct army *this) {
    for (uint16_t i = 0; i < this->cohortCount; i += 1) {
        memcpy(warriors, this->cohort[i].info.instance, this->cohort[i].info.instanceCount * sizeof(struct instance));

        warriors += this->cohort[i].info.instanceCount;
    }
}

static void copyBackWarriors(struct instance *warriors, struct army *this) {
    for (uint16_t i = 0; i < this->cohortCount; i += 1) {
        memcpy(this->cohort[i].info.instance, warriors, this->cohort[i].info.instanceCount * sizeof(struct instance));

        warriors += this->cohort[i].info.instanceCount;
    }
}

void armyCollision(struct army *this, struct instance floor) {
    uint16_t warriorCount = getWarriorsCount(this);
    struct instance warriors[warriorCount];

    copyWarriors(warriors, this);

    collision(warriorCount, warriors, floor);

    copyBackWarriors(warriors, this);
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
