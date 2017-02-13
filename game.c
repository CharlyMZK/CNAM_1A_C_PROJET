#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "game.h"

// -- Struct init
Board* BOARD;					// -- Structure du plateau de jeu
Chains* CHAINS;					// -- Structure contenant les chaines crées
Territory* TERRITORY;			// -- Structure contenant les territoires

// -- Params init
bool bot_activated = false; 	// -- Activation du bot ou non
bool game_launched = false;		// -- Jeu lancé ou non ( Si non on affichera les menus )
bool size_picked = false;		// -- Size du board choisie ou non
bool handicap_picked = false;	// -- Handicap choisis ou non
bool mode_picked = false;		// -- Mode de jeu choisis ou non ( J vs Bot ou J V J ou Partie chargée)
int cell_size; 					// -- Taille de la cellule
int turn = 0;					// -- Tour du joueur ( 0 ou 1 )
int pass_counter = 0;			// -- Compteur de tour passé ( si 2 la partie est finie)
int handicap_number = 0;		// -- Nombre de handicap qui a été choisis
char debug_mode = 'c';			// -- Debug mode ( Prints de charly = 'c' Prints de quentin = 'q' )


// --
// -- PLAY FUNCTIONS
// --

/**
 * Indique dans la textbox quel joueur joue
 */
void player_play(int x, int y){
	int placement_x;
	int placement_y;

	pass_counter = 0;

	// -- Check si l'endroit posé est ok
	if( (x >= cell_size && x <= width_win() ) && ( y >= cell_size && y <= height_win() )  ){

		// -- Placement x et y sur le render
		x = test_clicked(x);
		y = test_clicked(y);
		// -- Placement x et y sur le tableau
		placement_x = (x/cell_size)-1;
		placement_y = (y/cell_size)-1;

		if(debug_mode == 'c'){printf("\n===============================PIERRE POSEE  [%d, %d]===============================\n",x,y);}
		if(debug_mode == 'c'){printf("\n TOUR : %d \n",turn);}
		if(turn == 0){
			if(debug_mode == 'c'){printf("[Player play] La pierre jouée est noire");}
			if(play_stone(placement_x,placement_y,'B')){
				turn = 1;
				drop_stone(x,y,'B');
			}
		}else{
			if(debug_mode == 'c'){printf("[Player play] La pierre jouée est blanche");}
			if(play_stone(placement_x,placement_y,'W')){
				turn = 0;
				drop_stone(x,y,'W');
			}
		}
		draw_player_turn(cell_size,turn);
		check_game_finished();
		if(debug_mode == 'c'){printf("\n TOUR : %d \n",turn);
		printf("\n\n===============================FIN PIERRE POSEE  [%d, %d] ]===============================\n\n\n\n",x,y);}
	}
	//printf("Pas de pose de pierre !");

}

/**
 * Le bot joue
 */
void bot_play(){
	if(debug_mode == 'c'){printf("================================= [LE BOT JOUE] ============================");
	printf("\n TOUR : %d \n",turn);}
	int x = 0;
	int y = 0;
	srand(time(NULL)); // initialisation de rand
	x = rand()%(BOARD->size);
	y = rand()%(BOARD->size);

	if(debug_mode == 'c'){printf("\nil pose sur : %d / %d\n",x,y);  }
	if(get_stone(x,y)==NULL){
		if(turn == 0){
			if(play_stone(x,y,'B')){
				turn = 1;
				drop_stone(x,y,'B');
			}
		}else{
			if(play_stone(x,y,'W')){
				turn = 0;
				drop_stone(x,y,'W');
			}
		}
		redraw_win();
	}else{
		bot_play();
		return;
	}
	if(debug_mode == 'c'){printf("\n TOUR : %d \n",turn);
	printf("\n================================= [LE BOT A FINI] ============================\n");}
}

/**
 * Pose la pierre sur le board de façon graphique
 */
 void drop_stone(int x, int y, char stone_color){
	if(stone_color == 'B'){
		color(0,0,0);
		if(debug_mode == 'c'){printf("\n[Drop stone] pose d'une pierre noire, le tour est à 0->1\n");}
	}else{
		color(1.0,1.0,1.0);
		if(debug_mode == 'c'){printf("\n[Drop stone] pose d'une pierre blanche, le tour est à 1->0\n");}
	}
	// -- Pose le point
	filled_circle(x,y,cell_size/3);
}

