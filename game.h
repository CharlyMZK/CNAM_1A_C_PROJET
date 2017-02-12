#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dessine.h"

// -- Structure de la pierre
typedef struct Structure_Stone{
  char color;                     // -- Couleur de la stone posée
  int x;                          // -- Coordonnée x
  int y;                          // -- Coordonnée y
  bool visible;                   // -- Permet de savoir s'il faut afficher la pierre
}Stone;

// -- Structure du plateau de jeu
typedef struct Structure_Board{
  int size;                       // -- Taille du plateau
  Stone** intersections;          // -- Tableau des cases
}Board;

//  -- tructure d'une chaine
typedef struct Structure_Chain{
  int chain_size;                 // -- Taille du plateau
  int number_of_freedoms;         // -- Nombre de libertés
  Stone** stones;                 // -- Tableau des cases
}Chain;

// -- Structure des territoires
typedef struct Structure_Territory{
    int size;                     // -- Taille du territoire
    int min_x;                    // -- Coordonnée x minimum
    int max_x;                    // -- Coordonnée x maximum
    int min_y;                    // -- Coordonnée y minimum
    int max_y;                    // -- Coordonnée y maximum
    Stone** stones;               // -- Tableau des cases
    Stone** stones_in_territory;  // -- Tableau des stones présentes dans le territoire
}Territory;

//Structure des lignes du plateau
typedef struct Structure_Line{
    int min_x;                    // -- Coordonnée x minimum
    int max_x;                    // -- Coordonnée x maximum
    int y;                        // -- Coordonnée y
    char color;                   // -- Couleur de la ligne
}Line;

// Structure des colonnes du plateau
typedef struct Structure_Column{
    int x;                        // -- Coordonnée x
    int min_y;                    // -- Coordonnée y minimum
    int max_y;                    // -- Coordonnée y maximum
    char color;                   // -- Couleur de la ligne
}Column;

// Structure des chaines
typedef struct Structure_Chains{
  int number_of_chain;            // -- Nombre de chaines
  Chain** chains;                 // -- Tableau de chaines
}Chains;

/**
 * Indique dans la textbox quel joueur joue
 */
void player_play(int x, int y);

/**
 * Le bot joue
 */
void bot_play();

/**
 * Pose le point de la pierre sur le board
 */
void drop_stone(int x, int y);

/*
 * Permet au joueur dont la couleur est passé en parametre de jouer un caillou
 * Retourne 1 si le jeu a pu jouer, 0 sinon
 */
int play_stone(int x, int y, char color);

/*
 * Permet de savoir si le joueur peut jouer
 * Retourne 1 si la pierre peut etre placée sur cette case
 */
int check_play(int x, int y);

/**
 * Test si le point est posé au milieu d'un carré et le remet correctement a l'intersection
 */
int test_clicked(int coord);

/**
 * on a appuyé sur une touche
 * code: code touche x11 (XK_...)
 * c caractère correspondant si caractere
 * x_souris,y_souris position de la souris
 */
void key_pressed(KeySym code, char c, int x_souris, int y_souris);

/**
 * on a cliqué a la souris:
 * bouton: 1,2,3,4,5,... : gauche, milieu, droit, molette, ...
 * x,y position
 */
void mouse_clicked(int bouton, int x, int y);

/*
 * Passe le tour du joueur
 */
void pass();

/**
 * Check si la partie est terminée
 */
void check_game_finished();

/**
 * Finis la partie
 */
void game_finished();

/**
 * Mettre ici son code pour dessiner dans la fenetre
 * 1er affichage + redessine si resize
 */
void draw_win();

/*
 * Redessine la fenêtre
 */
void redraw_win();

/*
 * Permet d'afficher ce qui est présent sur le plateau
 */
void print_board();

/*
 * Permet d'afficher ce qui est présent dans le territoire
 */
void print_territory();

/*
 * Permet de créer un tableau d'une taille défini avec un int passé en paramètre
 */
void init_board(int size);

/*
 * Permet de récupérer la pierre a x et y
 * Retourne NULL s'il n'y pas de pierre, la pierre si elle est bonne et une pierre avec color = "O" si elle est hors case
 */
Stone* get_stone(int x, int y);

/*
 * Permet de mettre une pierre à la position à x et y
 */
void set_stone(Stone* stone);

/*
 * Affiche le tableau de chaines
 */
void print_chains();

/*
 * Ajoute une pierre dans le tableau de chaines
 */
int add_in_chain(Stone* stone);

/*
 * Cherche si la pierre est dans une chaine
 */
Chain* find_chain(Stone* stone);

/*
 * Modifie les libertés des chaines autour
 */
void modify_freedoms(Stone* stone);

/*
 * Gère la capture des chaines
 */
void chain_captured(Chain* chain);

/*
 * Verifie si la chaine de la stone1 est dans la même chaine que la chaine de la stone2
 */
bool is_in_same_chain(Stone* stone1, Stone* stone2);

/*
 * Permet de savoir s'il reste des libertés a la chaine
 * Retourne 1 si oui, non sinon
 */
int check_chain_liberties(int size, Stone** stones);

/*
 * Permet de récupérer la pierre a x et y du territoire
 * Retourne NULL s'il n'y pas de pierre, la pierre si elle est bonne et une pierre avec color = "O" si elle est hors case
 */
Stone* get_stone_territory(int x, int y);

/*
 * Permet de mettre une pierre à la position x et y dans le territoire
 */
void set_stone_territory(Stone* stone);

/*
 * Permet d'appeler toutes les fonctions liées au territoire
 */
void do_territory(Stone* stone);

/*
 * Permet de savoir si la pierre peut délimiter un territoire et si oui, appele la fonction adéquat
 */
void do_stone_territory(Stone* stone);

/*
 *  Permet de créer un territoire
 */
void create_territory();

/*
 * Permet d'ajuste les x et y du territoire
 */
void adjust_territory(Stone* stone);

/*
 * Permet de remplir les lignes et colonnes pour ensuite mettre les pierres sur la board
 */
void seek_intersetion_territory(int color);

/*
* Permet de remplir une ligne de pierre invisible dans le tableau du plateau
*/
void fill_board(Line* line, Column* column);

/*
 * Permet de remettre à 0 toutes la variables de la ligne passé en paramètre
 */
void init_line(Line* line);

/*
 * Permet de remettre à 0 toutes les variables de la colonne passée en paramètre
 */
void init_column(Column* column);
