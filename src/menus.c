/**
 * @file menus.c
 * @brief Implémentation des menus interactifs (voir menus.h).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include "cJSON.h"
#include "menus.h"
#include "json_io.h"
#include "display.h"
#include "filter.h"
#include "utils.h"

/* ── Recherche / filtrage ────────────────────────────────────────────────── */

int Menu_Find_Complex_Commands(cJSON *json, cJSON *heros, int *HerosNumber){
    while (1){
        char *input = readline("-> ");

        if (strsame(input, "aide")){
            PrintHelp(cJSON_GetArrayItem(heros, 0));
        }
        else if (strsame(input, "filtre")){
            printf("retire les héros quand la condition est vraie\n");
            char *elm = readline("champ à filtrer\t\t-> ");
            char op   = readline("opération ( =, >, < )\t-> ")[0];
            int  nb   = atoi(readline("nombre\t\t\t-> "));
            int i = 0;
            printf("\n");
            while (i < *HerosNumber){
                if (Menu_Find_Complex_filter(cJSON_GetArrayItem(heros, i), elm, op, nb) == 0){
                    printf("suppression de %s\n",
                        cJSON_GetObjectItem(cJSON_GetArrayItem(heros, i), "name")->valuestring);
                    cJSON_DeleteItemFromArray(heros, i);
                    *HerosNumber -= 1;
                }
                else{ i++; }
            }
        }
        else if (strsame(input, "liste")){
            printHerosList(heros);
        }
        else if (strsame(input, "ajouter")){
            char *nom;
            while (1){
                nom = readline("nom du héros à ajouter\t-> ");
                if (findByName(json, nom) != -1){ break; }
                printf("héros introuvable dans la base, réessayez.\n");
            }
            if (findByName(heros, nom) == -1){
                int i = 0;
                int idCible = FindJsonValueElm(cJSON_GetArrayItem(json, findByName(json, nom)), "id");
                for (; i < cJSON_GetArraySize(heros); i++){
                    if (FindJsonValueElm(cJSON_GetArrayItem(heros, i), "id") > idCible){ break; }
                }
                cJSON_InsertItemInArray(heros, i,
                    cJSON_Duplicate(cJSON_GetArrayItem(json, findByName(json, nom)), 1));
                *HerosNumber += 1;
                printf("héros ajouté\n");
            }
            else{ printf("héros déjà présent\n"); }
        }
        else if (strsame(input, "retirer")){
            char *nom;
            while (1){
                nom = readline("nom du héros à retirer\t-> ");
                if (findByName(json, nom) != -1){ break; }
                printf("héros introuvable dans la base, réessayez.\n");
            }
            if (findByName(heros, nom) != -1){
                cJSON_DeleteItemFromArray(heros, findByName(heros, nom));
                *HerosNumber -= 1;
                printf("héros retiré\n");
            }
            else{ printf("héros absent de la liste\n"); }
        }
        else if (strsame(input, "sauvegarder")){
            char *chemin = readline("chemin du fichier de sauvegarde\t-> ");
            saveJson(heros, chemin);
        }
        else if (strsame(input, "ouvrir")){
            /* NOTE : le résultat est ignoré ici — à améliorer si besoin */
            loadJson(readline("chemin du fichier à ouvrir\t->"));
        }
        else if (strsame(input, "quitter")){
            return 0;
        }
        else{
            printf("commande inconnue (aide / filtre / liste / ajouter / retirer / sauvegarder / ouvrir / quitter)\n");
        }
    }
}

int Menu_Find_Complex(cJSON *json){
    clear();
    printf("--------------------------\n");
    printf("filtrer et gérer les héros\n");
    printf("commandes : aide  filtre  liste  ajouter  retirer  ouvrir  sauvegarder  quitter\n\n");

    cJSON *heros = cJSON_CreateArray();
    int HerosNumber = cJSON_GetArraySize(json);

    for (int i = 0; i < HerosNumber; i++){
        cJSON_AddItemReferenceToArray(heros, cJSON_GetArrayItem(json, i));
    }

    Menu_Find_Complex_Commands(json, heros, &HerosNumber);

    cJSON_Delete(heros);
    return 0;
}

int Menu_Find_ByName(const cJSON *json){
    printf("\n--------------------------\n");
    printf("Recherche par nom\n");
    char *choix = readline("nom du héros -> ");
    int id = findByName(json, choix);
    if (id == -1){
        printf("Héros '%s' introuvable.\n", choix);
        return 1;
    }
    printf("le héros '%s' est à l'indice %d\n\n", choix, id);
    printHeroInfoFull(json, id);
    return 0;
}

