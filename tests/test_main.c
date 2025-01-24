#include "unity.h"

void test_createArmyButtons(void);
void test_updateArmyStructure(void);
void test_calculateButtonPos(void);
void test_checkForClick(void);

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_createArmyButtons);
    RUN_TEST(test_updateArmyStructure);
    RUN_TEST(test_calculateButtonPos);
    RUN_TEST(test_checkForClick);

    // Informacje końcowe o liczbie testów
    printf("\n[INFO] Wszystkie testy zostały wykonane.\n");
    printf("[INFO] Liczba testów: %llu\n", Unity.NumberOfTests);
    printf("[INFO] Liczba zakończonych błędem: %llu\n", Unity.TestFailures);
    printf("[INFO] Liczba zakończonych powodzeniem: %llu\n", Unity.NumberOfTests - Unity.TestFailures);

    return UNITY_END();
}
