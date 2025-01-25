#include "unity.h"
#include "cohord.h"
#include <stdlib.h>
#include <cglm.h>

struct cohort test_cohort_movement;

void prepare_test_cohort_data_movement() {
    // Przygotowanie danych kohorty
    test_cohort_movement.info.instanceCount = 3;
    test_cohort_movement.info.instance = malloc(sizeof(struct instance) * 3);
    test_cohort_movement.warrior = malloc(sizeof(struct warrior) * 3);

    for (int i = 0; i < 3; i++) {
        // Pozycje startowe
        glm_vec3_zero(test_cohort_movement.info.instance[i].pos);
        glm_vec3_zero(test_cohort_movement.warrior[i].velocity);
        glm_vec3_zero(test_cohort_movement.warrior[i].acceleration);

        test_cohort_movement.warrior[i].maxVelocity = 5.0f;
    }
}

void cleanup_test_cohort_data_movement() {
    free(test_cohort_movement.info.instance);
    free(test_cohort_movement.warrior);
}

void test_giveAcceleration() {
    printf("\n[INFO] Rozpoczynam test: test_giveAcceleration\n");

    prepare_test_cohort_data_movement();

    // Dodanie przyspieszenia do wojowników
    for (int i = 0; i < 3; i++) {
        test_cohort_movement.warrior[i].acceleration[0] = 1.0f; // Przyspieszenie w osi X
    }

    giveAcceleration(0.0f, test_cohort_movement); // Delta time = 1s

    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, test_cohort_movement.warrior[i].acceleration[0]);
    }

    cleanup_test_cohort_data_movement();
    printf("[INFO] Test test_giveAcceleration zakonczony pomyslnie.\n");
}

void test_calculateVelocity() {
    printf("\n[INFO] Rozpoczynam test: test_calculateVelocity\n");

    prepare_test_cohort_data_movement();

    // Dodanie przyspieszenia
    for (int i = 0; i < 3; i++) {
        test_cohort_movement.warrior[i].acceleration[0] = 2.0f; // Przyspieszenie w osi X
    }

    calculateVelocity(0.5f, 3, test_cohort_movement.warrior); // Delta time = 0.5s

    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, test_cohort_movement.warrior[i].velocity[0]); // V = a * t
    }

    cleanup_test_cohort_data_movement();
    printf("[INFO] Test test_calculateVelocity zakonczony pomyslnie.\n");
}

void test_move() {
    printf("\n[INFO] Rozpoczynam test: test_move\n");

    prepare_test_cohort_data_movement();

    // Ustawienie początkowej prędkości
    for (int i = 0; i < 3; i++) {
        test_cohort_movement.warrior[i].velocity[0] = 2.0f; // Ruch w osi X
    }

    move(1.0f, 3, test_cohort_movement.info.instance, test_cohort_movement.warrior); // Delta time = 1s

    for (int i = 0; i < 3; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 2.0f, test_cohort_movement.info.instance[i].pos[0]); // S = v * t
    }

    cleanup_test_cohort_data_movement();
    printf("[INFO] Test test_move zakonczony pomyslnie.\n");
}

