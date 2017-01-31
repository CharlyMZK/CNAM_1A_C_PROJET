#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "game.h"

Board* BOARD;
Chains* CHAINS;

int cell_size;
int turn = 0;

void pass(){
	if(turn = 0){
		turn = 1;
	}else{
		turn = 0;
	}
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
		printf("\n tour passé \n");
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
	// -- Initialisation du board
  	BOARD = malloc(sizeof(Board));
	BOARD->size = size;
	BOARD->intersections = malloc(size*size*sizeof(Stone));
	// -- Initialisation des chaines
	CHAINS = malloc(sizeof(Chains)); 
	CHAINS->number_of_chain = 0;  
	CHAINS->chains = malloc(size*size*sizeof(Stone));

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
 
	
	// -- Ajout dans les chaines
	add_in_chain(stone);  
	modify_freedoms(stone);
	print_chains(); 
	// -- Affichage des chaines
	//print_chains();   
 
	if((played = check_play(x,y)) == 1){ // on vérifie si le joueur peut jouer
	  	set_stone(x, y, stone);
	}
 
	return played;
}

/*
 * Affiche le tableau de chaines
 */
void print_chains(){ 
	printf("\n-------------------------------[PRINT CHAINS]--------------------------------------------\n");   
	// -- On boucle sur la liste des chaines
	for(int chainsCounter = 0; chainsCounter < CHAINS->number_of_chain; chainsCounter++){
		    printf("\n[CHAINES] Compteur de chains : %d\n",chainsCounter); 
			// -- On récupère la chaine
			Chain* chain = CHAINS->chains[chainsCounter];
			printf("\n ----- [CHAINE] Chaine freedoms : %d",chain->number_of_freedoms);  
			 // -- On boucle sur les pierres de la chaine 
			 for(int chainCounter = 0; chainCounter < chain->chain_size; chainCounter++){
				Stone* stoneChecked = chain->stones[chainCounter];
				printf("\n ---------------- [PIERRE] Compteur de pierres : %d\n",chainsCounter); 
				printf("\n ---------------- [PIERRE] [%c][%d][%d]\n",stoneChecked->color,stoneChecked->x,stoneChecked->y); 
			 }
	}
	printf("\n-----------------------------------[PRINT CHAINS END]----------------------------------------\n");   
}

/*
 * Ajoute une pierre dans le tableau de chaines
 * 
 */
int add_in_chain(Stone* stone){
	int checkx = 0;
	int checky = 0; 
	bool added = false; 
	
	// -- On boucle sur la liste des chaines
	for(int chainsCounter = 0; chainsCounter < CHAINS->number_of_chain; chainsCounter++){
	 
			// -- On récupère la chaine
			Chain* chain = CHAINS->chains[chainsCounter];
			 // -- On boucle sur les pierres de la chaine 
			 for(int chainCounter = 0; chainCounter < chain->chain_size; chainCounter++){
				Stone* stoneChecked = chain->stones[chainCounter];
				// -- Check il est dedans
				if(stoneChecked->color == stone->color){

					// -- Check des x
					if(stoneChecked->x-1 == stone->x && stoneChecked->y == stone->y){
						checkx = 1;
					}
					if(stoneChecked->x+1 == stone->x && stoneChecked->y == stone->y){
						checkx = 1;
					}  

					// -- Check des y
					if(stoneChecked->y-1 == stone->y && stoneChecked->x == stone->x){
						checky = 1;
					} 
					if(stoneChecked->y+1 == stone->y && stoneChecked->x == stone->x){
						checky = 1;
					}   
				}

			 }    
			// -- Si checkx et checky = 1 c'est que une pierre autour est de la meme couleur, on l'ajoute donc a sa chaine
			 if(checkx != 0 || checky != 0){ 
				added = true; 
				CHAINS->chains[chainsCounter]->stones[chain->chain_size] = stone; 
				CHAINS->chains[chainsCounter]->number_of_freedoms = CHAINS->chains[chainsCounter]->number_of_freedoms + 2;  
				CHAINS->chains[chainsCounter]->chain_size++;  
			 }
 
			checkx = 0;
   			checky = 0;    
	} 
	// -- Si la pierre n'a pas été ajoutée nul part on crée une nouvelle chaine
	if(!added){ 
				Chain* chain = malloc(sizeof(Chain));
				chain->stones = malloc(BOARD->size*BOARD->size*sizeof(Stone));
				chain->chain_size = 0;       
				chain->stones[chain->chain_size] = stone; 
				chain->number_of_freedoms = 4; 
				CHAINS->chains[CHAINS->number_of_chain] = chain;
				CHAINS->number_of_chain++;
				chain->chain_size++;
	}

}

