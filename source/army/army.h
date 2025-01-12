struct army {
    uint16_t cohortCount;
    struct cohort *cohort;
    int16_t chosenOne;
};
struct button;

struct army setupArmy(struct army this);
void freeArmy(struct army *this);

void moveArmy(float deltaTime, struct army *this);
void armyCollision(struct army *this, struct instance floor);
void chooseCohort(struct army *army, struct button *button, struct VulkanTools *vulkan, bool isLeftMouseButtonClicked);
