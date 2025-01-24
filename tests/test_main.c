#include "unity.h"

// Deklaracje funkcji testowych z różnych plików
void test_createArmyButtons(void);
void test_updateArmyStructure(void);

int main(void) {
    UNITY_BEGIN();

    // Testy z test_game.c
    printf("[INFO] Rozpoczynam test: test_createArmyButtons\n");
    RUN_TEST(test_createArmyButtons);
    printf("[INFO] Rozpoczynam test: test_updateArmyStructure\n");
    RUN_TEST(test_updateArmyStructure);

    // Informacje końcowe o liczbie testów
    printf("\n[INFO] Wszystkie testy zostały wykonane.\n");
    printf("[INFO] Liczba testów: %llu\n", Unity.NumberOfTests);
    printf("[INFO] Liczba zakończonych błędem: %llu\n", Unity.TestFailures);
    printf("[INFO] Liczba zakończonych powodzeniem: %llu\n", Unity.NumberOfTests - Unity.TestFailures);

    return UNITY_END();
}
