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
int pass_counter = 0;

/**
 * Passe le tour du joueur
 *
 */
void pass(){
	pass_counter++;
	if(pass_counter == 2){
		game_finished(); 
	}
	if(turn == 0){
		turn = 1;
		draw_player_turn();  
	}else{ 
		turn = 0;
		draw_player_turn();
	} 
}

/**
 * Check si la partie est terminée
 *
 */
void check_game_finished(){
	int stone_counter = 0;
	bool finished = false;
    for(int i = 0; i < BOARD->size; i++){ 
      for(int j = 0; j < BOARD->size; j++){
		if(get_stone(i,j) != NULL){
			stone_counter++;       
		}  
      } 
    }  
	Stone* checked_stone = NULL;
	if(stone_counter >= 1){
		game_finished();
	}
	printf("\n Dans le jeu il y a %d pierres sur %d",stone_counter,BOARD->size*BOARD->size);   
}

/**
 * Finis la partie 
 *
 */
void game_finished(){
	Stone* checked_stone = NULL;  
	int points_player_1 = 0; 
	int points_player_2 = 0;
	for(int i = 0; i < BOARD->size; i++){ 
		for(int j = 0; j < BOARD->size; j++){
			checked_stone = get_stone(i,j);
			if(checked_stone != NULL){
				if(checked_stone->color == 'B'){
					points_player_1++;
				}else{
					points_player_2++; 
				} 
			}  
		}  
	} 

	// -- Refresh du rectange 
	color(255,178,102); 
	filled_rectangle(width_win()+cell_size+5,50,300,30);
	// -- Marquage du joueur
	color(0,0,0);
	string(width_win()+cell_size+20,70,"La partie est terminee");


	// -- Refresh du rectange 
	color(255,178,102); 
	filled_rectangle(width_win()+cell_size+5,100,300,30);
	// -- Marquage du joueur
	color(0,0,0); 
	if(points_player_1 > points_player_2){
		string(width_win()+cell_size+20,120,"Le joueur 1 gagne");
	}else{
		string(width_win()+cell_size+20,120,"Le joueur 2 gagne");
	}       

	printf("\n Le joueur 1 a %d points",points_player_1);  
	printf("\n Le joueur 2 a %d points",points_player_2);
} 

/**
 * Indique dans la textbox quel joueur joue
 *
 */
void player_play(int x, int y){ 
	int placement_x;
	int placement_y;

	pass_counter = 0;


	if( (x >= cell_size && x <= ( height_win()+cell_size )  ) && ( y >= cell_size && y <= ( height_win()+cell_size )  )  ){

	// -- Placement x et y sur le render
	x = test_clicked(x);
	y = test_clicked(y);
	// -- Placement x et y sur le tableau
	placement_x = (x/cell_size)-1;
	placement_y = (y/cell_size)-1;

	printf("\n===============================PIERRE POSEE  [%d, %d]===============================\n",x,y);

		if(turn == 0){
			if(play_stone(placement_x,placement_y,'B')){
				drop_stone(x,y);
			}
		}else{
			if(play_stone(placement_x,placement_y,'W')){
				drop_stone(x,y);
			}
		}
		draw_player_turn(); 
		check_game_finished();
		printf("\n\n]===============================FIN PIERRE POSEE  [%d, %d] ]===============================\n\n\n\n",x,y);
	}
	printf("Pas de pose de pierre !");
}

/**
 * Affiche le tour du joueur
 */ 
void draw_player_turn(){
	// -- Refresh du rectange  
	color(255,178,102); 
	filled_rectangle(width_win()+cell_size+5,0,300,30);
	// -- Marquage du joueur
	color(0,0,0);
	printf("\n\nTour du joueur : %d\n\n",turn);
	if(turn == 0){
		string(width_win()+cell_size+20,20,"Tour du joueur 1");
	}else{
		string(width_win()+cell_size+20,20,"Tour du joueur 2");
	} 
}

/**
 * Mettre ici son code pour dessiner dans la fenetre
 * 1er affichage + redessine si resize
 */
