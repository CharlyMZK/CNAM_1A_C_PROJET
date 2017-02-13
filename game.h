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
  int handicap;                   // -- Nombre d'handicap
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

//Structure d'une ligne du plateau
typedef struct Structure_Line{
    int min_x;                    // -- Coordonnée x minimum
    int max_x;                    // -- Coordonnée x maximum
    int y;                        // -- Coordonnée y
}Line;

// Structure d'une colonne du plateau
typedef struct Structure_Column{
    int x;                        // -- Coordonnée x
    int min_y;                    // -- Coordonnée y minimum
    int max_y;                    // -- Coordonnée y maximum
}Column;

//Structure des lignes du plateau
typedef struct Structure_Lines{
    int size;                     // -- Tailles des lignes
    Line** lines;                 // -- Tableau des lignes
    char color;                   // -- Couleur de la ligne
}Lines;

// Structure des colonnes du plateau
typedef struct Structure_Columns{
    int size;                     // -- Taille des colonnes
    Column** columns;             // -- Tableau des colonnes
    char color;                   // -- Couleur de la colonne
}Columns;


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
 * Pose la pierre sur le board de façon graphique
 */
void drop_stone(int x, int y, char color);

/*
 * Permet au joueur dont la couleur est passé en parametre de jouer un caillou
 * Retourne true si le joueur a pu jouer, false sinon
 */
bool play_stone(int x, int y, char color);

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
 * Permet d'afficher les informations d'une pierre
 */
void print_stone(Stone* stone);

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
 * Retourne si la pierre capture les pierres qui sont autour  
 */
bool check_around_captured(Stone* stone);

/*
 * Verifie si la chaine de la stone1 est dans la même chaine que la chaine de la stone2
 */
bool is_in_same_chain(Stone* stone1, Stone* stone2);

/*
 * Permet de savoir si une pierre a des libertées
 * Retourne true ou false
 */
bool check_stone_liberties(Stone* stone);

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
 * Permet de savoir si la pierre passé en paramètre se situe sur le bord du plateau
 * Retourne true si c'est sur la bord
 */
bool is_on_border(Stone* stone);

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
void fill_board(Lines* lines, Columns* columns);

/*
 * Permet de remettre à 0 toutes la variables de la ligne passé en paramètre
 */
Line*  init_line();

/*
 * Permet de remettre à 0 toutes la variables de la colonne passé en paramètre
 */
Column* init_column();

/*
 * Permet de sauvegarder la partie dans un fichier
 */
void save_game(Board* board);

/*
 * Permet de crééer le fichier et de mettre l'entête dedans
 */
void create_header(int board_size, int handicap);

/*
 * Permet d'écrire la partie dans le fichier
 */
void write_game(Board* board);

/*
 * Permet d'importer un fichier
 */
void import_file(char* file_name, Board* board);

/*
 * Permet d'importer les pions sur le board
 */
void import_game(Board* board);
