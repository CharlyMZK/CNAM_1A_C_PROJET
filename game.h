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

void draw_win();
void mouse_clicked(int bouton, int x, int y);
void key_pressed(KeySym code, char c, int x_souris, int y_souris);
void print_board();
void create_board(int size);
Stone* get_stone(int x, int y);
void set_stone(int x, int y, Stone* stone);
int play_white_stone(int x, int y);
int play_black_stone(int x, int y);
int play_stone(int x, int y, char color);
int check_play(int x, int y);
