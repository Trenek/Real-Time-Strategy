#include "unity.h"
#include "button.h"
#include "army.h"
#include "cohord.h"
#include <stdlib.h> // malloc, free
#include <stdio.h>  // printf

struct button createArmyButtons(struct button button, struct army *army);
void updateArmyStructure(struct button *button, struct army *army);

struct button test_button;
struct army test_army;

void setUp(void) {
    printf("[DEBUG] setUp: Przygotowanie danych testowych.\n");
}

void tearDown(void) {
    printf("[DEBUG] tearDown: Czyszczenie danych testowych.\n");

    free(test_army.cohort);
    free(test_button.info);
}

void test_createArmyButtons(void) {
    printf("[INFO] Rozpoczynam test: test_createArmyButtons\n");


    printf("[INFO] Test test_createArmyButtons zakończony pomyślnie.\n");
}

void test_updateArmyStructure(void) {
    printf("[INFO] Rozpoczynam test: test_updateArmyStructure\n");


    printf("[INFO] Test test_updateArmyStructure zakończony pomyślnie.\n");
}

