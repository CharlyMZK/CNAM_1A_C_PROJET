#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "game.h"

/*
 * Permet de sauvegarder la partie dans un fichier
 */
void save_game(Board* board);

/*
 * Permet de crééer le fichier et de mettre l'entête dedans
 */
void create_header(int board_size, int handicap);

/*
 * Permet d'écrire la partie dans le fichier
 */
void write_game(Board* board);