int Menu_Find(cJSON *json){
    printf("+-------------------------------+\n");
    printf("| Menu recherche\t\t|\n");
    printf("| 1 - recherche par nom\t\t|\n");
    printf("| 2 - recherche avec filtres\t|\n");
    printf("+-------------------------------+\n");
    char *input = readline("-> ");
    if (input == NULL){ return 1; }
    switch (atoi(input)){
        case 1: Menu_Find_ByName(json); break;
        case 2: Menu_Find_Complex(json); break;
    }
    return 0;
}

/* ── Comparaison ─────────────────────────────────────────────────────────── */

int ComparaisonHeros(cJSON *json){
    int id1, id2;

    while (1){
        char *nom = readline("nom du premier héros\t-> ");
        id1 = findByName(json, nom);
        if (id1 != -1){ break; }
        printf("héros introuvable, réessayez.\n");
    }
    while (1){
        char *nom = readline("nom du second héros\t-> ");
        id2 = findByName(json, nom);
        if (id2 != -1){ break; }
        printf("héros introuvable, réessayez.\n");
    }

    cJSON *hero1 = cJSON_GetArrayItem(json, id1);
    cJSON *hero2 = cJSON_GetArrayItem(json, id2);

    char *cles[14] = {
        "name", "id",
        "intelligence", "strength", "speed", "durability", "power", "combat",
        "gender", "race", "height", "weight", "eyeColor", "hairColor"
    };

    printf("+-----------------------------------------------+\n");
    for (int i = 0; i < 14; i++){
        char *v1 = FindJsonStrElm(hero1, cles[i]);
        char *v2 = FindJsonStrElm(hero2, cles[i]);
        printf("%-14s : %-20s | %s\n", cles[i],
            v1 ? v1 : "N/A",
            v2 ? v2 : "N/A");
    }
    printf("+-----------------------------------------------+\n");
    return 0;
}

/* ── Quiz ────────────────────────────────────────────────────────────────── */

int QuizzMenu(cJSON *json){
    int heroId = rand() % cJSON_GetArraySize(json);
    cJSON *hero = cJSON_Duplicate(cJSON_GetArrayItem(json, heroId), 1);

    char *prop[] = {
        "intelligence", "strength", "speed", "durability",
        "power", "combat", "gender", "race",
        "height", "weight", "eyeColor", "hairColor"
    };
    int propNum = 12;
    int *propUtilise = calloc(propNum, sizeof(int));
    int indiceIndice = rand() % propNum;
    int indices = 0;
    int gagne = 0;

    printf("Retrouvez le héros avec le moins d'indices possible !\n");

    while (indices < propNum){
        while (propUtilise[indiceIndice] != 0){
            indiceIndice = rand() % propNum;
        }
        propUtilise[indiceIndice] = 1;
        indices++;

        printf("Indice n°%d : %s = %s\n",
            indices, prop[indiceIndice],
            FindJsonStrElm(hero, prop[indiceIndice]));

        printf("Votre réponse -> ");
        char *reponse = readline("");
        if (strsame(FindJsonStrElm(hero, "name"), reponse)){
            gagne = 1;
            break;
        }
    }

    if (gagne){
        printf("Bravo ! Le héros était bien %s (trouvé en %d indice(s))\n",
            FindJsonStrElm(hero, "name"), indices);
    }
    else{
        printf("Perdu ! Le héros était %s\n", FindJsonStrElm(hero, "name"));
    }

    free(propUtilise);
    cJSON_Delete(hero);
    return 0;
}

/* ── Menu principal ──────────────────────────────────────────────────────── */

int Menu(cJSON *json){
    printf("+-------------------------------+\n");
    printf("| Menu principal\t\t|\n");
    printf("| 1 - base de données complète\t|\n");
    printf("| 2 - recherche\t\t\t|\n");
    printf("| 3 - comparaison\t\t|\n");
    printf("| 4 - quizz\t\t\t|\n");
    printf("| 5 - ressources externes\t|\n");
    printf("| 6 - quitter\t\t\t|\n");
    printf("+-------------------------------+\n\n");

    char *input = readline("-> ");
    switch (atoi(input)){
        case 1:
            clear();
            for (int i = 0; i < cJSON_GetArraySize(json); i++){
                printHeroInfoFull(json, i);
                printf("-------------------\n");
            }
            readline("appuyez sur entrée pour continuer-> ");
            break;
        case 2:
            clear();
            Menu_Find(json);
            break;
        case 3:
            clear();
            ComparaisonHeros(json);
            readline("appuyez sur entrée pour continuer-> ");
            break;
        case 4:
            clear();
            QuizzMenu(json);
            readline("appuyez sur entrée pour continuer-> ");
            break;
        case 5:
            clear();
            printExternalLinks();
            readline("appuyez sur entrée pour continuer-> ");
            break;
        case 6:
            clear();
            return 0;
    }
    return 1;
}
