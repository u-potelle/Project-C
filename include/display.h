/**
 * @file display.h
 * @brief Fonctions d'affichage des informations des super-héros dans le terminal.
 *
 * Toutes les sorties textuelles liées aux héros et aux ressources externes
 * sont regroupées ici pour séparer la logique d'affichage du reste.
 *
 * Dépendances : cJSON, json_io.h
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "cJSON.h"

/* ── Primitives d'affichage JSON ─────────────────────────────────────────── */

/**
 * @brief Affiche la valeur brute d'un nœud JSON (number ou string).
 *
 * N'affiche rien si le type n'est ni number ni string.
 *
 * @param Json  Nœud cJSON à afficher.
 */
void printJson(const cJSON *Json);

/**
 * @brief Affiche un champ nommé d'un objet JSON sur une ligne.
 *
 * Format : `<clé>: <valeur>\n`. Gère les types number, string et array
 * (les éléments d'un tableau sont affichés séparés par des espaces).
 *
 * @param Json    Objet JSON parent.
 * @param element Nom du champ à afficher.
 */
void printJsonElt(const cJSON *Json, const char *element);

/* ── Affichage d'un héros ────────────────────────────────────────────────── */

/**
 * @brief Affiche les informations d'identification d'un héros (id, name, id-api, slug).
 *
 * @param mainJson  Objet JSON représentant un héros.
 */
void printHeroInfo(const cJSON *mainJson);

/**
 * @brief Affiche les statistiques de puissance d'un héros.
 *
 * Champs affichés : intelligence, strength, speed, durability, power, combat.
 *
 * @param PowerJson  Objet JSON "powerstats" du héros.
 */
void printHeroPowers(const cJSON *PowerJson);

/**
 * @brief Affiche les données d'apparence d'un héros.
 *
 * Champs affichés : gender, race, height, weight (impérial|métrique), eyeColor, hairColor.
 *
 * @param AppearJson  Objet JSON "appearance" du héros.
 */
void printHeroAppear(const cJSON *AppearJson);

/**
 * @brief Affiche toutes les informations d'un héros (identité + pouvoirs + apparence).
 *
 * @param mainJson  Tableau JSON complet des héros.
 * @param n         Indice du héros dans le tableau.
 */
void printHeroInfoFull(const cJSON *mainJson, int n);

/**
 * @brief Affiche la liste synthétique de tous les héros (id, name, id-api, slug).
 *
 * @param mainJson  Tableau JSON complet des héros.
 */
void printHerosList(const cJSON *mainJson);

/* ── Aide et ressources ──────────────────────────────────────────────────── */

/**
 * @brief Affiche la liste des champs filtrables disponibles.
 *
 * @param json  Tableau JSON des héros (utilisé pour le contexte, non parcouru ici).
 */
void PrintHelp(const cJSON *json);

/**
 * @brief Affiche un tableau de liens externes vers des ressources sur les super-héros.
 *
 * Liens : Wikipedia, Vikidia, Wiktionnaire, Alucare, YouTube.
 */
void printExternalLinks(void);

#endif /* DISPLAY_H */
