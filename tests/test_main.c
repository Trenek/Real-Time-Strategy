#include "unity.h"

void test_calculateButtonPos(void);
void test_checkForClick(void);

void test_setPosition(void);
void test_highlightCohort(void);

void test_armyDisplacement(void);

void test_warriorMovement(void);
void test_barbarianMovement(void);

void test_giveAcceleration(void);
void test_calculateVelocity(void);
void test_move(void);

void test_attackCohord_noEnemies(void);
void test_attackCohord_withEnemies(void);
void test_attackCohord_healthUpdates(void);
void test_attackCohord_removeDeadUnits(void);


int main(void) {
    UNITY_BEGIN();

    // test button
    RUN_TEST(test_calculateButtonPos);
    RUN_TEST(test_checkForClick);

    // test cohord
    RUN_TEST(test_setPosition);
    RUN_TEST(test_highlightCohort);

    // test displacement
    RUN_TEST(test_armyDisplacement);

    // test fightBuilder
    RUN_TEST(test_warriorMovement);
    RUN_TEST(test_barbarianMovement);

    // test movement
    RUN_TEST(test_giveAcceleration);
    RUN_TEST(test_calculateVelocity);
    RUN_TEST(test_move);

    //test fight
    RUN_TEST(test_attackCohord_noEnemies); // - fail, przeliczyć - nie wiem dlaczego jest fail dlaczego ten ostatni sie nie inicjalizuje na 0
    RUN_TEST(test_attackCohord_withEnemies); // - fail, przeliczyć
    RUN_TEST(test_attackCohord_healthUpdates); // - fail, przeliczyć
    RUN_TEST(test_attackCohord_removeDeadUnits);


    // Informacje końcowe o liczbie testów
    printf("\n[INFO] Wszystkie testy zostaly wykonane.\n");
    printf("[INFO] Liczba testow: %llu\n", Unity.NumberOfTests);
    printf("[INFO] Liczba zakonczonych bledem: %llu\n", Unity.TestFailures);
    printf("[INFO] Liczba zakonczonych powodzeniem: %llu\n", Unity.NumberOfTests - Unity.TestFailures);

    return UNITY_END();
}