/*
 * Permet au joueur dont la couleur est passé en parametre de jouer un caillou
 * Retourne true si le joueur a pu jouer, false sinon
 */
bool play_stone(int x, int y, char color){
	bool play = false;
  Stone* stone = malloc(sizeof(Stone));
	stone->color = color;
	stone->x = x;
	stone->y = y;
 	stone->visible = true;
	if(get_stone(x,y) == NULL || !get_stone(x,y)->visible && check_stone_liberties(stone)){ // on vérifie si le joueur peut jouer
		play = true;
  	set_stone(stone);
		// -- Ajout dans les chaines
		add_in_chain(stone);
		if(debug_mode == 'c'){printf("Avant modify freedom \n");}
		modify_freedoms(stone); // -- Modify freedom redessine le plateau en fonction des turns, il est possible qu'il revienne avec un turn pas exact
		if(debug_mode == 'c'){printf("Apres modify freedom \n");}

		// -- Check si la pierre posé peut former un territoire
		do_territory(stone);
	}
	return play;
}

/**
 * Test si le point est posé au milieu d'un carré et le remet correctement a l'intersection
 */
int test_clicked(int coord){
 	float res = (float)coord/cell_size; // on prend la cellule avec une virgule
 	res = round(res) * cell_size; // On arroundi et on multiplie par la taille d'une cellule
 	return (int) res;
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

	if(c == 's'){
		printf("\n tour passé \n");
		save_game(BOARD);
	}
}

/**
 * on a cliqué a la souris:
 * bouton: 1,2,3,4,5,... : gauche, milieu, droit, molette, ...
 * x,y position
 */
void mouse_clicked(int bouton, int x, int y){
	if(debug_mode == 'c'){printf("[MOUSE CLICKED - %d]",turn);}
	// -- Si tout a été pick, on joue
	if(size_picked && mode_picked && handicap_picked && game_launched){
		if(debug_mode == 'c'){printf("[ENTER CLICK PLAY]");}
		player_play(x,y);

		if(bot_activated){bot_play();}

		if(!bot_activated && handicap_picked && handicap_number > 0){
			if(debug_mode == 'c'){printf("[ENTER PASS]");}
			handicap_number--;
			pass();
		}

	}

	// -- Handicap
	if(size_picked && mode_picked && !handicap_picked){
		// -- On met un handicap de moins car les noir jouerons 3 fois puis le blanc jouera
		if(debug_mode == 'c'){printf("[ENTER CLICK HANDICAP]");}
		// -- 6 handicap
		if( (x > 50 && x < 250) && (y>50 && y<80) ){
			handicap_picked = true;
			game_launched = true;
			handicap_number = 5;
			draw_win();
		}

		// -- 3 handicap
		if( (x > 50 && x < 250) && (y>100 && y<130) ){
			handicap_picked = true;
			game_launched = true;
			handicap_number = 2;
			draw_win();

		}

		// -- 0 handicap
		if( (x > 50 && x < 250) && (y>150 && y<180) ){
			handicap_picked = true;
			game_launched = true;
			handicap_number = 0;
			draw_win();
		}

		if(bot_activated && handicap_number > 0){
			for(int i = 0; i < handicap_number+1; i++){
				turn = 0;
				bot_play();
			}
		}

		BOARD->handicap = handicap_number+1;
	}

	// -- Mode de jeu
	if(size_picked && !mode_picked){

		if(debug_mode == 'c'){printf("[ENTER CLICK MODE DE JEU]");}

		// -- Clic boutton jcj
		if( (x > 50 && x < 250) && (y>50 && y<80) ){
			bot_activated = false;
			game_launched = true;
			draw_menu_handicap();
		}

		// -- Clic boutton bot
		if( (x > 50 && x < 250) && (y>100 && y<130) ){
			bot_activated = true;
			game_launched = true;
			draw_menu_handicap();
		}

		// -- Clic boutton charger
		if( (x > 50 && x < 250) && (y>150 && y<180) ){
			if(debug_mode == 'c'){printf("Boutton charger"); }
			draw_menu_handicap();
		}
		mode_picked = true;
	}


	// -- Size du board
	if(!size_picked){
		if(debug_mode == 'c'){ printf("[ENTER CLICK SIZE BOARD]"); }
		if( (x > 50 && x < 250) && (y>50 && y<80) ){
			init_board(19);
			size_picked = true;
		}

		if( (x > 50 && x < 250) && (y>100 && y<130) ){
			init_board(13);
			size_picked = true;
		}

		if( (x > 50 && x < 250) && (y>150 && y<180) ){
			init_board(9);
			size_picked = true;
		}
		if(size_picked){draw_win_menu();}
	}

}

