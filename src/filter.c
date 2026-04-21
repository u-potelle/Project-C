/**
 * @file filter.c
 * @brief Implémentation du filtre par attribut numérique (voir filter.h).
 */

#include "cJSON.h"
#include "filter.h"
#include "json_io.h"

int Menu_Find_Complex_filter(cJSON *hero, char *elm, char op, int nombre){
    int n = FindJsonValueElm(hero, elm);
    if (n == -1){ return 0; }
    switch (op){
        case '<': return n < nombre;
        case '>': return n > nombre;
        case '=': return n == nombre;
        default:  return 0;
    }
}
