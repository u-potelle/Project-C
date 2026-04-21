/**
 * @file utils.c
 * @brief Implémentation des utilitaires généraux (voir utils.h).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/* ── Compatibilité multi-plateforme ──────────────────────────────────────── */

#if defined(_WIN32) || defined(_WIN64)
void clear(void){ system("cls"); }
#elif __linux__
void clear(void){ system("clear"); }
#else
void clear(void){
    printf("\n-------------------------\n");
}
#endif

/* ── Manipulation de chaînes ─────────────────────────────────────────────── */

char *intTostr(int n){
    char *buffer = malloc(12);
    snprintf(buffer, 12, "%d", n);
    return buffer;
}

int strsame(const char *str1, const char *str2){
    if (str1 == NULL || str2 == NULL){ return 0; }
    if (strlen(str1) != strlen(str2)){ return 0; }
    for (int i = 0; i < (int)strlen(str1); i++){
        if (str1[i] != str2[i]){ return 0; }
    }
    return 1;
}

int strinsert(char *a, char *b, int *n){
    for (int i = 0; i < (int)strlen(b); i++){
        a[*n + i] = b[i];
    }
    return *n + strlen(b);
}