void draw_win(){
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

	// -- On défini le tour du joueur
	draw_player_turn(); 
	
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
void mouse_clicked(int bouton, int x, int y){
	player_play(x,y);
}

/**
 * on a appuyé sur une touche
 * code: code touche x11 (XK_...)
 * c caractère correspondant si caractere
 * x_souris,y_souris position de la souris
 */
void key_pressed(KeySym code, char c, int x_souris, int y_souris){
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
	Stone* stone = NULL; 
	if(x >= 0 && x < BOARD->size && y >= 0 && y < BOARD->size)
		stone = BOARD->intersections[x*(BOARD->size-1)+y];
	return stone;
}

/*
 * Permet de mettre une pierre à la position à x et y
 */
void set_stone(int x, int y, Stone* stone){
	if(x >= 0 && x < BOARD->size && y >= 0 && y < BOARD->size)
		BOARD->intersections[x*(BOARD->size-1)+y] = stone;
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
 	stone->visible = true;

	// -- Ajout dans les chaines
	add_in_chain(stone);
	modify_freedoms(stone);
	print_chains();
	// -- Affichage des chaines
	//print_chains();

	if((played = check_play(x,y)) == 1){ // on vérifie si le joueur peut jouer
	  	set_stone(x, y, stone); // Sinon hors du tableau
	}

	return played;
}

/* 
 * Affiche le tableau de chaines
 */
void print_chains(){
	printf("\n--------------------------------[PRINT CHAINS]---------------------------------\n");
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
				printf("\n ---------------- [PIERRE] [%c][%d][%d][%d]\n",stoneChecked->color,stoneChecked->x,stoneChecked->y,stoneChecked->visible);
			 }
	}
	printf("\n--------------------------------[PRINT CHAINS END]--------------------------------\n");
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
				CHAINS->chains[chainsCounter]->number_of_freedoms = CHAINS->chains[chainsCounter]->number_of_freedoms + 3;
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
	printf("\n--------------------------------[MODIFY FREEDOM]--------------------------------\n");
	Chain* stone_parameter_chain = find_chain(stone);
	bool check_is_in_same_chain = false;

	// -- CHECK PIERRE A DROITE
	printf("\n -- Je cherche la pierre a droite\n");
	Stone* getStone = get_stone(stone->x+1,stone->y);
	if(getStone != NULL){
		Chain* getChain = find_chain(getStone);

		printf("\nils sont dans la meme chaine ? \n");
		printf("\n%d\n",is_in_same_chain(stone,getStone));

		printf("\nLibertés de sa chaine : %d\n",getChain->number_of_freedoms);
		printf("\n-1\n");
		check_is_in_same_chain = is_in_same_chain(stone,getStone);
		getChain->number_of_freedoms =  getChain->number_of_freedoms - 1;
		if(!check_is_in_same_chain){stone_parameter_chain->number_of_freedoms =  stone_parameter_chain->number_of_freedoms - 1; }

		if(getChain->number_of_freedoms == 0){
			chain_captured(getChain);
		}

		printf("\nNouvelle liberetés de sa chaine : %d\n",getChain->number_of_freedoms);
	}

	// -- CHECK PIERRE A GAUCHE

	printf("\n -- Je cherche la pierre a gauche\n");  
	getStone = get_stone(stone->x-1,stone->y);  
	if(getStone != NULL){     
		printf("la pierre n'est pas nulle"); 
		Chain* getChain = find_chain(getStone);   
		printf("\nLibertés de sa chaine : %d\n",getChain->number_of_freedoms); 
		printf("\n-1\n");
		check_is_in_same_chain = is_in_same_chain(stone,getStone);
		getChain->number_of_freedoms =  getChain->number_of_freedoms - 1;
		if(!check_is_in_same_chain){stone_parameter_chain->number_of_freedoms =  stone_parameter_chain->number_of_freedoms - 1; }

		if(getChain->number_of_freedoms == 0){
			chain_captured(getChain);
		}
		printf("\nNouvelle liberetés de sa chaine : %d\n",getChain->number_of_freedoms);
	}
 
	// -- CHECK PIERRE AU DESSUS
	printf("\n -- Je cherche la pierre au dessus\n");
	getStone = get_stone(stone->x,stone->y-1);
	if(getStone != NULL){
		Chain* getChain = find_chain(getStone);
		printf("\nLibertés de sa chaine : %d\n",getChain->number_of_freedoms);
		printf("\n-1\n");
		check_is_in_same_chain = is_in_same_chain(stone,getStone);
		getChain->number_of_freedoms =  getChain->number_of_freedoms - 1;
		if(!check_is_in_same_chain){stone_parameter_chain->number_of_freedoms =  stone_parameter_chain->number_of_freedoms - 1; }

		if(getChain->number_of_freedoms == 0){
			chain_captured(getChain);
		}
		printf("\nNouvelle liberetés de sa chaine : %d\n",getChain->number_of_freedoms);
	}

	// -- CHECK PIERRE EN DESSOUS
	printf("\n -- Je cherche la pierre en dessous\n");
	getStone = get_stone(stone->x,stone->y+1);
	if(getStone != NULL){
		Chain* getChain = find_chain(getStone);
		printf("\nLibertés de sa chaine : %d\n",getChain->number_of_freedoms);
		printf("\n-1\n");
		check_is_in_same_chain = is_in_same_chain(stone,getStone);
		getChain->number_of_freedoms =  getChain->number_of_freedoms - 1;
		if(!check_is_in_same_chain){stone_parameter_chain->number_of_freedoms =  stone_parameter_chain->number_of_freedoms - 1; };

		if(getChain->number_of_freedoms == 0){
			chain_captured(getChain);
		}
		printf("\nNouvelle liberetés de sa chaine : %d\n",getChain->number_of_freedoms);
	}

	printf("\n--------------------------------[MODIFY FREEDOM END]--------------------------------\n");
}

