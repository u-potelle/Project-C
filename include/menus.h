/**
 * @file menus.h
 * @brief Menus interactifs en ligne de commande de l'application Super-Héros.
 *
 * Ce module gère toute l'interaction utilisateur : recherche, filtrage,
 * comparaison, quiz et menu principal. Il s'appuie sur la bibliothèque
 * readline pour les saisies.
 *
 * Dépendances : cJSON, json_io.h, display.h, filter.h, utils.h
 */

#ifndef MENUS_H
#define MENUS_H

#include "cJSON.h"

/* ── Recherche / filtrage ────────────────────────────────────────────────── */

/**
 * @brief Boucle de commandes du mode filtrage avancé.
 *
 * Commandes disponibles : aide, filtre, liste, ajouter, retirer,
 * sauvegarder, ouvrir, quitter.
 *
 * @param json        Tableau JSON complet (base de référence).
 * @param heros       Tableau de travail courant (modifié en place).
 * @param HerosNumber Pointeur sur le nombre courant d'éléments dans @p heros.
 * @return            0 quand l'utilisateur saisit "quitter".
 */
int Menu_Find_Complex_Commands(cJSON *json, cJSON *heros, int *HerosNumber);

/**
 * @brief Affiche le menu de filtrage avancé et lance la boucle de commandes.
 *
 * Initialise un tableau de travail avec tous les héros, puis délègue
 * à `Menu_Find_Complex_Commands()`.
 *
 * @param json  Tableau JSON complet des héros.
 * @return      0 à la sortie.
 */
int Menu_Find_Complex(cJSON *json);

/**
 * @brief Recherche un héros par nom et affiche ses informations complètes.
 *
 * Invite l'utilisateur à saisir un nom, puis affiche les détails du héros
 * trouvé ou un message d'erreur.
 *
 * @param json  Tableau JSON complet des héros.
 * @return      0 en cas de succès, 1 si le héros est introuvable.
 */
int Menu_Find_ByName(const cJSON *json);

/**
 * @brief Sous-menu de recherche (par nom ou par filtres).
 *
 * @param json  Tableau JSON complet des héros.
 * @return      0 à la sortie.
 */
int Menu_Find(cJSON *json);

/* ── Comparaison ─────────────────────────────────────────────────────────── */

/**
 * @brief Compare côte à côte les attributs de deux héros saisis par l'utilisateur.
 *
 * Demande les noms des deux héros, puis affiche un tableau comparatif
 * de leurs powerstats et données d'apparence.
 *
 * @param json  Tableau JSON complet des héros.
 * @return      0 à la fin de la comparaison.
 */
int ComparaisonHeros(cJSON *json);

/* ── Quiz ────────────────────────────────────────────────────────────────── */

/**
 * @brief Lance une partie du quiz « devinez le héros ».
 *
 * Choisit un héros aléatoire et dévoile ses attributs un par un jusqu'à
 * ce que l'utilisateur trouve le nom ou que tous les indices soient épuisés.
 * Affiche le nombre d'indices utilisés en cas de victoire.
 *
 * @param json  Tableau JSON complet des héros.
 * @return      0 à la fin de la partie.
 */
int QuizzMenu(cJSON *json);

/* ── Menu principal ──────────────────────────────────────────────────────── */

/**
 * @brief Affiche le menu principal et exécute l'option choisie.
 *
 * Options : base complète, recherche, comparaison, quiz, ressources, quitter.
 *
 * @param json  Tableau JSON complet des héros.
 * @return      1 pour rester dans la boucle principale, 0 pour quitter.
 */
int Menu(cJSON *json);

#endif /* MENUS_H */
