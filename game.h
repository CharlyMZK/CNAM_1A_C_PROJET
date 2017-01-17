#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dessine.h"

// Structure de la pierre
typedef struct Structure_Stone{
  int color; // 0 pour les noirs et 1 pour les blancs
}Stone;

// Structure du plateau de jeu
typedef struct Structure_Board{
  Stone* stones[19][19]; // Le plateau de jeu est constitué des intersections
}Board;

void draw_win();
void mouse_clicked(int bouton, int x, int y);
void key_pressed(KeySym code, char c, int x_souris, int y_souris);
void print_board(Board* board, int size);
Board* create_board();
void play_stone(Board* board, int color, int x, int y);
