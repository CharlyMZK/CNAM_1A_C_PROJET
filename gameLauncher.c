#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.c"

/*
 * Permet de choisir de jouer contre un joueur ou l'ordinateur
 */
char ask_mode(){
	char mode;
	printf("Voulez vous jouer : contre un joueur : entrez j\n");
	printf("                    contre l'ordinateur : entrez o\n");
	scanf("%c", &mode);
	if(mode != 'j' || mode != 'o');
		ask_mode;
	return mode;
}

/*
 * Permet de choisir la taille du plateau
 */
int ask_board_size(){
	int size;
	printf("Quelle taille de plateau voulez vous avoir ? (Les possibilitées sont 9, 13 et 19)\n");
	scanf("%i", &size);
	if(size != 9 || size != 13 || size != 19)
		ask_board_size;
	return size;
}

// Peut prendre en paramètre la taille de la fenêtre
int main(int argc, char* argv[])
{
	//if(ask_mode() == 'j'){
		//init_board(ask_board_size());
		init_board(9);
		 
		if(argc > 1)
			init_win(atoi(argv[1]),atoi(argv[1]), "GO Game Mraled");
		else
			init_win(608,608, "GO Game Mraled");
		event_loop();
	//}
	return EXIT_SUCCESS;
}
