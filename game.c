#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

/**
 * Mettre ici son code pour dessiner dans la fenetre
 * 1er affichage + redessine si resize
 */
void draw_win()
{
	// - Vide la fenetre
	clear_win();

	// -- Initialisation
	double box_size,i;
	int board_size = 19;

	// -- Lines color
	color(0,0,0);

	// -- Cadre du board
	rectangle(0,0,width_win(),height_win());

 	// -- Calcul du nombre de cases
	box_size = width_win() / board_size;

	// -- Dessin des lignes horizontales
	for(i=0; i<width_win(); i+=box_size)
		line(i,0,i, height_win());

 	// -- Dessin des lignes verticales
	for(i=0; i<height_win(); i+=box_size)
		line(0,i,width_win(),i);
}


/**
 * on a cliqué a la souris:
 * bouton: 1,2,3,4,5,... : gauche, milieu, droit, molette, ...
 * x,y position
 */
void mouse_clicked(int bouton, int x, int y)
{
	double i = x/30;
	double j = y/30;
	printf("\n Bouton %d presse au coord. %d,%d \n",bouton,x,y);
	printf("\n %f",i);
	printf("\n %f \n",j);
	color(0,0,0);
	filled_circle(x,y,5);
}


/**
 * on a appuyé sur une touche
 * code: code touche x11 (XK_...)
 * c caractère correspondant si caractere
 * x_souris,y_souris position de la souris
 */
void key_pressed(KeySym code, char c, int x_souris, int y_souris)
{
	switch(code)
	{
		case XK_Down:
			printf("bas\n");
			break;
		case XK_Up:
			printf("haut\n");
			break;
		case XK_Left:
			printf("gauche\n");
			break;
		case XK_Right:
			printf("droite\n");
			break;
	//~ case XK_Return:
	//~ case XK_Shift_L:
	//~ case XK_Control_R:
	//~ ....
		default:
			break;
	}

	if (c>' ' && c<'z')
		printf("char: %c \n",c);

	printf(" avec pos souris: %d,%d \n",x_souris,y_souris);

}

/*
 * Permet d'afficher ce qui est présent sur le plateau
 */
void print_board(Board* board, int size){
    for(int i = 0; i < size; i++){
      for(int j = 0; j < size; j++){
        printf("board[%i][%i] = %i", i, j, (board->stones[i][j]!=NULL)?(board->stones[i][j]->color):(-1));
      }
    }
}

/*
 * Permet de créer un tableau d'une taille défini
 */
Board* create_board(){
  Board* board = malloc(sizeof(Board));
  return board;
}

/*
 * Permet au joueur dont la couleur est passé en parametre de jouer un caillou
 */
void play_stone(Board* board, int color, int x, int y){
  Stone* stone = malloc(sizeof(Stone));
  stone->color = color;
  board->stones[x][y] = stone;
}
