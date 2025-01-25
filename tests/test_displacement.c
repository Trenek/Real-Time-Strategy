#include "unity.h"
#include "cohord.h"
#include <stdlib.h>
#include <stdio.h>

struct warrior test_warrior[10];

void prepare_test_warriors(uint16_t num) {
    for (uint16_t i = 0; i < num; i++) {
        glm_vec2_zero(test_warrior[i].pos);
    }
}

void test_armyDisplacement() {
    printf("\n[INFO] Rozpoczynam test: test_armyDisplacement\n");

    uint16_t displacementNum = 0;
    uint16_t num = 10;
    float scale = 2.0f;

    prepare_test_warriors(num);

    armyDisplacement(displacementNum, num, scale, test_warrior);

    for (uint16_t i = 0; i < num; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.1f, 0.0f, test_warrior[i].pos[0]);
        TEST_ASSERT_FLOAT_WITHIN(0.1f, 0.0f, test_warrior[i].pos[1]);
    }

    printf("[INFO] Test test_armyDisplacement zakonczony pomyslnie.\n");
}
