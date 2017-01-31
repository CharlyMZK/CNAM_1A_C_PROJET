#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "game.h"

Board* BOARD;

int cell_size;
int turn = 0;

void pass(){
	if(turn = 0){
		turn = 1;
	}else{
		turn = 0;
	}
}

int find_chain(int x, int y, int size, Stone** stones){
	/*printf("\nje recupère : %d , %d \n",x,y);
	Stone* stone = get_stone(x,y);
	int equals = 0;
	int i;
	for(i = 0 ; i < size ; i++){
		if( ( stones[i]->x == stone->x ) && (stones[i]->y == stone->y ) ) {
			equals = 1;
		}
	}*/



	/*if(stone != NULL){
		printf("\nje retourne 1 \n");
		check_capture(x+1,y);
		check_capture(x,y+1);
		check_capture(x-1,y);
		check_capture(x,y-1);
		return 1;
	}else{
		printf("\nje retourne 0 \n");
		return 0;
	}	*/


}

/**
 * Indique dans la textbox quel joueur joue
 *
 */

void player_play(int x, int y){
	int placement_x;
	int placement_y;

	// -- Placement x et y sur le render
	x = test_clicked(x);
	y = test_clicked(y);
	// -- Placement x et y sur le tableau
	placement_x = (x/cell_size)-1;
	placement_y = (y/cell_size)-1;

	if(turn == 0){
		printf("Joue une pierre : %d, %d\n",x,y);
		if(play_stone(placement_x,placement_y,'W')){
			// -- Refresh du rectange
			color(255,178,102);
			filled_rectangle(width_win()+cell_size+5,0,300,30);
			// -- Marquage du joueur
			color(0,0,0);
			string(width_win()+cell_size+20,20,"Tour du joueur 1");
			drop_stone(x,y);
		}

	}else{
		if(play_stone(placement_x,placement_y,'B')){
			// -- Refresh du rectangle
			color(255,178,102);
			filled_rectangle(width_win()+cell_size+5,0,300,30);
			// -- Marquage du joueur
			color(0,0,0);
			string(width_win()+cell_size+20,20,"Tour du joueur 2");
			drop_stone(x,y);
		}

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
	int i;

	// -- Lines color
	color(0,0,0);

	// -- Taille de la cellule
	cell_size = round(height_win() / BOARD->size);

	// -- Dessin des lignes horizontales
	for(i=cell_size; i<=cell_size*BOARD->size; i+=cell_size) // On prend la taille d'une cellule - la taille du board - 1 car sinon on a une case de trop
		line(i,cell_size,i, width_win());

 	// -- Dessin des lignes verticales
	for(i=cell_size; i<=cell_size*BOARD->size; i+=cell_size)// On prend la taille d'une cellule - la taille du board - 1 car sinon on a une case de trop
		line(cell_size,i,height_win(),i);

	// On dessine les hoshis
	draw_hoshi();
	init_board(BOARD->size);
}


/**
 * Test si le point est posé au milieu d'un carré et le remet correctement a l'intersection
 *
 */
 int test_clicked(int coord){
 	float res = (float)coord/cell_size; // on prend la cellule avec une virgule
 	res = round(res) * cell_size; // On arroundi et on multiplie par la taille d'une cellule
 	return (int) res;
 }

 /*
  * Permet de dessiner les hoshis sur le plateau
  */
 void draw_hoshi()
 {
 	int i,j;
	color(0,0,0);
	switch(BOARD->size){
		case 9:
			for(i=2; i <= 6; i+=4){
				for(j=2; j <= 6; j+=4){
					int x = cell_size + (j * cell_size);
					int y = cell_size + (i * cell_size);
					filled_circle(x,y,cell_size/6);
				}
			}
			break;
		case 13:
			for(i=3; i <= 9; i+=3){
				for(j=3; j <= 9; j+=3){
					int x = cell_size + (j * cell_size);
					int y = cell_size + (i * cell_size);
					filled_circle(x,y,cell_size/6);
				}
			}
		break;
		case 19:
			for(i=3; i <= 15; i+=6){
				for(j=3; j <= 15; j+=6){
					int x = cell_size + (j * cell_size);
					int y = cell_size + (i * cell_size);
					filled_circle(x,y,cell_size/6);
				}
			}
			break;
	}
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
		color(1.0,1.0,1.0);
	}
	// -- Pose le point
	filled_circle(x,y,cell_size/3);
}

