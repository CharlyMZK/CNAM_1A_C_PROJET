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

int main()
{
	//if(ask_mode() == 'j'){
		init_board(/*ask_board_size()*/19);
		init_win(608,608, "Essai");
		event_loop();
	//}
	return EXIT_SUCCESS;
}