// --
// -- GAME FUNCTIONS
// --


/*
 * Passe le tour du joueur
 */
void pass(){
	pass_counter++;
	if(pass_counter == 2){
		game_finished();
	}
	if(turn == 0){
		turn = 1;
		draw_player_turn(cell_size,turn);
	}else{
		turn = 0;
		draw_player_turn(cell_size,turn);
	}
}

/**
 * Check si la partie est terminée
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
	//printf("\n Dans le jeu il y a %d pierres sur %d",stone_counter,BOARD->size*BOARD->size);
}

/**
 * Finis la partie
 */
void game_finished(){
	Stone* checked_stone = NULL;
	int points_player_1 = 0;
	int points_player_2 = 0;
	for(int i = 0; i < BOARD->size; i++){
		for(int j = 0; j < BOARD->size; j++){
			checked_stone = get_stone(i,j);
			if(checked_stone != NULL){
				if(checked_stone->color == 'B'){					points_player_1++;
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
}

// --
// -- WIN DRAW FUNCTIONS
// --

/**
 * Mettre ici son code pour dessiner dans la fenetre
 * 1er affichage + redessine si resize
 */
void draw_win(){
	// - Vide la fenetre
	clear_win();
	if(debug_mode == 'c'){printf("\n[DRAW WIN]\n"); }
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
	draw_hoshi(BOARD->size,cell_size);

	// -- On défini le tour du joueur
	draw_player_turn(cell_size,turn);

}

/*
 * Redessine la fenêtre
 */
void redraw_win(){
	draw_win();
	Stone* stone = NULL;
  for(int i = 0; i < BOARD->size; i++){
    for(int j = 0; j < BOARD->size; j++){
			if(get_stone(i,j)!=NULL){
				stone = get_stone(i,j);
				printf("\n[Redraw win]board[%i][%i][%d] = %c\n", i, j, stone->visible, (get_stone(i,j)!=NULL)?(get_stone(i,j)->color):(' '));
				if(stone->visible == 1){
					if(stone->color == 'B'){
						drop_stone( (i+1) *cell_size, (j+1) *cell_size, 'B');
					}else{
						drop_stone( (i+1) *cell_size, (j+1) *cell_size, 'W');
					}
				}
			}
    }
  }

}


// --
// -- BOARD FUNCTIONS
// --


/*
 * Permet d'afficher ce qui est présent sur le plateau
 */
void print_board(){
    for(int i = 0; i < BOARD->size; i++){
      for(int j = 0; j < BOARD->size; j++){
        printf("\nboard[%i][%i] = %c", i, j, (get_stone(i,j)!=NULL)?(get_stone(i,j)->color):(' '));
      }
    }
}

/*
 * Permet d'afficher les informations d'une pierre
 */
void print_stone(Stone* stone){
	printf("\nStone : x = %i, y = %i et color = %c\n", stone->x, stone->y, stone->color);
}

/*
 * Permet de créer un tableau d'une taille défini avec un int passé en paramètre
 */
void init_board(int size){
	// -- Initialisation du board
	BOARD = malloc(sizeof(Board));
	BOARD->size = size;
	BOARD->intersections = malloc(size*size*sizeof(Stone));
	BOARD->handicap = 0;
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
		stone = BOARD->intersections[x*BOARD->size+y];
	return stone;
}

/*
 * Permet de mettre une pierre à la position à x et y
 */
void set_stone(Stone* stone){
	if(stone->x >= 0 && stone->x < BOARD->size && stone->y >= 0 && stone->y < BOARD->size)
		BOARD->intersections[stone->x*BOARD->size+stone->y] = stone;
}

// --
// -- CHAINS FUNCTIONS
// --

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
				int number_of_freedoms = 3;
				if(stone->x - 1 < 0){
					number_of_freedoms--;
				}
				if(stone->y - 1 < 0){
					number_of_freedoms--;
				}
				if(stone->x + 1 == BOARD->size){
					number_of_freedoms--;
				}
				if(stone->y + 1 == BOARD->size){
					number_of_freedoms--;
				}
				CHAINS->chains[chainsCounter]->number_of_freedoms = CHAINS->chains[chainsCounter]->number_of_freedoms + number_of_freedoms;
				CHAINS->chains[chainsCounter]->chain_size++;
			 }

			checkx = 0;
   			checky = 0;
	}
	// -- Si la pierre n'a pas été ajoutée nul part on crée une nouvelle chaine
	if(!added){
				int number_of_freedoms = 4;
				if(stone->x - 1 < 0){
					number_of_freedoms--;
				}
				if(stone->y - 1 < 0){
					number_of_freedoms--;
				}
				if(stone->x + 1 == BOARD->size){
					number_of_freedoms--;
				}
				if(stone->y + 1 == BOARD->size){
					number_of_freedoms--;
				}
				Chain* chain = malloc(sizeof(Chain));
				chain->stones = malloc(BOARD->size*BOARD->size*sizeof(Stone));
				chain->chain_size = 0;
				chain->stones[chain->chain_size] = stone;
				chain->number_of_freedoms = number_of_freedoms;
				CHAINS->chains[CHAINS->number_of_chain] = chain;
				CHAINS->number_of_chain++;
				chain->chain_size++;
	}

}

/*
 * Cherche si la pierre est dans une chaine
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
 * Modifie les libertés des chaines autour
 */
void modify_freedoms(Stone* stone){
	//printf("\n--------------------------------[MODIFY FREEDOM]--------------------------------\n");
	Chain* stone_parameter_chain = find_chain(stone);
	bool check_is_in_same_chain = false;

	// -- CHECK PIERRE A DROITE
	printf("\n -- Je cherche la pierre a droite\n");
	Stone* getStone = get_stone(stone->x+1,stone->y);
	if(getStone != NULL && getStone->visible == true){
		Chain* getChain = find_chain(getStone);

		//printf("\nils sont dans la meme chaine ? \n");
		//printf("\n%d\n",is_in_same_chain(stone,getStone));

		//printf("\nLibertés de sa chaine : %d\n",getChain->number_of_freedoms);
		//printf("\n-1\n");
		check_is_in_same_chain = is_in_same_chain(stone,getStone);
		getChain->number_of_freedoms =  getChain->number_of_freedoms - 1;
		if(!check_is_in_same_chain){stone_parameter_chain->number_of_freedoms =  stone_parameter_chain->number_of_freedoms - 1; }

		if(getChain->number_of_freedoms == 0){
			chain_captured(getChain);
		}

		//printf("\nNouvelle liberetés de sa chaine : %d\n",getChain->number_of_freedoms);
	}

	// -- CHECK PIERRE A GAUCHE

	printf("\n -- Je cherche la pierre a gauche\n");
	getStone = get_stone(stone->x-1,stone->y);
	if(getStone != NULL && getStone->visible == true){
		//printf("la pierre n'est pas nulle");
		Chain* getChain = find_chain(getStone);
		//printf("\nLibertés de sa chaine : %d\n",getChain->number_of_freedoms);
		//printf("\n-1\n");
		check_is_in_same_chain = is_in_same_chain(stone,getStone);
		getChain->number_of_freedoms =  getChain->number_of_freedoms - 1;
		if(!check_is_in_same_chain){stone_parameter_chain->number_of_freedoms =  stone_parameter_chain->number_of_freedoms - 1; }

		if(getChain->number_of_freedoms == 0){
			chain_captured(getChain);
		}
		//printf("\nNouvelle liberetés de sa chaine : %d\n",getChain->number_of_freedoms);
	}

	// -- CHECK PIERRE AU DESSUS
	printf("\n -- Je cherche la pierre au dessus\n");
	getStone = get_stone(stone->x,stone->y-1);
	if(getStone != NULL && getStone->visible == true){
		print_chains();
		Chain* getChain = find_chain(getStone);
		//printf("\nLibertés de sa chaine : %d\n",getChain->number_of_freedoms);
		//printf("\n-1\n");
		check_is_in_same_chain = is_in_same_chain(stone,getStone);
		getChain->number_of_freedoms =  getChain->number_of_freedoms - 1;
		if(!check_is_in_same_chain){stone_parameter_chain->number_of_freedoms =  stone_parameter_chain->number_of_freedoms - 1; }

		if(getChain->number_of_freedoms == 0){
			chain_captured(getChain);
		}
		//printf("\nNouvelle liberetés de sa chaine : %d\n",getChain->number_of_freedoms);
	}

	// -- CHECK PIERRE EN DESSOUS
	printf("\n -- Je cherche la pierre en dessous\n");
	getStone = get_stone(stone->x,stone->y+1);
	if(getStone != NULL && getStone->visible == true){
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

	//printf("\n--------------------------------[MODIFY FREEDOM END]--------------------------------\n");
}

/*
 * Gère la capture des chaines
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
		// -- Redonne au pierres autour une libereté pour pas être capturé
		printf("\nJe redonne des libereté, base : %d , %d",stone_checked->x,stone_checked->y);

		Stone* getStone;
		Chain* getChain;
		getStone = get_stone(stone_checked->x+1,stone_checked->y);
		if(getStone != NULL && getStone->visible == true){
			printf("\n a %d , %d ",stone_checked->x+1,stone_checked->y);
			getChain = find_chain(getStone);
			getChain->number_of_freedoms = getChain->number_of_freedoms + 1;
		}
		getStone = get_stone(stone_checked->x-1,stone_checked->y);
		if(getStone != NULL && getStone->visible == true){
			printf("\n a %d , %d ",stone_checked->x-1,stone_checked->y);
			getChain = find_chain(getStone);
			getChain->number_of_freedoms = getChain->number_of_freedoms + 1;
		}
		getStone = get_stone(stone_checked->x,stone_checked->y+1);
		if(getStone != NULL && getStone->visible == true){
			printf("\n a %d , %d ",stone_checked->x,stone_checked->y+1);
			getChain = find_chain(getStone);
			getChain->number_of_freedoms = getChain->number_of_freedoms + 1;
		}
		getStone = get_stone(stone_checked->x,stone_checked->y-1);
		if(getStone != NULL && getStone->visible == true){
			printf("\n a %d , %d ",stone_checked->x,stone_checked->y-1);
			getChain = find_chain(getStone);
			getChain->number_of_freedoms = getChain->number_of_freedoms + 1;
		}
	}

	redraw_win();
}

/*
 * Verifie si la chaine de la stone1 est dans la même chaine que la chaine de la stone2
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
 * Permet de savoir si une pierre a des libertées
 * Retourne true ou false
 */
bool check_stone_liberties(Stone* stone){
	bool result = true;
	if(get_stone(stone->x+1, stone->y) != NULL && get_stone(stone->x+1, stone->y)->color != stone->color
		&& get_stone(stone->x-1, stone->y) != NULL && get_stone(stone->x-1, stone->y)->color != stone->color
		&& get_stone(stone->x, stone->y+1) != NULL && get_stone(stone->x, stone->y+1)->color != stone->color
		&& get_stone(stone->x, stone->y-1) != NULL && get_stone(stone->x, stone->y-1)->color != stone->color){
		// Regarde si la pierre est entouré par une pierre ou qu'elle est sur la limite
		result = false;
	}
	return result;
}

// --
// -- TERRITORY FUNCTIONS
// --

/*
 * Permet de récupérer la pierre a x et y du territoire
 * Retourne NULL s'il n'y pas de pierre, la pierre si elle est bonne et une pierre avec color = "O" si elle est hors case
 */
Stone* get_stone_territory(int x, int y){
	Stone* stone = NULL;
	if(x >= 0 && x < BOARD->size && y >= 0 && y < BOARD->size)
		stone = TERRITORY->stones[x*BOARD->size+y];
	return stone;
}


/*
 * Permet de mettre une pierre à la position x et y dans le territoire
 */
void set_stone_territory(Stone* stone){
	if(stone->x >= 0 && stone->x < BOARD->size && stone->y >= 0 && stone->y < BOARD->size) {
		TERRITORY->stones[stone->x*BOARD->size+stone->y] = stone;
		TERRITORY->stones_in_territory[TERRITORY->size] = stone;
		TERRITORY->size++;
	}
}

/*
 * Permet d'appeler toutes les fonctions liées au territoire
 */
void do_territory(Stone* stone){
	create_territory();
	do_stone_territory(stone);
	free(TERRITORY);
}

/*
 * Permet de savoir si la pierre peut délimiter un territoire et si oui, appele la fonction adéquat
 */
void do_stone_territory(Stone* stone){
	bool found_stone, found_starting_stone = false;
	int number_of_stones = 0;
	int i, j;
	Stone** stones_around = malloc(8*sizeof(Stone)); // Tableau pour les cailloux environnents
	Stone * stone_checked = malloc(sizeof(Stone));
	if(get_stone(stone->x, stone->y-1) != NULL && (get_stone(stone->x, stone->y-1))->color == stone->color){ // En haut
		stones_around[0] = get_stone(stone->x, stone->y-1);
		number_of_stones++;
	}
	if(get_stone(stone->x+1, stone->y-1) != NULL && (get_stone(stone->x+1, stone->y-1))->color == stone->color){ // En haut à droite
		stones_around[1] = get_stone(stone->x+1, stone->y-1);
		number_of_stones++;
	}
	if(get_stone(stone->x+1, stone->y) != NULL && (get_stone(stone->x+1, stone->y))->color == stone->color){ // A Droite
		stones_around[2] = get_stone(stone->x+1, stone->y);
		number_of_stones++;
	}
	if(get_stone(stone->x+1, stone->y+1) != NULL && (get_stone(stone->x+1, stone->y+1))->color == stone->color){ // A Droite en bas
		stones_around[3] = get_stone(stone->x+1, stone->y+1);
		number_of_stones++;
	}
	if(get_stone(stone->x, stone->y+1) != NULL && (get_stone(stone->x, stone->y+1))->color == stone->color){ // En bas
		stones_around[4] = get_stone(stone->x, stone->y+1);
		number_of_stones++;
	}
	if(get_stone(stone->x-1, stone->y+1) != NULL && (get_stone(stone->x-1, stone->y+1))->color == stone->color){ // En bas a gauche
		stones_around[5] = get_stone(stone->x-1, stone->y+1);
		number_of_stones++;
	}
	if(get_stone(stone->x-1, stone->y) != NULL && (get_stone(stone->x-1, stone->y))->color == stone->color){ // A gauche
		stones_around[6] = get_stone(stone->x-1, stone->y);
		number_of_stones++;
	}
	if(get_stone(stone->x-1, stone->y-1) != NULL && (get_stone(stone->x-1, stone->y-1))->color == stone->color){ // A gauche en haut
		stones_around[7] = get_stone(stone->x-1, stone->y-1);
		number_of_stones++;
	}

	if(number_of_stones > 1 || is_on_border(stone) && number_of_stones > 0){
		set_stone_territory(stone);
		adjust_territory(stone);
		for(i = 0; i < 8; i++){ // Pour toutes les pierres du tableau
			stone_checked = stones_around[i];
			if(stone_checked != NULL) { // Si la pierre n'est pas null
				for(j = 0; j < TERRITORY->size; j++){
					if(stone_checked == TERRITORY->stones_in_territory[j]){ // On chercher que la pierre ne soit pas encore dedans
						found_stone = true;
						break;
					}
				}
				printf("\nFound stone x : %i,y : %i, : %i",stone_checked->x, stone_checked->y, found_stone);
				if(!found_stone){ // Si les pierres aux environs ne sont pas dans le tableau
					do_stone_territory(stone_checked);
				}else{ // Si elle l'est deja
					found_stone = false;
					if(stone_checked == TERRITORY->stones_in_territory[0]){ // Si on a retrouvé la premiere pierre
							found_starting_stone = true;
					}
				}
			}
		}
		if(found_starting_stone)
			seek_intersetion_territory(stone->color);
	}
	free(stones_around);
}

/*
 * Permet de savoir si la pierre passé en paramètre se situe sur le bord du plateau
 * Retourne true si c'est sur la bord
 */
bool is_on_border(Stone* stone){
	bool result = false;
	if(stone->x == 0 || stone->x == BOARD->size-1 || stone->y == 0 || stone->y == BOARD->size-1)
		result = true;
	return result;
}

/*
 *  Permet de créer un territoire
 */
void create_territory(){
	TERRITORY = malloc(sizeof(Territory));
	TERRITORY->size = 0;
	TERRITORY->min_x = BOARD->size-1;
	TERRITORY->min_y = BOARD->size-1;
	TERRITORY->max_x = 0;
	TERRITORY->max_y = 0;
	TERRITORY->stones = malloc(BOARD->size*BOARD->size*sizeof(Stone));
	TERRITORY->stones_in_territory = malloc(BOARD->size*BOARD->size*sizeof(Stone));
}

/*
 * Permet d'ajuste les x et y du territoire
 */
void adjust_territory(Stone* stone){
	if(TERRITORY->min_x > stone->x)
		TERRITORY->min_x = stone->x;
	if(TERRITORY->max_x < stone->x)
		TERRITORY->max_x = stone->x;
	if(TERRITORY->min_y > stone->y)
		TERRITORY->min_y = stone->y;
	if(TERRITORY->max_y < stone->y)
		TERRITORY->max_y = stone->y;
}

/*
 * Permet de remplir les lignes et colonnes pour ensuite mettre les pierres sur la board
 */
void seek_intersetion_territory(int color){
	Lines* lines = malloc(sizeof(Lines));
	lines->size = 0;
	lines->lines = malloc(BOARD->size*sizeof(Line));
	lines->color = color;
	Columns* columns = malloc(sizeof(Column));
	columns->size = 0;
	columns->columns = malloc(BOARD->size*sizeof(Column));
	columns->color = color;

	int i,j ;
	Stone* stone_checked = malloc(sizeof(Stone));
	Line* line = NULL;
	Column* column = NULL;
	for(i = TERRITORY->min_y; i <= TERRITORY->max_y; i++){ // Pour tout le territoire, on cherche quelles sont les lignes où l'on doit mettre des pierres
		line = init_line();
		line->y = i;
		printf("Territory : x: min %d max %d y : min %d max %d", TERRITORY->min_x, TERRITORY->max_x, TERRITORY->min_y, TERRITORY->max_y);
		for(j = TERRITORY->min_x; j <= TERRITORY->max_x; j++){
			stone_checked = get_stone_territory(j,i);
			if(stone_checked != NULL && stone_checked->color == lines->color){ // Si la pierre n'est pas nul et qu'elle est de la bonne couleur
				print_stone(stone_checked);
				if(line->min_x > stone_checked->x)
					line->min_x = stone_checked->x;
				if(line->max_x < stone_checked->x)
					line->max_x = stone_checked->x;
			}
			else if(j == 0) //  Ou que l'on est sur un bord
				line->min_x = 0;
			else if(j == BOARD->size-1)
				line->max_x = BOARD->size-1;
		}
		if(line != NULL) {
			lines->lines[lines->size] = line;
			lines->size++;
		}
	}
	for(i = TERRITORY->min_x; i <= TERRITORY->max_x; i++){ // Pour tout le territoire, on cherche quelles sont les colonnes où l'on doit mettre des pierres
		column = init_column(column);
		column->x = i;
		for(j = TERRITORY->min_y; j <= TERRITORY->max_y; j++){
			stone_checked = get_stone_territory(i,j);
			if(stone_checked != NULL && stone_checked->color == columns->color){
				if(column->min_y > stone_checked->y)
					column->min_y = stone_checked->y;
				if(column->max_y < stone_checked->y)
					column->max_y = stone_checked->y;
			}
			else if(j == 0)
				column->min_y = 0;
			else if(j == BOARD->size-1)
				column->max_y = BOARD->size-1;
		}
		if(column != NULL){
			columns->columns[columns->size] = column;
			columns->size++;
		}
	}
	if(lines->size > 0 && columns->size > 0)
		fill_board(lines, columns);
}

/*
* Permet de remplir une ligne de pierre invisible dans le tableau du plateau
*/
void fill_board(Lines* lines, Columns* columns){
	int i,j,k;
	Stone* stone_to_place = malloc(sizeof(Stone));
	stone_to_place->color = lines->color;
	stone_to_place->visible = false;
	for(i = 0; i < lines->size; i++){
		for(j = lines->lines[i]->min_x; j <= lines->lines[i]->max_x; j++){
			for(k = 0; k < columns->size; k++){
				if(columns->columns[k]->x == j){
					if(lines->lines[i]->y >= columns->columns[k]->min_y && lines->lines[i]->y <= columns->columns[k]->max_y){ // Si la line et la colonne sont dans le territoire
						if(get_stone(j, lines->lines[i]->y) == NULL /*|| get_stone(j, lines->lines[i]->y)->visible == false*/){ // S'il n'y a pas de pierre, ou que la pierre est invisible ou blanche
							stone_to_place->x = j;
							stone_to_place->y = lines->lines[i]->y;
							set_stone(stone_to_place);
						}
					}
				}
			}
		}
	}
	print_board();
}

/*
 * Permet de remettre à 0 toutes la variables de la ligne passé en paramètre
 */
Line* init_line(){
	Line* line = malloc(sizeof(Line));
	if(line != NULL){
		line->min_x = BOARD->size-1;
		line->max_x = 0;
		line->y = 0;
	}
	return line;
}

/*
 * Permet de remettre à 0 toutes la variables de la colonne passé en paramètre
 */
Column* init_column(){
	Column* column = malloc(sizeof(Column));
	if(column != NULL){
		column->x = 0;
		column->min_y = BOARD->size-1;
		column->max_y = 0;
	}
	return column;
}


FILE* FILE_GAME;

///////////////////// FILE_GAME SAVER /////////////////////

/*
 * Permet de sauvegarder la partie dans un fichier
 */
void save_game(Board* board){
  create_header(board->size, board->handicap);
  write_game(board);
  fclose(FILE_GAME);
}

/*
 * Permet de crééer le fichier et de mettre l'entête dedans
 */
void create_header(int board_size, int handicap){
	time_t timet = time(NULL);
	struct tm time_struct = *localtime(&timet); // Pour avoir la date du jour
  FILE_GAME = fopen("score.sgf", "w"); // Recreer un fichier et écris de dans
  fprintf(FILE_GAME, "(;FF[4]GM[1]SZ[%i]\n\n", board_size);
  fprintf(FILE_GAME, "GN[Go Game Mraled]\n");
  fprintf(FILE_GAME, "PB[Black]\n");
  fprintf(FILE_GAME, "HA[%i]\n", handicap);
  fprintf(FILE_GAME, "PW[White]\n");
  fprintf(FILE_GAME, "KM[%0.1f]\n", 7.5+handicap);
  fprintf(FILE_GAME, "DT[%d-%d-%d]\n", time_struct.tm_year + 1900, time_struct.tm_mon + 1, time_struct.tm_mday);
  fprintf(FILE_GAME, "TM[1800]\n"); // Non renseigner
  fprintf(FILE_GAME, "RU[Japanese]\n\n");
}

/*
 * Permet d'écrire la partie dans le fichier
 */
void write_game(Board* board){
  int first_time = true; // Premiere fois qu'on mets une pierre de cette couleur
	bool white_stones = false; // Permet de savoir s'il y a des pierres blanches
  Stone * stone_to_save = malloc(sizeof(Stone));
  fprintf(FILE_GAME, ";");
  for(int i = 0; i < board->size; i++){
    for(int j = 0; j < board->size; j++){ // Mets les pierres blanches
      stone_to_save = get_stone(i,j);
      if(stone_to_save != NULL && stone_to_save->color == 'W'){
        if(first_time) {
          fprintf(FILE_GAME, "AW");
					first_time = false;
					white_stones = true;
				}
        fprintf(FILE_GAME, "[%c%c]", stone_to_save->x + 'a', stone_to_save->y + 'a');
      }
    }
  }
  first_time = true;
	if(white_stones)
		fprintf(FILE_GAME, "\n");
  for(int i = 0; i < board->size; i++){
    for(int j = 0; j < board->size; j++){ // Mets les pierres noires
      stone_to_save = get_stone(i,j);
      if(stone_to_save != NULL && stone_to_save->color == 'B'){
        if(first_time) {
          fprintf(FILE_GAME, "AB");
					first_time = false;
				}
        fprintf(FILE_GAME, "[%c%c]", stone_to_save->x + 'a', stone_to_save->y + 'a');
      }
    }
  }
  fprintf(FILE_GAME,")");
}

///////////////////// FILE_GAME IMPORTER /////////////////////
/*
 * Permet d'importer un fichier
 */
void import_FILE_GAME(char* FILE_GAME_name, Board* board){
  /*FILE_GAME = fopen(FILE_GAME_name, "r");
  if(FILE_GAME != NULL){
    import_game(board);
    fclose(FILE_GAME);
  } else {
    printf("Fichier non ouvert, nom ou chemin incorrect");
  }*/
}

/*
 * Permet d'importer les pions sur le board
 */
void import_game(Board* board){
  /*int character; // Le caractère à prendre
  while((character = fgetc(fp)) != EOF)
    printf("%c",character);*/
}
