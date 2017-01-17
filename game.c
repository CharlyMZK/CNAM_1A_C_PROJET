#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "game.h"
#include "dessine.h"

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

