/**
 * @file main.c
 * @brief Point d'entrée de l'application Super-Héros.
 *
 * Charge le fichier JSON, initialise le générateur aléatoire,
 * puis lance la boucle du menu principal.
 *
 * Compilation (exemple) :
 *   gcc -I include main.c src/*.c -o build/app -lreadline
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "json_io.h"
#include "menus.h"
#include "utils.h"

int main(void){
    srand(time(NULL));

    cJSON *json = loadJson("./json/SuperHeros.json");
    if (json == NULL){ return -1; }

    while (Menu(json)){ clear(); }

    cJSON_Delete(json);
    return 0;
}
