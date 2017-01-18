#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dessine.h"

// Structure de la pierre
typedef struct Structure_Stone{
  char color; // W pour blanc et B pour noir
  int visible; // Permet de savoir s'il faut afficher la pierre
}Stone;

// Structure du plateau de jeu
typedef struct Structure_Board{
  int size; //Taille du plateau
  Stone** intersections; // Tableau des cases
}Board;

int find_chain(int x, int y, int size, Stone** stones);
void player_play(int x, int y);
void draw_win();
int test_clicked(int coord);
void drop_stone(int x, int y);
void mouse_clicked(int bouton, int x, int y);
void key_pressed(KeySym code, char c, int x_souris, int y_souris);
void print_board();
void init_board(int size);
Stone* get_stone(int x, int y);
void set_stone(int x, int y, Stone* stone);
int play_white_stone(int x, int y);
int play_black_stone(int x, int y);
int play_stone(int x, int y, char color);
int check_play(int x, int y);
