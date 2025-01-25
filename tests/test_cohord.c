#include "unity.h"
#include "cohord.h"
#include "VulkanTools.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct cohort test_cohort;


void prepare_test_cohort_data() {
    test_cohort.info.instanceCount = 3;
    test_cohort.info.instance = malloc(sizeof(struct instance) * test_cohort.info.instanceCount);
    test_cohort.warrior = malloc(sizeof(struct warrior) * test_cohort.info.instanceCount);

    for (uint32_t i = 0; i < test_cohort.info.instanceCount; i++) {
        test_cohort.info.instance[i].pos[0] = 0.0f;
        test_cohort.info.instance[i].pos[1] = 0.0f;
        test_cohort.info.instance[i].pos[2] = 0.0f;
        test_cohort.info.instance[i].scale[0] = 1.0f;

        test_cohort.warrior[i].pos[0] = 0.0f;
        test_cohort.warrior[i].pos[1] = 0.0f;
        test_cohort.warrior[i].pos[2] = 0.0f;
    }
}

void cleanup_test_cohort_data() {
    free(test_cohort.info.instance);
    free(test_cohort.warrior);
}


void test_setPosition() {
    printf("\n[INFO] Running test: test_setPosition\n");

    prepare_test_cohort_data();
    setPosition(test_cohort.info, test_cohort.info.instanceCount, test_cohort.warrior);

    for (uint32_t i = 0; i < test_cohort.info.instanceCount; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, test_cohort.warrior[i].pos[0], test_cohort.info.instance[i].pos[0]);
        TEST_ASSERT_FLOAT_WITHIN(0.01f, test_cohort.warrior[i].pos[1], test_cohort.info.instance[i].pos[1]);
        TEST_ASSERT_EQUAL_FLOAT(1.0f, test_cohort.info.instance[i].scale[0]);
    }

    cleanup_test_cohort_data();
    printf("[INFO] Test test_setPosition completed successfully.\n");
}

void test_highlightCohort() {
    printf("\n[INFO] Running test: test_highlightCohort\n");

    prepare_test_cohort_data();

    highlightCohort(&test_cohort, true);
    for (int i = 0; i < (int)test_cohort.info.instanceCount; i++) {
        TEST_ASSERT_TRUE(test_cohort.info.instance[i].shadow);
    }

    highlightCohort(&test_cohort, false);
    for (int i = 0; i < (int)test_cohort.info.instanceCount; i++) {
        TEST_ASSERT_FALSE(test_cohort.info.instance[i].shadow);
    }

    cleanup_test_cohort_data();
    printf("[INFO] Test test_highlightCohort completed successfully.\n");
}
