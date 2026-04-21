/**
 * @file utils.h
 * @brief Utilitaires généraux : compatibilité multi-plateforme et manipulation de chaînes.
 *
 * Ce module fournit des fonctions indépendantes de cJSON, réutilisables
 * dans l'ensemble du projet.
 */

#ifndef UTILS_H
#define UTILS_H

/* ── Compatibilité multi-plateforme ──────────────────────────────────────── */

/**
 * @brief Efface le terminal (compatible Windows, Linux et autres).
 *
 * Sous Windows appelle `cls`, sous Linux `clear`,
 * et affiche un séparateur visuel sur les autres plateformes.
 */
void clear(void);

/* ── Manipulation de chaînes ─────────────────────────────────────────────── */

/**
 * @brief Convertit un entier en chaîne de caractères allouée dynamiquement.
 *
 * @param n  L'entier à convertir.
 * @return   Pointeur vers la chaîne allouée (à libérer avec `free()`).
 */
char *intTostr(int n);

/**
 * @brief Compare deux chaînes de caractères (sensible à la casse).
 *
 * Équivalent sûr de `strcmp` qui gère les pointeurs NULL.
 *
 * @param str1 Première chaîne (peut être NULL).
 * @param str2 Deuxième chaîne (peut être NULL).
 * @return     1 si les chaînes sont identiques, 0 sinon.
 */
int strsame(const char *str1, const char *str2);

/**
 * @brief Insère la chaîne @p b dans le tampon @p a à partir de la position @p *n.
 *
 * Ne réalloue pas @p a : le tampon doit être assez grand.
 *
 * @param a  Tampon de destination (pré-alloué).
 * @param b  Chaîne source à insérer.
 * @param n  Pointeur sur la position courante dans @p a (mis à jour).
 * @return   Nouvelle position après l'insertion (= ancienne position + longueur de @p b).
 */
int strinsert(char *a, char *b, int *n);

#endif /* UTILS_H */
