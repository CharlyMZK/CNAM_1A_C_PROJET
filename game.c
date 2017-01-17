#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "game.h"

Board* BOARD;

int cell_size;
int turn = 0;
int height_win_spaced;
int width_win_spaced;

/**
 * Indique dans la textbox quel joueur joue
 * 
 */
void player_play(){
	// -- Couleur du rectangle
	color(255,178,102); 
	filled_rectangle(width_win_spaced+5,0,300,30);
	// -- Couleur de l'écriture
	color(0,0,0); 
	if(turn == 0){
		string(width_win_spaced+20,20,"Tour du joueur 1");
	}else{
		string(width_win_spaced+20,20,"Tour du joueur 2");
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
	rectangle(cell_size,cell_size,width_win(),height_win());

 	// -- Calcul du nombre de cases
	box_size = width_win() / board_size;
	box_size = round(box_size);
	
	// -- Taille de la cellule
	cell_size = 32;
	
	// -- Hauteur et largeur du tableau avec les margins
	height_win_spaced = height_win()+cell_size;
	width_win_spaced = width_win()+cell_size;

	// -- Dessin des lignes horizontales
	for(i=cell_size; i<=width_win(); i+=box_size)
		line(i,cell_size,i, height_win_spaced);

 	// -- Dessin des lignes verticales
	for(i=cell_size; i<=height_win(); i+=box_size)
		line(cell_size,i,height_win_spaced,i);	


  	init_board(19);
 	play_stone(5, 13,1);
  	print_board();
}

 
/**
 * Test si le point est posé au milieu d'un carré et le remet correctement a l'intersection
 * 
 */
int test_clicked(int coord){
	float res = 0;
	// -- On fait le check jusqu'a la fin du board, les extremités sont prises en comptes
	height_win_spaced = height_win()+(cell_size*2);
	width_win_spaced = width_win()+(cell_size*2); 
 
	// -- Check pour mettre le point sur une intersection
	if(coord < cell_size){
		coord = cell_size;
	}else if(coord > width_win_spaced - cell_size){
		coord = width_win_spaced - cell_size;
	}else if(coord > height_win_spaced - cell_size){
		height_win_spaced - cell_size;
	}
	  
	// -- Calcul exact 
	res = round(coord/cell_size);
	res = res * cell_size;  
	return (int) res;
}

/**
 * Pose le point de la pierre sur le board
 * 
 */
void drop_stone(int x, int y){
	// -- Couleur du point en fonction du tour du joueur
	if(turn == 0){
		turn = 1;
		color(0,0,0);
	}else{
		turn = 0;
		color(155,155,155);
	}
	// -- Pose le point
	filled_circle(x,y,5);
}

/**
 * on a cliqué a la souris:
 * bouton: 1,2,3,4,5,... : gauche, milieu, droit, molette, ...
 * x,y position
 */
void mouse_clicked(int bouton, int x, int y)
{
	int placement_x;
	int placement_y;
	
	// -- Placement x et y sur le render
	x = test_clicked(x);  
	y = test_clicked(y);  

	// -- Placement x et y sur le tableau
	placement_x = (x/cell_size)-1;
	placement_y = (y/cell_size)-1;  
	play_stone(placement_x, placement_y,1);
	
	printf("\n x : %d \n",placement_x); 
	printf("\n y : %d \n ",placement_y);  
	print_board();
	// -- Render la stone et le tour du joueur
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
void init_board(int size){
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
