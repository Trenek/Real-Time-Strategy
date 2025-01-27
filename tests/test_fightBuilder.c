#include "unity.h"
#include "cohord.h"
#include <stdlib.h>
#include <stdio.h>
#include <cglm.h>

struct cohort test_cohort_fightBuilder;
struct cohort enemy_cohort;

static void prepare_test() {
    // Tworzenie testowej kohorty
    test_cohort_fightBuilder.info.instanceCount = 3;
    test_cohort_fightBuilder.info.instance = malloc(sizeof(struct instance) * 3);
    test_cohort_fightBuilder.warrior = malloc(sizeof(struct warrior) * 3);

    // Tworzenie wrogiej kohorty
    enemy_cohort.info.instanceCount = 3;
    enemy_cohort.info.instance = malloc(sizeof(struct instance) * 3);
    enemy_cohort.warrior = malloc(sizeof(struct warrior) * 3);

    // Ustawianie początkowych pozycji i statystyk
    for (int i = 0; i < 3; i++) {
        test_cohort_fightBuilder.info.instance[i].pos[0] = i * 2.0f;
        test_cohort_fightBuilder.info.instance[i].pos[1] = 0.0f;
        test_cohort_fightBuilder.info.instance[i].scale[0] = 1.0f;

        test_cohort_fightBuilder.warrior[i].strength = 10.0f;
        test_cohort_fightBuilder.warrior[i].health = 100.0f;

        enemy_cohort.info.instance[i].pos[0] = i * 2.0f;
        enemy_cohort.info.instance[i].pos[1] = 0.0f;
        enemy_cohort.info.instance[i].scale[0] = 1.0f;

        enemy_cohort.warrior[i].health = 100.0f;
    }

    test_cohort_fightBuilder.enemyToFight = &enemy_cohort;
    enemy_cohort.enemyToFight = &test_cohort_fightBuilder;
}

static void cleanup_test() {
    free(test_cohort_fightBuilder.info.instance);
    free(test_cohort_fightBuilder.warrior);
    free(enemy_cohort.info.instance);
    free(enemy_cohort.warrior);
}

void test_warriorMovement() {
    prepare_test();
    test_cohort_fightBuilder = warriorMovement(test_cohort_fightBuilder);

    // Sprawdzamy, czy funkcje wskaźnikowe zostały przypisane
    TEST_ASSERT_NOT_NULL(test_cohort_fightBuilder.isInRange);
    TEST_ASSERT_NOT_NULL(test_cohort_fightBuilder.attack);
    TEST_ASSERT_NOT_NULL(test_cohort_fightBuilder.chooseDestination);

    // Sprawdzamy zasięg
    TEST_ASSERT_TRUE(test_cohort_fightBuilder.isInRange(&test_cohort_fightBuilder, 0, 0));  // Powinno być w zasięgu
    TEST_ASSERT_FALSE(test_cohort_fightBuilder.isInRange(&test_cohort_fightBuilder, 0, 2)); // Poza zasięgiem

    // Sprawdzamy atak
    test_cohort_fightBuilder.attack(&test_cohort_fightBuilder, 0, 0); // Wojownik 0 atakuje wojownika 0 wroga
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 90.0f, enemy_cohort.warrior[0].health); // Zdrowie przeciwnika

    // Sprawdzamy wybór celu
    test_cohort_fightBuilder.chooseDestination(&test_cohort_fightBuilder);
    TEST_ASSERT_EQUAL_FLOAT(enemy_cohort.info.instance[0].pos[0], test_cohort_fightBuilder.fightPos[0]);
    TEST_ASSERT_EQUAL_FLOAT(enemy_cohort.info.instance[0].pos[1], test_cohort_fightBuilder.fightPos[1]);

    cleanup_test();
}

void test_barbarianMovement() {
    prepare_test();
    test_cohort_fightBuilder = barbarianMovement(test_cohort_fightBuilder);

    // Sprawdzamy, czy funkcje wskaźnikowe zostały przypisane
    TEST_ASSERT_NOT_NULL(test_cohort_fightBuilder.isInRange);
    TEST_ASSERT_NOT_NULL(test_cohort_fightBuilder.attack);
    TEST_ASSERT_NOT_NULL(test_cohort_fightBuilder.chooseDestination);

    // Sprawdzamy zasięg
    TEST_ASSERT_TRUE(test_cohort_fightBuilder.isInRange(&test_cohort_fightBuilder, 0, 0));  // Powinno być w zasięgu
    TEST_ASSERT_FALSE(test_cohort_fightBuilder.isInRange(&test_cohort_fightBuilder, 0, 2)); // Poza zasięgiem

    // Sprawdzamy atak
    test_cohort_fightBuilder.attack(&test_cohort_fightBuilder, 0, 0); // Wojownik 0 atakuje wojownika 0 wroga
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 70.0f, enemy_cohort.warrior[0].health); // Zdrowie przeciwnika
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 100.0f, test_cohort_fightBuilder.warrior[0].health);  // Zdrowie atakującego

    // Sprawdzamy wybór celu
    test_cohort_fightBuilder.chooseDestination(&test_cohort_fightBuilder);
    TEST_ASSERT_EQUAL_FLOAT(enemy_cohort.info.instance[0].pos[0], test_cohort_fightBuilder.fightPos[0]);
    TEST_ASSERT_EQUAL_FLOAT(enemy_cohort.info.instance[0].pos[1], test_cohort_fightBuilder.fightPos[1]);

    cleanup_test();
}

