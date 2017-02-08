#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dessine.h"

// Structure de la pierre
typedef struct Structure_Stone{
  char color;
  int x;
  int y;
  bool visible; // Permet de savoir s'il faut afficher la pierre
}Stone;

// Structure du plateau de jeu
typedef struct Structure_Board{
  int size; //Taille du plateau
  Stone** intersections; // Tableau des cases
}Board;

// Structure d'une chaine
typedef struct Structure_Chain{
  int chain_size; //Taille du plateau
  int number_of_freedoms; // -- Nombre de libertés
  Stone** stones; // Tableau des cases
}Chain;

//Structure des territoires
typedef struct Structure_Territory{
    int size; //Taille du territoire
    int min_x;
    int min_y;
    int max_x;
    int max_y; // Coordonnées pour la recherche
    Stone** stones; // Tableau des cases
}Territory;

typedef struct Structure_Chains{
  int number_of_chain;
  Chain** chains;
}Chains;

void pass();
void check_game_finished();
void game_finished();
void player_play(int x, int y);
void bot_play();
void draw_player_turn();
void draw_win();
void draw_win_menu();
int test_clicked(int coord);
void draw_hoshi();
void drop_stone(int x, int y);
void drop_white_stone(int x, int y);
void drop_black_stone(int x, int y);
void mouse_clicked(int bouton, int x, int y);
void key_pressed(KeySym code, char c, int x_souris, int y_souris);
void print_board();
void print_territory();
void init_board(int size);
Stone* get_stone(int x, int y);
void set_stone(int x, int y, Stone* stone);
int play_white_stone(int x, int y);
int play_black_stone(int x, int y);
int play_stone(int x, int y, char color);
void print_chains();
int add_in_chain(Stone* stone);
Chain* find_chain(Stone* stone);
void chain_captured(Chain* chain);
void redraw_win();
void modify_freedoms(Stone* stone);
int find_in_chain(Stone* stone);
int check_play(int x, int y);
bool is_in_same_chain(Stone* stone1, Stone* stone2);
int check_chain_liberties(int size, Stone** stones);
void check_stone_territory(Stone* stone);
void create_territory();
