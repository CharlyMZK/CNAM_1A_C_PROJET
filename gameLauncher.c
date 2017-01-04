#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.c"

int main() 
{
	init_win(400,400, "Essai",0.2,0.2,0.6);
	event_loop();
	return EXIT_SUCCESS;
}