/*
 * Modifie les libertés des chaines autour  
 * 
 */
void modify_freedoms(Stone* stone){ 
	printf("\n-----------------------------------[MODIFY FREEDOM]----------------------------------------\n");
	printf("\n -- Je cherche la pierre a droite\n");  
	Stone* getStone = get_stone(stone->x+1,stone->y); 
	if(getStone != NULL){ 
		Chain* getChain = find_chain(getStone); 
		printf("\nLibertés de sa chaine : %d\n",getChain->number_of_freedoms); 
		printf("\n-1\n");
		getChain->number_of_freedoms =  getChain->number_of_freedoms - 1; 
		printf("\nNouvelle liberetés de sa chaine : %d\n",getChain->number_of_freedoms); 
	}

	printf("\n -- Je cherche la pierre a gauche\n");  
	getStone = get_stone(stone->x-1,stone->y); 
	if(getStone != NULL){ 
		Chain* getChain = find_chain(getStone); 
		printf("\nLibertés de sa chaine : %d\n",getChain->number_of_freedoms); 
		printf("\n-1\n");
		getChain->number_of_freedoms =  getChain->number_of_freedoms - 1; 
		printf("\nNouvelle liberetés de sa chaine : %d\n",getChain->number_of_freedoms); 
	}  

		printf("\n -- Je cherche la pierre au dessus\n");  
	getStone = get_stone(stone->x,stone->y-1); 
	if(getStone != NULL){ 
		Chain* getChain = find_chain(getStone); 
		printf("\nLibertés de sa chaine : %d\n",getChain->number_of_freedoms); 
		printf("\n-1\n");
		getChain->number_of_freedoms =  getChain->number_of_freedoms - 1; 
		printf("\nNouvelle liberetés de sa chaine : %d\n",getChain->number_of_freedoms); 
	}  

		printf("\n -- Je cherche la pierre en dessous\n");  
	getStone = get_stone(stone->x+1,stone->y+1); 
	if(getStone != NULL){  
		Chain* getChain = find_chain(getStone); 
		printf("\nLibertés de sa chaine : %d\n",getChain->number_of_freedoms); 
		printf("\n-1\n");
		getChain->number_of_freedoms =  getChain->number_of_freedoms - 1; 
		printf("\nNouvelle liberetés de sa chaine : %d\n",getChain->number_of_freedoms); 
	}   

	printf("\n-----------------------------------[MODIFY FREEDOM END]----------------------------------------\n");
}
/*
 * Cherche si la pierre est dans une chaine 
 * 
 */
Chain* find_chain(Stone* stone){   
	printf("\n-----------------------------[ FIND IN CHAIN ]----------------------------------------------\n");   
	// -- On boucle sur la liste des chaines
	Chain* returnedChain = NULL;
	for(int chainsCounter = 0; chainsCounter < CHAINS->number_of_chain; chainsCounter++){
			// -- On récupère la chaine
			Chain* chain = CHAINS->chains[chainsCounter];
			 // -- On boucle sur les pierres de la chaine 
			 for(int chainCounter = 0; chainCounter < chain->chain_size; chainCounter++){
				Stone* stoneChecked = chain->stones[chainCounter];
				if(stoneChecked->x == stone->x && stoneChecked->y == stone->y){ 
					returnedChain = chain; 
				}
			 } 
	}
	return returnedChain;
	printf("\n----------------------------[ FIND IN CHAIN END ]-----------------------------------------------\n");   
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
