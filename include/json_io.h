/**
 * @file json_io.h
 * @brief Chargement, sauvegarde et accès aux données JSON des super-héros.
 *
 * Ce module encapsule toutes les opérations d'entrée/sortie sur les fichiers
 * JSON ainsi que les fonctions d'extraction de valeurs depuis un arbre cJSON.
 *
 * Dépendances : cJSON, utils.h
 */

#ifndef JSON_IO_H
#define JSON_IO_H

#include "cJSON.h"

/* ── Entrée / Sortie fichier ──────────────────────────────────────────────── */

/**
 * @brief Charge et parse un fichier JSON depuis le disque.
 *
 * Ouvre le fichier, lit son contenu en mémoire, puis le parse avec cJSON.
 * Affiche un message d'erreur si le fichier est introuvable ou mal formé.
 *
 * @param path  Chemin vers le fichier JSON.
 * @return      Pointeur vers l'arbre cJSON alloué (à libérer avec `cJSON_Delete()`),
 *              ou NULL en cas d'erreur.
 */
cJSON *loadJson(char *path);

/**
 * @brief Sérialise un objet cJSON et l'écrit dans un fichier.
 *
 * Utilise `cJSON_Print()` pour produire une représentation lisible.
 *
 * @param json   Arbre cJSON à sauvegarder.
 * @param chemin Chemin du fichier de destination (créé ou écrasé).
 * @return       0 en cas de succès, 1 en cas d'erreur d'écriture.
 */
int saveJson(cJSON *json, char *chemin);

/* ── Accès aux données ───────────────────────────────────────────────────── */

/**
 * @brief Retourne la valeur d'un champ de premier niveau sous forme de chaîne.
 *
 * Gère les champs de type string et number.
 * Pour un number, le résultat est alloué dynamiquement (via `intTostr`).
 *
 * @param Json    Objet JSON contenant le champ.
 * @param element Nom du champ recherché.
 * @return        Valeur sous forme de chaîne, ou "" si absent.
 */
char *getStringElm(const cJSON *Json, const char *element);

/**
 * @brief Recherche un héros par nom exact dans un tableau JSON.
 *
 * La comparaison est sensible à la casse.
 *
 * @param Json  Tableau cJSON de héros.
 * @param name  Nom du héros à rechercher.
 * @return      Indice du héros dans le tableau, ou -1 s'il est absent.
 */
int findByName(const cJSON *Json, const char *name);

/**
 * @brief Recherche récursive d'un champ numérique dans un arbre JSON.
 *
 * Parcourt objets et tableaux imbriqués jusqu'à trouver le champ @p elm.
 *
 * @param json  Racine de l'arbre JSON à parcourir.
 * @param elm   Nom du champ numérique recherché.
 * @return      Valeur entière du champ, ou -1 si absent.
 */
int FindJsonValueElm(const cJSON *json, char *elm);

/**
 * @brief Recherche récursive d'un champ sous forme de chaîne dans un arbre JSON.
 *
 * Gère les types string, number et array (les éléments du tableau sont
 * concaténés et séparés par "/").
 *
 * @param json  Racine de l'arbre JSON à parcourir.
 * @param elm   Nom du champ recherché.
 * @return      Valeur sous forme de chaîne (peut être allouée dynamiquement),
 *              ou "" si absent.
 */
char *FindJsonStrElm(const cJSON *json, char *elm);

#endif /* JSON_IO_H */
