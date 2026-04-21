/**
 * @file json_io.c
 * @brief Implémentation du chargement, de la sauvegarde et de l'accès JSON (voir json_io.h).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "json_io.h"
#include "utils.h"

/* ── Entrée / Sortie fichier ──────────────────────────────────────────────── */

cJSON *loadJson(char *path){
    FILE *fp = fopen(path, "r");
    if (fp == NULL){
        printf("Erreur : impossible d'ouvrir '%s'\n", path);
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *buffer = malloc(size + 1);
    if (buffer == NULL){
        printf("Erreur : allocation mémoire échouée\n");
        fclose(fp);
        return NULL;
    }
    fread(buffer, 1, size, fp);
    buffer[size] = '\0';
    fclose(fp);

    cJSON *json = cJSON_Parse(buffer);
    free(buffer);
    if (json == NULL){
        const char *err = cJSON_GetErrorPtr();
        if (err){ printf("Erreur de parsing JSON près de : %s\n", err); }
        return NULL;
    }
    printf("'%s' chargé (%ld octets)\n", path, size);
    return json;
}

int saveJson(cJSON *json, char *chemin){
    char *json_str = cJSON_Print(json);
    printf("sauvegarde de '%s' ...\n", chemin);
    FILE *fp = fopen(chemin, "w");
    if (fp == NULL){
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        free(json_str);
        return 1;
    }
    fprintf(fp, "%s", json_str);
    fclose(fp);
    free(json_str);
    printf("sauvegardé\n");
    return 0;
}

/* ── Accès aux données ───────────────────────────────────────────────────── */

char *getStringElm(const cJSON *Json, const char *element){
    cJSON *elm = cJSON_GetObjectItem(Json, element);
    if (cJSON_IsString(elm)){ return elm->valuestring; }
    else if (cJSON_IsNumber(elm)){ return intTostr(elm->valueint); }
    return "";
}

int findByName(const cJSON *Json, const char *name){
    if (cJSON_IsArray(Json)){
        for (int i = 0; i < cJSON_GetArraySize(Json); i++){
            cJSON *hero = cJSON_GetArrayItem(Json, i);
            if (strsame(name, getStringElm(hero, "name"))){ return i; }
        }
    }
    return -1;
}

int FindJsonValueElm(const cJSON *json, char *elm){
    if (cJSON_IsNumber(cJSON_GetObjectItem(json, elm))){
        return cJSON_GetObjectItem(json, elm)->valueint;
    }
    else if (cJSON_IsArray(json)){
        for (int i = 0; i < cJSON_GetArraySize(json); i++){
            return FindJsonValueElm(cJSON_GetArrayItem(json, i), elm);
        }
    }
    else if (cJSON_IsObject(json)){
        for (int i = 0; i < cJSON_GetArraySize(json); i++){
            int valeur = FindJsonValueElm(cJSON_GetArrayItem(json, i), elm);
            if (valeur > 0){ return valeur; }
        }
    }
    return -1;
}

char *FindJsonStrElm(const cJSON *json, char *elm){
    cJSON *elmJson = cJSON_GetObjectItem(json, elm);
    if (cJSON_IsNumber(elmJson)){
        return intTostr(elmJson->valueint);
    }
    else if (cJSON_IsString(elmJson)){
        return elmJson->valuestring;
    }
    else if (cJSON_IsArray(elmJson)){
        /* assemble les éléments du tableau séparés par "/" */
        char *buffer = malloc(100);
        int pos = 0;
        for (int i = 0; i < cJSON_GetArraySize(elmJson); i++){
            cJSON *item = cJSON_GetArrayItem(elmJson, i);
            if (cJSON_IsNumber(item)){
                pos = strinsert(buffer, intTostr(item->valueint), &pos);
            }
            else if (cJSON_IsString(item)){
                pos = strinsert(buffer, item->valuestring, &pos);
            }
            if (i < cJSON_GetArraySize(elmJson) - 1){
                pos = strinsert(buffer, "/", &pos);
            }
        }
        buffer[pos] = '\0';
        return buffer;
    }
    else if (cJSON_IsArray(json)){
        for (int i = 0; i < cJSON_GetArraySize(json); i++){
            return FindJsonStrElm(cJSON_GetArrayItem(json, i), elm);
        }
    }
    else if (cJSON_IsObject(json)){
        for (int i = 0; i < cJSON_GetArraySize(json); i++){
            char *resultat = FindJsonStrElm(cJSON_GetArrayItem(json, i), elm);
            if (resultat != NULL && strlen(resultat) != 0){ return resultat; }
        }
    }
    return "";
}
