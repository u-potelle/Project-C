/**
 * @file display.c
 * @brief Implémentation des fonctions d'affichage (voir display.h).
 */

#include <stdio.h>
#include "cJSON.h"
#include "display.h"
#include "json_io.h"

/* ── Primitives d'affichage JSON ─────────────────────────────────────────── */

void printJson(const cJSON *Json){
    if (cJSON_IsNumber(Json)){ printf("%d", Json->valueint); }
    if (cJSON_IsString(Json)){ printf("%s", Json->valuestring); }
}

void printJsonElt(const cJSON *Json, const char *element){
    cJSON *champ = cJSON_GetObjectItem(Json, element);
    if (cJSON_IsNumber(champ)){ printf("%s: %d\n", element, champ->valueint); }
    else if (cJSON_IsString(champ)){ printf("%s: %s\n", element, champ->valuestring); }
    else if (cJSON_IsArray(champ)){
        printf("%s", element);
        for (int i = 0; i < cJSON_GetArraySize(champ); i++){
            printf(" ");
            printJson(cJSON_GetArrayItem(champ, i));
        }
        printf("\n");
    }
}

/* ── Affichage d'un héros ────────────────────────────────────────────────── */

void printHeroInfo(const cJSON *mainJson){
    printJsonElt(mainJson, "id");
    printJsonElt(mainJson, "name");
    printJsonElt(mainJson, "id-api");
    printJsonElt(mainJson, "slug");
}

void printHeroPowers(const cJSON *PowerJson){
    printJsonElt(PowerJson, "intelligence");
    printJsonElt(PowerJson, "strength");
    printJsonElt(PowerJson, "speed");
    printJsonElt(PowerJson, "durability");
    printJsonElt(PowerJson, "power");
    printJsonElt(PowerJson, "combat");
}

void printHeroAppear(const cJSON *AppearJson){
    printJsonElt(AppearJson, "gender");
    printJsonElt(AppearJson, "race");
    printJsonElt(AppearJson, "height");
    cJSON *poids = cJSON_GetObjectItem(AppearJson, "weight");
    if (cJSON_IsArray(poids)){
        cJSON *imp = cJSON_GetArrayItem(poids, 0);
        cJSON *met = cJSON_GetArrayItem(poids, 1);
        if (cJSON_IsString(imp)){ printf("weight: %s | ", imp->valuestring); }
        if (cJSON_IsString(met)){ printf("%s\n", met->valuestring); }
    }
    printJsonElt(AppearJson, "eyeColor");
    printJsonElt(AppearJson, "hairColor");
}

void printHeroInfoFull(const cJSON *mainJson, int n){
    cJSON *hero = cJSON_GetArrayItem(mainJson, n);
    printHeroInfo(hero);
    printf("-pouvoirs:\n");
    printHeroPowers(cJSON_GetObjectItem(hero, "powerstats"));
    printf("-apparence:\n");
    printHeroAppear(cJSON_GetObjectItem(hero, "appearance"));
}

void printHerosList(const cJSON *mainJson){
    int len = cJSON_GetArraySize(mainJson);
    for (int i = 0; i < len; i++){
        printf("------------------------\n");
        printHeroInfo(cJSON_GetArrayItem(mainJson, i));
    }
    printf("------------------------\n");
}

/* ── Aide et ressources ──────────────────────────────────────────────────── */

void PrintHelp(const cJSON *json){
    (void)json; /* paramètre réservé pour une future aide contextuelle */
    printf("id\n");
    printf("id-api\n");
    printf("intelligence\n");
    printf("strength\n");
    printf("speed\n");
    printf("durability\n");
    printf("power\n");
    printf("combat\n");
}

void printExternalLinks(void){

    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║           RESSOURCES EXTERNES - SUPER-HEROS                  ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║ [1] Wikipedia - Super-heros                                  ║\n");
    printf("║     https://fr.wikipedia.org/wiki/Super-h%%C3%%A9ros           ║\n");
    printf("║                                                              ║\n");
    printf("║ [2] Vikidia - Superheros                                     ║\n");
    printf("║     https://fr.vikidia.org/wiki/Superh%%C3%%A9ros              ║\n");
    printf("║                                                              ║\n");
    printf("║ [3] Wikipedia - La Legion des super-heros                    ║\n");
    printf("║     https://fr.wikipedia.org/wiki/La_L%%C3%%A9gion_des_        ║\n");
    printf("║     super-h%%C3%%A9ros                                         ║\n");
    printf("║                                                              ║\n");
    printf("║ [4] Wikipedia - Puissants Vengeurs                           ║\n");
    printf("║     https://fr.wikipedia.org/wiki/Puissants_Vengeurs         ║\n");
    printf("║                                                              ║\n");
    printf("║ [5] Wiktionnaire - Définition super-heros                    ║\n");
    printf("║     https://fr.wiktionary.org/wiki/super-h%%C3%%A9ros          ║\n");
    printf("║                                                              ║\n");
    printf("║ [6] Alucare - Liste complète des super-heros                 ║\n");
    printf("║     https://www.alucare.fr/la-liste-complete-des-super-      ║\n");
    printf("║     heros/                                                   ║\n");
    printf("║                                                              ║\n");
    printf("║ [7] YouTube - Vidéo super-heros                              ║\n");
    printf("║     https://youtu.be/kLo_t7RI9pE?si=3_wOoBfD5TNC-km7         ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
}
