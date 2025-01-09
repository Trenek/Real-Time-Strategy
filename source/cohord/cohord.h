struct warrior {
    vec2 pos;
};

struct cohort {
    struct Model info;
    vec3 center;
    int formation;

    vec2 *warriorPos;
    int currentFormation;
};

void giveAcceleration(struct cohort this);

void calculateVelocity(uint32_t num, struct instance models[num]);
void collision(uint32_t num, struct instance models[num], struct instance floor);
void move(uint32_t num, struct instance models[num]);

void armyDisplacement(uint16_t displacementNum, uint16_t num, vec2 pos[num]);

void setPosition(struct Model model, uint32_t num, vec2 pos[num]);
