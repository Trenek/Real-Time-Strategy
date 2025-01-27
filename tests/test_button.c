#include "unity.h"
#include "button.h"
#include "instanceBuffer.h"
#include "model.h"
#include <stdlib.h>
#include <string.h>

// Dane testowe dla test_button.c
static struct button test_button;
static struct Model test_model;
static struct instance test_instances[3];

void setUp(void){}
void tearDown(void){}

// Lokalne funkcje pomocnicze
static void prepare_test(void) {
    test_model.instanceCount = 3;
    test_model.instance = test_instances;
    test_model.texturesQuantity = 1;

    test_button.model = &test_model;
    test_button.info = malloc(test_model.instanceCount * sizeof(struct ButtonInfo));

    for (uint16_t i = 0; i < test_model.instanceCount; i++) {
        test_button.info[i] = (struct ButtonInfo){
            .x = i * 0.1f,
            .y = i * 0.2f,
            .xPos = i,
            .yPos = i,
            .scaleX = 0.1f,
            .scaleY = 0.1f,
            .newState = i
        };
    }
}

static void cleanup_test(void) {
    free(test_button.info);
}

void test_calculateButtonPos(void) {
    prepare_test();
    struct button result = calculateButtonPos(test_button);

    TEST_ASSERT_NOT_NULL(result.info);
    TEST_ASSERT_EQUAL_INT(-1, result.chosen);

    for (uint16_t i = 0; i < result.model->instanceCount; i++) {
        TEST_ASSERT_EQUAL_FLOAT(test_button.info[i].x + test_button.info[i].xPos * test_button.info[i].scaleX, result.model->instance[i].pos[0]);
        TEST_ASSERT_EQUAL_FLOAT(test_button.info[i].y - test_button.info[i].yPos * test_button.info[i].scaleY, result.model->instance[i].pos[1]);
        TEST_ASSERT_EQUAL_FLOAT(0.1f, result.model->instance[i].scale[0]);
        TEST_ASSERT_EQUAL_FLOAT(0.1f, result.model->instance[i].scale[2]);
    }

    cleanup_test();
}

void test_checkForClick(void) {
    prepare_test();
    test_model.instance[0] = (struct instance){
        .pos = {0.0f, 0.0f, 0.0f},
        .scale = {0.5f, 1.0f, 0.5f},
        .shadow = false
    };

    struct GLFWwindow *mock_window = (struct GLFWwindow *)0x1;
    TEST_ASSERT_NOT_NULL(mock_window);

    int result = checkForClick(mock_window, &test_button);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(test_model.instance[0].shadow);

    cleanup_test();
}
