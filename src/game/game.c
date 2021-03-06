#include <stdio.h>
#include <stdlib.h>
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
bool loaded_game = false; 		// -- Une partie est chargée ou non
int cell_size; 					// -- Taille de la cellule
int turn = 0;					// -- Tour du joueur ( 0 ou 1 )
int pass_counter = 0;			// -- Compteur de tour passé ( si 2 la partie est finie)
int handicap_number = 0;		// -- Nombre de handicap qui a été choisis
char debug_mode = 'c';			// -- Debug mode ( Prints de charly = 'c' Prints de quentin = 'q' )
char* date;
bool game_is_finished = false;

// --
// -- PLAY FUNCTIONS
// --

/**
 * Indique dans la textbox quel joueur joue
 * Retourne true si un joueur à joué
 */
bool player_play(int x, int y){
	bool play = false;
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
			if(debug_mode == 'c'){printf("[Player play] La pierre jouée est noir");}
			if(play_stone(placement_x,placement_y,'B')){
				play = true;
				turn = 1;
				drop_stone(x,y,'B');
			}
		}else{
			if(debug_mode == 'c'){printf("[Player play] La pierre jouée est blanche");}
			if(play_stone(placement_x,placement_y,'W')){
				play = true;
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
	return play;
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
		if(debug_mode == 'c'){printf("\n[Drop stone] pose d'une pierre noir, le tour est à 0->1\n");}
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
	if(get_stone(x,y) == NULL || !get_stone(x,y)->visible && ( check_stone_liberties(stone) || check_around_captured(stone) )) { // on vérifie si le joueur peut jouer
		play = true;
  		set_stone(stone);
		// -- Ajout dans les chaines
		add_in_chain(stone);
		//if(debug_mode == 'c'){printf("Avant modify freedom \n");}
		modify_freedoms(stone); // -- Modify freedom redessine le plateau en fonction des turns, il est possible qu'il revienne avec un turn pas exact
		//if(debug_mode == 'c'){printf("Apres modify freedom \n");}

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
	printf("\nSELECTED\n");


	if(debug_mode == 'c'){printf("[MOUSE CLICKED - %d]",turn);}
	// -- Si tout a été pick, on joue
	if(size_picked && mode_picked && handicap_picked && game_launched){
		if(debug_mode == 'c'){printf("[ENTER CLICK PLAY]");}
		if(player_play(x,y)){
			if(bot_activated){
				bot_play();
			}
			if(!bot_activated && handicap_picked && handicap_number > 0){
				if(debug_mode == 'c'){printf("[ENTER PASS]");}
				handicap_number--;
				pass();
			}
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

	// -- Size du board
	if(mode_picked && !size_picked){
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
		if(size_picked){draw_menu_handicap();}
	}

	// -- Mode de jeu
	if(!mode_picked){

		if(debug_mode == 'c'){printf("[ENTER CLICK MODE DE JEU %d,%d]",x,y);}

		// -- Clic boutton jcj
		if( (x > 50 && x < 250) && (y>50 && y<80) ){
			bot_activated = false;
			game_launched = true;
			mode_picked = true;
			draw_win_board_size();
		}

		// -- Clic boutton bot
		if( (x > 50 && x < 250) && (y>100 && y<130) ){
			bot_activated = true;
			game_launched = true;
			mode_picked = true;
			draw_win_board_size();
		}

		// -- Clic boutton charger
		if( (x > 50 && x < 250) && (y>150 && y<180) ){
			if(debug_mode == 'c'){printf("Boutton charger"); }
			draw_win();
			import_file("score.sgf");
		}
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
		game_is_finished = true;
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
	if(stone_counter >= ( BOARD->size * BOARD->size ) ) {
		game_is_finished = true;
		
	}  
	game_finished(); 
}

/**
 * Finit la partie
 */
void game_finished(){
	char display[50];
	float score[2];
	calculate_score(score);

 
	
	if(game_is_finished){
		// -- Refresh du rectange
		color(255,178,102);
		filled_rectangle(width_win()+cell_size+5,50,300,30);

		// -- Marquage du joueur
		color(0,0,0);
		string(width_win()+cell_size+20,70,"La partie est terminee");
	}

	// -- Refresh du rectange
	color(255,178,102);
	filled_rectangle(width_win()+cell_size+5,100,300,30);
	// -- Marquage du joueur
	color(0,0,0);
	if(score[0] > score[1]){
		sprintf(display,"Noir gagne : %g points contre %g", score[0], score[1]);
	} else{
		sprintf(display,"Blanc gagne : %g points contre %g", score[1], score[0]);
	}
	string(width_win()+cell_size+20,120,display);
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
	float score[2];
	char display_date[30];
	char display_won[50];

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
	if(!loaded_game){
		draw_player_turn(cell_size,turn);
	}else{
		calculate_score(score);
		sprintf(display_date, "Enregistre le : %s", date); // Formatage de la chaine de date
		if(score[0] > score[1])
			sprintf(display_won, "Noir a gagne : %g contre %g", score[0], score[1]);
		else
			sprintf(display_won, "Blanc a gagne : %g contre %g", score[1], score[0]);
		draw_loaded_game(cell_size,turn,display_date, display_won);
	}

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
	//printf("\nStone : x = %i, y = %i et color = %c\n", stone->x, stone->y, stone->color);
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
			 for(int chainCounter = 0; chainCounter < chain->size; chainCounter++){
				Stone* stone_checked = chain->stones[chainCounter];
				printf("\n ---------------- [PIERRE] Compteur de pierres : %d\n",chainsCounter);
				printf("\n ---------------- [PIERRE] [%c][%d][%d][%d]\n",stone_checked->color,stone_checked->x,stone_checked->y,stone_checked->visible);
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
			 for(int chainCounter = 0; chainCounter < chain->size; chainCounter++){
				Stone* stone_checked = chain->stones[chainCounter];
				// -- Check il est dedans
				if(stone_checked->color == stone->color){
					// -- Check des x
					if(stone_checked->x-1 == stone->x && stone_checked->y == stone->y){
						checkx = 1;
					}
					if(stone_checked->x+1 == stone->x && stone_checked->y == stone->y){
						checkx = 1;
					}
					// -- Check des y
					if(stone_checked->y-1 == stone->y && stone_checked->x == stone->x){
						checky = 1;
					}
					if(stone_checked->y+1 == stone->y && stone_checked->x == stone->x){
						checky = 1;
					}
				}
			 }
			// -- Si checkx et checky = 1 c'est que une pierre autour est de la meme couleur, on l'ajoute donc a sa chaine
			 if(checkx != 0 || checky != 0){
				added = true;
				CHAINS->chains[chainsCounter]->stones[chain->size] = stone;
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
				CHAINS->chains[chainsCounter]->size++;
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
				chain->size = 0;
				chain->stones[chain->size] = stone;
				chain->number_of_freedoms = number_of_freedoms;
				CHAINS->chains[CHAINS->number_of_chain] = chain;
				CHAINS->number_of_chain++;
				chain->size++;
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
			 for(int chainCounter = 0; chainCounter < chain->size; chainCounter++){
				Stone* stone_checked = chain->stones[chainCounter];
				if(stone_checked->x == stone->x && stone_checked->y == stone->y){
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
	check_capture(stone, get_stone(stone->x+1,stone->y));
	check_capture(stone, get_stone(stone->x-1,stone->y));
	check_capture(stone, get_stone(stone->x,stone->y-1));
	check_capture(stone, get_stone(stone->x,stone->y+1));
}

/*
 * Permet de savoir s'il faut capturer une pierre ou non
 */
void check_capture(Stone* stone, Stone* stone_around) {
	bool check_is_in_same_chain;
	Chain* stone_around_chain;
	Chain* chain_stone;
	if(stone_around != NULL && stone_around->visible == true){
		check_is_in_same_chain = false;
		stone_around_chain = find_chain(stone_around);
		chain_stone = find_chain(stone);
		check_is_in_same_chain = is_in_same_chain(stone,stone_around);
		chain_stone->number_of_freedoms =  chain_stone->number_of_freedoms - 1; // -1 car il y a une pierre a côté
		if(!check_is_in_same_chain)
			stone_around_chain->number_of_freedoms =  stone_around_chain->number_of_freedoms - 1; // Si pas dans la même chaine, on baisse aussi le nombre de libertée de l'autre
		if(stone_around_chain->number_of_freedoms < 1)
			capture_chain(stone_around_chain);
		if(chain_stone->number_of_freedoms < 1) // On sait jamais, peut etre à enlever
			capture_chain(chain_stone);
	}
}

/*
 * Gère la capture des chaines
 */
void capture_chain(Chain* chain){
	printf("\n============================[ CHAIN CAPUTRES ]========================================");
	for(int count_chain = 0; count_chain < chain->size; count_chain++){
		Stone* stone_checked = chain->stones[count_chain];
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
		give_chain_liberties(get_stone(stone_checked->x+1,stone_checked->y));
		give_chain_liberties(get_stone(stone_checked->x-1,stone_checked->y));
		give_chain_liberties(get_stone(stone_checked->x,stone_checked->y+1));
		give_chain_liberties(get_stone(stone_checked->x,stone_checked->y-1));
	}
	printf("\n============================[ END CHAIN CAPUTRES ]========================================");
	redraw_win();
}

/*
 * Redonne des libertées à la chaine de la pierre passé en paramètre
 */
void give_chain_liberties(Stone* stone){
	Chain* chain_found;
	if(stone != NULL && stone->visible == true){
		chain_found = find_chain(stone);
		chain_found->number_of_freedoms = chain_found->number_of_freedoms + 1;
	}
}

/*
 * Retourne si la pierre capture les pierres qui sont au tour
 */
bool check_around_captured(Stone* stone){
	bool jouable = false;
	Stone* stone_around;
	if((stone_around = get_stone(stone->x+1, stone->y)) != NULL && stone_around->color != stone->color){
		is_playable(stone_around);
	}
	if((stone_around = get_stone(stone->x-1, stone->y)) != NULL && stone_around->color != stone->color){
		is_playable(stone_around);
	}
	if((stone_around = get_stone(stone->x, stone->y+1)) != NULL && stone_around->color != stone->color){
		is_playable(stone_around);
	}
	if((stone_around = get_stone(stone->x, stone->y-1)) != NULL && stone_around->color != stone->color){
		is_playable(stone_around);
	}
	return jouable;
}

/*
 * Retourne true si la pierre est jouable
 */
bool is_playable(Stone* stone){
	bool jouable = false;
	if(find_chain(stone) != NULL){
		printf("\n-- libereté de cette chaine : %d\n",find_chain(stone)->number_of_freedoms);
		if(find_chain(stone)->number_of_freedoms == 1)
			jouable = true;
	}
	return jouable;
}

/*
 * Verifie si la chaine de la stone1 est dans la même chaine que la chaine de la stone2
 */
bool is_in_same_chain(Stone* stone1, Stone* stone2){
	bool is_in_his_chain = false;
	Chain* stone1_chain = find_chain(stone1);
	for(int chainCounter = 0; chainCounter < stone1_chain->size; chainCounter++){
		Stone* stone_checked = stone1_chain->stones[chainCounter];
		if(stone_checked->x == stone2->x && stone_checked->y == stone2->y){
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
	if(get_stone(stone->x+1, stone->y) != NULL && get_stone(stone->x+1, stone->y)->visible && get_stone(stone->x+1, stone->y)->color != stone->color
		&& get_stone(stone->x-1, stone->y) != NULL && get_stone(stone->x-1, stone->y)->visible && get_stone(stone->x-1, stone->y)->color != stone->color
		&& get_stone(stone->x, stone->y+1) != NULL && get_stone(stone->x, stone->y+1)->visible && get_stone(stone->x, stone->y+1)->color != stone->color
		&& get_stone(stone->x, stone->y-1) != NULL && get_stone(stone->x, stone->y-1)->visible && get_stone(stone->x, stone->y-1)->color != stone->color){
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
				//printf("\nFound stone x : %i,y : %i, : %i",stone_checked->x, stone_checked->y, found_stone);
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

/*
 * Permet de calculer le score de chaque joueur
 */
void calculate_score(float result[2]){
	Stone* checked_stone = NULL;
	float score_player_1 = 0;
	float score_player_2 = 0;
	for(int i = 0; i < BOARD->size; i++){
		for(int j = 0; j < BOARD->size; j++){
			checked_stone = get_stone(i,j);
			if(checked_stone != NULL){
				if(checked_stone->color == 'B'){ // On calcule les points pour le joueur noire
					score_player_1++;
				}else{ // On calcule les points pour le joueur blanc
					score_player_2++;
				}
			}
		}
	}
	//On ajoute les points pour avoir commencer deuxieme
	score_player_2 += 7.5;
	result[0] = score_player_1;
	result[1] = score_player_2;
}

/*
 * Permet de mettre la variable loaded_game a true
 */
void loaded_game_is_true(){
	loaded_game = true;
}
