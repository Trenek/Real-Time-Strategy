#include "unity.h"
#include "cohord.h"
#include "VulkanTools.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct cohort test_cohort;

static void prepare_test() {
    test_cohort.info.instanceCount = 3;
    test_cohort.info.texturesQuantity = 9;
    test_cohort.info.instance = malloc(sizeof(struct instance) * test_cohort.info.instanceCount);
    test_cohort.warrior = malloc(sizeof(struct warrior) * test_cohort.info.instanceCount);

    for (uint32_t i = 0; i < test_cohort.info.instanceCount; i++) {
        test_cohort.info.instance[i].pos[0] = i * 1.0f;
        test_cohort.info.instance[i].pos[1] = i * 1.5f;
        test_cohort.info.instance[i].pos[2] = 0.0f;
        test_cohort.info.instance[i].scale[0] = 1.0f;

        test_cohort.warrior[i].pos[0] = i * 1.0f;
        test_cohort.warrior[i].pos[1] = i * 1.5f;
        test_cohort.warrior[i].pos[2] = 0.0f;
    }
}

static void cleanup_test() {
    free(test_cohort.info.instance);
    free(test_cohort.warrior);
}

void test_setPosition() {
    prepare_test();

    // Wywołanie funkcji do przetestowania
    setPosition(test_cohort.info, test_cohort.info.instanceCount, test_cohort.warrior);

    for (uint32_t i = 0; i < test_cohort.info.instanceCount; i++) {
        // Porównanie pozycji wojowników i modeli
        TEST_ASSERT_FLOAT_WITHIN(0.01f, test_cohort.warrior[i].pos[0], test_cohort.info.instance[i].pos[0]);
        TEST_ASSERT_FLOAT_WITHIN(0.01f, test_cohort.warrior[i].pos[1], test_cohort.info.instance[i].pos[1]);
        TEST_ASSERT_EQUAL_FLOAT(2.0f, test_cohort.info.instance[i].scale[0]);
    }

    cleanup_test();
}

void test_highlightCohort() {
    prepare_test();

    highlightCohort(&test_cohort, true);
    for (int i = 0; i < (int)test_cohort.info.instanceCount; i++) {
        TEST_ASSERT_TRUE(test_cohort.info.instance[i].shadow);
    }

    highlightCohort(&test_cohort, false);
    for (int i = 0; i < (int)test_cohort.info.instanceCount; i++) {
        TEST_ASSERT_FALSE(test_cohort.info.instance[i].shadow);
    }

    cleanup_test();
}
