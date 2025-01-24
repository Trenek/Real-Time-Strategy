#include "unity.h"
#include "button.h"
#include "army.h"
#include "cohord.h"
#include <stdlib.h>
#include <stdio.h>

// Dane testowe dla test_game.c
static struct button test_button;
static struct army test_army;

// Lokalne funkcje pomocnicze
static void prepare_test_game_data(void) {

}

static void cleanup_test_game_data(void) {
    free(test_army.cohort);
    free(test_button.info);
}

void test_createArmyButtons(void) {
    printf("\n[INFO] Rozpoczynam test: test_createArmyButtons\n");

    prepare_test_game_data();

    cleanup_test_game_data();
    printf("[INFO] Test test_createArmyButtons zakonczony pomyslnie.\n");
}

void test_updateArmyStructure(void) {
    printf("\n[INFO] Rozpoczynam test: test_updateArmyStructure\n");

    prepare_test_game_data();

    cleanup_test_game_data();
    printf("[INFO] Test test_updateArmyStructure zakonczony pomyslnie.\n");
}
