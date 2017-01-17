#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dessine.h"

// Structure de la pierre
typedef struct Structure_Stone{
  int color; // 0 pour les noirs et 1 pour les blancs
  int visible; // Permet de savoir s'il faut afficher la pierre
}Stone;

// Structure du plateau de jeu
typedef struct Structure_Board{
  int size; //Taille du plateau
  Stone** intersections; // Tableau des cases
}Board;

void player_play();
void draw_win();
int test_clicked(int coord);
void drop_stone(int x, int y);
void mouse_clicked(int bouton, int x, int y);
void key_pressed(KeySym code, char c, int x_souris, int y_souris);
void print_board();
void init_board(int size);
Stone* get_stone(int x, int y);
void set_stone(int x, int y, Stone* stone);
int play_stone(int x, int y, int color);
int check_play(int x, int y);
