#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.c"

int main() 
{
	init_win(600,600, "Essai");
	event_loop();
	return EXIT_SUCCESS;
}
