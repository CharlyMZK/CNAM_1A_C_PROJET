#include <stdio.h>
#include <stdlib.h>
#include "game.c"

int main()
{
  printf("test\n");
  Board* board = create_board();
  play_stone(board, 0, 5,13);
  print_board(board, 19);
}
