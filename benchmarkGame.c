#include <stdio.h>
#include <stdlib.h>
#include "game.c"

int main()
{
  printf("test\n");
  create_board(19);
  play_stone(5, 13,1);
  print_board();
}
