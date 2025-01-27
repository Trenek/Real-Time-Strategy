#include "unity.h"
#include "deltaTime.h"
#include <stdio.h>

#include <windows.h>

void test_deltaTime() {
    struct deltaTimeManager delta = initDeltaTime();

    Sleep(1000);

    updateDeltaTime(&delta);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1, delta.deltaTime);
}