/*
 * Cherche si la pierre est dans une chaine
 *
 */
Chain* find_chain(Stone* stone){
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

}

/*
 * Gère la capture des chaines
 *
 */
void chain_captured(Chain* chain){
	printf("============================[ CHAIN CAPUTRES ]========================================");
	for(int chainCounter = 0; chainCounter < chain->chain_size; chainCounter++){
		Stone* stone_checked = chain->stones[chainCounter];
		stone_checked->visible = false;
		printf("\nSTONE CHECKED : %d, %d, %d",stone_checked->x,stone_checked->y,stone_checked->visible);
		switch(stone_checked->color){
			case 'W':
				stone_checked->color = 'B';
				break;
			case 'B' :
				stone_checked->color = 'W';
				break;
		}
	}

	redraw_win();
}

 
/*
 * Redessine la fenêtre
 *
 */
void redraw_win(){ 
	draw_win(); 
	Stone* stone = NULL;
	int actualTurn = turn;
	printf("Redraw win : %lu",(BOARD->size*BOARD->size*sizeof(Stone)));
    for(int i = 0; i < BOARD->size; i++){
      for(int j = 0; j < BOARD->size; j++){

		if(get_stone(i,j)!=NULL){
			stone = get_stone(i,j);
			printf("board[%i][%i][%d] = %c", i, j, stone->visible, (get_stone(i,j)!=NULL)?(get_stone(i,j)->color):(' '));
			if(stone->visible == 1){
				if(stone->color == 'B'){
					turn = 0;
				}else{
					turn = 1;
				}
				drop_stone( (i+1) *cell_size, (j+1) *cell_size);
			}
		 }
      }
    }
	actualTurn = turn;

}

 
/*
 * Verifie si la chaine de la stone1 est dans la même chaine que la chaine de la stone2
 *
 */
bool is_in_same_chain(Stone* stone1, Stone* stone2){
	bool is_in_his_chain = false;
	Chain* stone1_chain = find_chain(stone1);
	for(int chainCounter = 0; chainCounter < stone1_chain->chain_size; chainCounter++){
		Stone* stoneChecked = stone1_chain->stones[chainCounter];

		if(stoneChecked->x == stone2->x && stoneChecked->y == stone2->y){ 
			is_in_his_chain = true; 
		} 
	}  
	return is_in_his_chain;  
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
		if(get_stone(stones[stone]->x+1, stones[stone]->y) != NULL
			&& get_stone(stones[stone]->x-1, stones[stone]->y) != NULL
			&& get_stone(stones[stone]->x, stones[stone]->y+1) != NULL
			&& get_stone(stones[stone]->x, stones[stone]->y-1) != NULL )
			// Regarde si la pierre est entouré par une pierre ou qu'elle est sur la limite
			result = 0;
	}
	return result;
}
