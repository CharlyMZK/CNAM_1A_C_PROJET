#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "game.h"

Board* BOARD;

int taille_plateau;
int turn = 0;


void player_play(){
	printf("ENtrée");
	color(255,178,102); 
	filled_rectangle(width_win()+5,0,300,30);

	color(0,0,0); 
	if(turn == 0){
		string(width_win()+20,20,"Tour du joueur 1");
	}else{
		string(width_win()+20,20,"Tour du joueur 2");
	}
}


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
	box_size = round(box_size);

	taille_plateau = 32;

	printf("\n box size : %f",box_size);
	// -- Dessin des lignes horizontales
	for(i=0; i<width_win(); i+=box_size)
		line(i,0,i, height_win());

 	// -- Dessin des lignes verticales
	for(i=0; i<height_win(); i+=box_size)
		line(0,i,width_win(),i);	

	
		
}



int test_clicked(int coord,float taille_case){
	float res = 0;
	if(coord < taille_case){
		coord = taille_case;
	}else if(coord > width_win() - taille_case){
		coord = width_win() - taille_case;
	}else if(coord > height_win() - taille_case){
		height_win() - taille_case;
	}
	  
	res = round(coord/taille_case);
	res = res * taille_case;  
	return (int) res;
}

void drop_stone(int x, int y){
	printf("drop");
	if(turn == 0){
		printf("couleur noire ");
		turn = 1;
		color(0,0,0);
	}else{
		printf("couleur blanche ");
		turn = 0;
		color(155,155,155);
	}
	filled_circle(x,y,5);
}

/**
 * on a cliqué a la souris:
 * bouton: 1,2,3,4,5,... : gauche, milieu, droit, molette, ...
 * x,y position
 */
void mouse_clicked(int bouton, int x, int y)
{
	x = test_clicked(x,taille_plateau);
	y = test_clicked(y,taille_plateau);  
	player_play();
	drop_stone(x,y);
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
 */
void play_stone(int x, int y, int color){
  Stone* stone = malloc(sizeof(Stone));
  stone->color = color;
  set_stone(x, y, stone);
}
