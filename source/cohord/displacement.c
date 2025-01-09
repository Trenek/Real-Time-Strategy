#include <cglm.h>

static void point(uint16_t num, vec2 actualPos[num]) {
    uint16_t squareNum = sqrt(num);
    vec2 (*pos)[squareNum] = (vec2 (*)[squareNum])actualPos;

    for (uint32_t i = 0; i < squareNum; i += 1) {
        for (uint32_t j = 0; j < squareNum; j += 1) {
            pos[i][j][0] = 0.0f;
            pos[i][j][1] = 0.0f;
        }
    }
}


static void square(uint16_t num, vec2 actualPos[num]) {
    uint16_t squareNum = sqrt(num);
    vec2 (*pos)[squareNum] = (vec2 (*)[squareNum])actualPos;

    for (uint32_t i = 0; i < squareNum; i += 1) {
        for (uint32_t j = 0; j < squareNum; j += 1) {
            pos[i][j][0] = 0.0f - i;
            pos[i][j][1] = 0.0f - j;
        }
    }
}

static void squareFromMiddle(uint16_t num, vec2 actualPos[num]) {
    int32_t p = 0;

    int32_t j = 0;
    int32_t k = 0;

    for (uint32_t i = 0; i < num; i += 1) {
        actualPos[i][0] = j;
        actualPos[i][1] = k;

        if (p == k && p == j) {
            k = p += 1;
        }
        else if (k == p && j != -p) {
            j -= 1;
        }
        else if (k != -p && j == -p) {
            k -= 1;
        }
        else if (k == -p && j != p) {
            j += 1;
        }
        else if (k != p && j == p) {
            k += 1;
        }
    }
}

static void circle(uint16_t num, vec2 actualPos[num]) {
    float pi2 = M_PI * 2;
    int32_t r = 0;
    int32_t rQuantity = 1;
    int32_t j = 0;

    for (uint32_t i = 0; i < num; i += 1) {
        actualPos[i][0] = r;
        actualPos[i][1] = r;
        glm_vec2_rotate(actualPos[i], glm_rad((float)(360 * j) / rQuantity), actualPos[i]);

        if ((j += 1) >= rQuantity) {
            r += 1;
            rQuantity = r * pi2;
            j = 0;
        }
    }
}

void armyDisplacement(uint16_t displacementNum, uint16_t num, vec2 pos[num]) {
    void (*displacement[])(uint16_t, vec2[num]) = {
        point,
        square,
        squareFromMiddle,
        circle
    };

    displacement[displacementNum](num, pos);
}