/**
 * on a cliqué a la souris:
 * bouton: 1,2,3,4,5,... : gauche, milieu, droit, molette, ...
 * x,y position
 */
void mouse_clicked(int bouton, int x, int y)
{
	player_play(x,y);
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

	if(c == 'p'){
		printf("tour passé");
		pass();
	}
}

/*
 * Permet d'afficher ce qui est présent sur le plateau
 */
void print_board(){
    for(int i = 0; i < BOARD->size; i++){
      for(int j = 0; j < BOARD->size; j++){
        printf("board[%i][%i] = %c", i, j, (get_stone(i,j)!=NULL)?(get_stone(i,j)->color):(' '));
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
 * Retourne NULL s'il n'y pas de pierre, la pierre si elle est bonne et une pierre avec color = "O" si elle est hors case
 */
Stone* get_stone(int x, int y){
	Stone* stone;
	if(x < BOARD->size && y < BOARD->size)
		stone = BOARD->intersections[x*BOARD->size+y];
	else{
		stone = malloc(sizeof(Stone));
		stone->color = 'O';
	}
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
 * Permet au joueur blanc de jouer une pierre
 * Retourne 1 si le jeu a pu jouer, 0 sinon
 */
int play_white_stone(int x, int y){
	return play_stone(x, y, 'W');
}

/*
 * Permet au joueur noir de placer une pierre
 * Retourne 1 si le jeu a pu jouer, 0 sinon
 */
int play_black_stone(int x, int y){
	return play_stone(x, y, 'B');
}

/*
 * Permet au joueur dont la couleur est passé en parametre de jouer un caillou
 * Retourne 1 si le jeu a pu jouer, 0 sinon
 */
int play_stone(int x, int y, char color){
	int played = 0;
  Stone* stone = malloc(sizeof(Stone));
	stone->color = color;
	stone->x = x;
	stone->y = y;

	if((played = check_play(x,y)) == 1) // on vérifie si le joueur peut jouer
  	set_stone(x, y, stone);
	return played;
}

/*
 * Permet de savoir si le joueur peut jouer
 * Retourne 1 si la pierre peut etre placée sur cette case
 */
int check_play(int x, int y){
	int can_play = 0;
	if(get_stone(x,y) == NULL) // s'il n'y a pas de pierre sur la case
		can_play = 1;
	return can_play;
}

/*
 * Permet de savoir s'il reste des libertés a la chaine
 * Retourne 1 si oui, non sinon
 */
int check_chain_liberties(int size, Stone** stones){
	int result = 1;
	for(int stone = 0; stone < size; stone++){
		if(get_stone(stones[stone]->x+1, stones[stone]->y) != NULL || get_stone(stones[stone]->x+1, stones[stone]->y)->color == 'O'
			&& get_stone(stones[stone]->x-1, stones[stone]->y) != NULL || get_stone(stones[stone]->x-1, stones[stone]->y)->color == 'O'
			&& get_stone(stones[stone]->x, stones[stone]->y+1) != NULL || get_stone(stones[stone]->x, stones[stone]->y+1)->color == 'O'
			&& get_stone(stones[stone]->x, stones[stone]->y-1) != NULL || get_stone(stones[stone]->x, stones[stone]->y-1)->color == 'O')
			// Regarde si la pierre est entouré par une pierre ou qu'elle est sur la limite
			result = 0;
	}
	return result;
}
