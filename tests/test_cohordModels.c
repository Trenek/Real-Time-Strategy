#include "unity.h"
#include "cohord.h"
//#include "model.h"
#include "VulkanTools.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct GraphicsSetup mock_graphics;
struct Model test_model;

void prepare_graphics_setup() {
    memset(&mock_graphics, 0, sizeof(mock_graphics));
}

void cleanup_test_model(struct Model *model) {
    if (model->instance != NULL) {
        free(model->instance);
        model->instance = NULL;
    }
}

void test_warriors() {
    printf("\n[INFO] Running test: test_warriors\n");

    prepare_graphics_setup();
    uint16_t instance_count = 10;

    struct Model model = warriors(instance_count, &mock_graphics);

    // Walidacja wyniku
    TEST_ASSERT_NOT_NULL(model.instance);
    TEST_ASSERT_EQUAL_UINT(instance_count, model.instanceCount);

    for (uint32_t i = 0; i < instance_count; i++) {
        TEST_ASSERT_NOT_NULL(&model.instance[i]);
    }

    cleanup_test_model(&model);
    printf("[INFO] Test test_warriors completed successfully.\n");
}

void test_enemy() {
    printf("\n[INFO] Running test: test_enemy\n");

    prepare_graphics_setup();
    uint16_t instance_count = 5;

    struct Model model = enemy(instance_count, &mock_graphics);

    // Walidacja wyniku
    TEST_ASSERT_NOT_NULL(model.instance);
    TEST_ASSERT_EQUAL_UINT(instance_count, model.instanceCount);

    for (uint32_t i = 0; i < instance_count; i++) {
        TEST_ASSERT_NOT_NULL(&model.instance[i]);
    }

    cleanup_test_model(&model);
    printf("[INFO] Test test_enemy completed successfully.\n");
}

