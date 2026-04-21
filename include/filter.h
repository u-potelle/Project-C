/**
 * @file filter.h
 * @brief Fonctions de filtrage des super-héros par attribut numérique.
 *
 * Dépendances : cJSON, json_io.h
 */

#ifndef FILTER_H
#define FILTER_H

#include "cJSON.h"

/**
 * @brief Teste si un héros satisfait une condition sur un champ numérique.
 *
 * Recherche récursivement le champ @p elm dans l'objet @p hero et compare
 * sa valeur entière avec @p nombre selon l'opérateur @p op.
 *
 * Opérateurs acceptés : `'<'`, `'>'`, `'='`.
 *
 * @param hero    Objet JSON représentant un héros.
 * @param elm     Nom du champ numérique à tester (ex. "strength").
 * @param op      Opérateur de comparaison : `'<'`, `'>'` ou `'='`.
 * @param nombre  Valeur de référence pour la comparaison.
 * @return        1 si la condition est vraie, 0 sinon (ou si le champ est absent).
 */
int Menu_Find_Complex_filter(cJSON *hero, char *elm, char op, int nombre);

#endif /* FILTER_H */
