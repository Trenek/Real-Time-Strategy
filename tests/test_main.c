#include "unity.h"

void test_calculateButtonPos(void);
void test_checkForClick(void);

void test_setPosition(void);
void test_highlightCohort(void);

void test_warriors(void);
void test_enemy(void);

void test_armyDisplacement(void);

void test_warriorMovement(void);
void test_barbarianMovement(void);

void test_giveAcceleration(void);
void test_calculateVelocity(void);
void test_move(void);


int main(void) {
    UNITY_BEGIN();

    // test button
    RUN_TEST(test_calculateButtonPos);
    RUN_TEST(test_checkForClick);

    // test cohord
//    RUN_TEST(test_setPosition); - nie uruchamia się narazie, ale nie wiem czemu
    RUN_TEST(test_highlightCohort);

    // test cohordModels
//    RUN_TEST(test_warriors); - nie uruchamia się narazie, ale nie wiem czemu
//    RUN_TEST(test_enemy); - nie uruchamia się narazie, ale nie wiem czemu

    // test displacement
    RUN_TEST(test_armyDisplacement); // - fail, przeliczyć

    // test fightBuilder
    RUN_TEST(test_warriorMovement);
    RUN_TEST(test_barbarianMovement);  // - fail, przeliczyć

    // test movement
    RUN_TEST(test_giveAcceleration); // - fail, przeliczyć
    RUN_TEST(test_calculateVelocity);
    RUN_TEST(test_move);



    // Informacje końcowe o liczbie testów
    printf("\n[INFO] Wszystkie testy zostały wykonane.\n");
    printf("[INFO] Liczba testów: %llu\n", Unity.NumberOfTests);
    printf("[INFO] Liczba zakończonych błędem: %llu\n", Unity.TestFailures);
    printf("[INFO] Liczba zakończonych powodzeniem: %llu\n", Unity.NumberOfTests - Unity.TestFailures);

    return UNITY_END();
}
