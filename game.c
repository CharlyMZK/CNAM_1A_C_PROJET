#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

Board* BOARD;

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
void print_board(){
    for(int i = 0; i < BOARD->size; i++){
      for(int j = 0; j < BOARD->size; j++){
        printf("board[%i][%i] = %i", i, j, (get_stone(i,j)!=NULL)?(get_stone(i,j)->color):(-1));
      }
    }
}

/*
 * Permet de créer un tableau d'une taille défini avec un int passé en paramètre
 */
void create_board(int size){
  BOARD = malloc(sizeof(Board));
	BOARD->size = size;
	BOARD->intersections = malloc(size*size*sizeof(Stone));
}

/*
 * Permet de récupérer la pierre a x et y
 */
Stone* get_stone(int x, int y){
	Stone* stone;
	if(x < BOARD->size && y < BOARD->size)
		stone = BOARD->intersections[x*BOARD->size+y];
	return stone;
}

/*
 * Permet de mettre une pierre à la position à x et y
 */
void set_stone(int x, int y, Stone* stone){
	if(x < BOARD->size && y < BOARD->size)
		BOARD->intersections[x*BOARD->size+y] = stone;
}


/*
 * Permet au joueur dont la couleur est passé en parametre de jouer un caillou
 * Retourne 1 si le jeu a pu jouer, 0 sinon
 */
int play_stone(int x, int y, int color){
	int played = 0;
  Stone* stone = malloc(sizeof(Stone));
  stone->color = color;
	if((played = check_play(x,y)) == 1)
  	set_stone(x, y, stone);
	return played;
}

/*
 * Permet de savoir si le joueur peut jouer
 * Retourne 1 si la pierre peut etre placée sur cette case
 */
int check_play(int x, int y){
	int can_play = 0;
	if(get_stone(x,y) == NULL)
		can_play = 1;
	return can_play;
}
