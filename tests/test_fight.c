#include "unity.h"
#include "cohord.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Dane testowe
struct cohort test_cohort_fight;
struct cohort enemy_cohort_fight;

// Funkcje pomocnicze
static void prepare_test() {
    test_cohort_fight.info.instanceCount = 3;
    test_cohort_fight.info.instance = malloc(sizeof(struct instance) * test_cohort_fight.info.instanceCount);
    test_cohort_fight.warrior = malloc(sizeof(struct warrior) * test_cohort_fight.info.instanceCount);
    test_cohort_fight.enemyToFight = &enemy_cohort_fight;

    for (uint16_t i = 0; i < test_cohort_fight.info.instanceCount; i++) {
        test_cohort_fight.info.instance[i].textureInc = 0;
        test_cohort_fight.info.instance[i].pos[0] = i * 1.0f;
        test_cohort_fight.info.instance[i].pos[1] = i * 1.5f;
        test_cohort_fight.info.instance[i].pos[2] = 0.0f;

        test_cohort_fight.warrior[i].health = 100.0f - i * 20.0f;
        test_cohort_fight.warrior[i].maxHealth = 100.0f;
        test_cohort_fight.warrior[i].cooldown = 0.0f;
        test_cohort_fight.warrior[i].fullCooldown = 5.0f;
        test_cohort_fight.warrior[i].strength = 10.0f;
    }

    enemy_cohort_fight.info.instanceCount = 2;
    enemy_cohort_fight.info.instance = malloc(sizeof(struct instance) * enemy_cohort_fight.info.instanceCount);
    enemy_cohort_fight.warrior = malloc(sizeof(struct warrior) * enemy_cohort_fight.info.instanceCount);
    enemy_cohort_fight.enemyToFight = NULL;

    for (uint16_t i = 0; i < enemy_cohort_fight.info.instanceCount; i++) {
        enemy_cohort_fight.warrior[i].health = 80.0f;
        enemy_cohort_fight.warrior[i].maxHealth = 80.0f;
    }
}

static void cleanup_test() {
    free(test_cohort_fight.info.instance);
    free(test_cohort_fight.warrior);
    free(enemy_cohort_fight.info.instance);
    free(enemy_cohort_fight.warrior);
}


bool mock_isInRange([[maybe_unused]]struct cohort *cohort, [[maybe_unused]]uint16_t this,[[maybe_unused]] uint16_t that) {
    return true; // Wojownik zawsze w zasięgu
}

void mock_attack(struct cohort *cohort, uint16_t this, uint16_t that) {
    cohort->enemyToFight->warrior[that].health -= cohort->warrior[this].strength;
}

void mock_chooseDestination(struct cohort *cohort) {
    memcpy(cohort->fightPos, cohort->enemyToFight->info.instance[0].pos, sizeof(vec3));
}

// Testy
void test_attackCohord_noEnemies() {
    prepare_test();
    test_cohort_fight.enemyToFight = NULL;

    attackCohord(&test_cohort_fight, 0.016f); // deltaTime = 16ms

    TEST_ASSERT_EQUAL_INT(3, test_cohort_fight.info.instanceCount);
    TEST_ASSERT_EQUAL_INT(0, test_cohort_fight.info.instance[0].textureInc);
    TEST_ASSERT_EQUAL_INT(0, test_cohort_fight.info.instance[1].textureInc);
    TEST_ASSERT_EQUAL_INT(1, test_cohort_fight.info.instance[2].textureInc);

    cleanup_test();
}

void test_attackCohord_withEnemies() {
    prepare_test();

    test_cohort_fight.isInRange = mock_isInRange;
    test_cohort_fight.attack = mock_attack;
    test_cohort_fight.chooseDestination = mock_chooseDestination;

    attackCohord(&test_cohort_fight, 0.016f); // deltaTime = 16ms

    TEST_ASSERT_EQUAL_INT(3, test_cohort_fight.info.instanceCount);
//    printf("Enemy %u: Health=%.2f\n", 0, enemy_cohort_fight.warrior[0].health);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 50.0f, enemy_cohort_fight.warrior[0].health); //jeden wrog dostanie obrazenia
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 80.0f, enemy_cohort_fight.warrior[1].health); //drugi nie dostanie

    cleanup_test();
}

void test_attackCohord_healthUpdates() {
    prepare_test();

    attackCohord(&test_cohort_fight, 0.016f); // deltaTime = 16ms

    // Sprawdź zmiany tekstur na podstawie zdrowia
    TEST_ASSERT_EQUAL_INT(0, test_cohort_fight.info.instance[0].textureInc);
    TEST_ASSERT_EQUAL_INT(0, test_cohort_fight.info.instance[1].textureInc);
    TEST_ASSERT_EQUAL_INT(1, test_cohort_fight.info.instance[2].textureInc);

    cleanup_test();
}

void test_attackCohord_removeDeadUnits() {
    prepare_test();
    test_cohort_fight.warrior[2].health = 0.0f;

    attackCohord(&test_cohort_fight, 0.016f); // deltaTime = 16ms

    TEST_ASSERT_EQUAL_INT(2, test_cohort_fight.info.instanceCount); // Jeden wojownik został usunięty
    TEST_ASSERT_FLOAT_WITHIN(0.1f, -100.0f, test_cohort_fight.info.instance[2].pos[2]); // Martwy wojownik przesunięty poza widoczność

    cleanup_test();
}
