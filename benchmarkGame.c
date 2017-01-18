#include <stdio.h>
#include <stdlib.h>
#include "game.c"

int main()
{
  printf("test\n");
<<<<<<< HEAD
  create_board(19);
  play_white_stone(5, 13);
=======
  init_board(19);
  play_stone(5, 13,1);
>>>>>>> 196d111a46f27acfb70f314124a60438d3279f08
  print_board();
}
