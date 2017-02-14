#include <stdio.h>
#include <stdlib.h>
#include "game.h"

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
  fprintf(FILE_GAME, "KM[%g]\n", 7.5);
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
/*
 * Permet d'importer un fichier
 */
void import_file(char* FILE_GAME_name){ 
  FILE_GAME = fopen(FILE_GAME_name, "r");
  if(FILE_GAME != NULL){
	import_game(); 
    fclose(FILE_GAME); 
  } else {
    printf("Fichier non ouvert, nom ou chemin incorrect");
  } 
}

/*
 * Permet d'importer les pions sur le board
 */
void import_game(){

 char file_string[1000] = "";
	  char character; // Le caractère à prendre
	  char* var_string = NULL;
    char* date;
	  int cptB = 0;
	  int cptW = 0;
	  int cptSz = 0;
	  int cpt_date = 0;

	  char* total = malloc(10*sizeof(char)); 
	  int size = 0;
	  Stone* stone;

        while (fgets(file_string, 1000, FILE_GAME) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
        {
        	printf("\n%s\n", file_string); // On affiche la chaîne qu'on vient de lire
			 
			//print(chaine);
			var_string = file_string;
 
			while(var_string[cptSz] != '\0'){
				if(var_string[cptSz] == 'S' && 	var_string[cptSz+1] == 'Z'){
					printf("size : %c %c",var_string[cptSz+3],var_string[cptSz+4]); 
					total[0] = var_string[cptSz+3];
					total[1] = var_string[cptSz+4];

					size = atoi(total); 
					printf("SIZE : %d",size); 
					init_board(size); 

				} 
				cptSz++; 
			}
			
			if(var_string[0] == 'D' && var_string[1] == 'T'){
				date = malloc(10*sizeof(char)); 
				cpt_date = 3;
				while(cpt_date < 12){
					date[cpt_date] = var_string[cpt_date];
					cpt_date++;
				} 
			}

			if(var_string[1] == 'A'){
				printf("\n\nSTOP\n\n"); 
				if(var_string[2] == 'W'){
					printf("\n\nWHITE\n\n");
 
					cptW = 3;
					while(var_string[cptW] != '\0'){
						printf("{%c}",var_string[cptW]); 
						if(var_string[cptW] == '['){
							printf("Entrée");
							stone = malloc(sizeof(Stone));
							stone->x = var_string[cptW+1] - 'a';
							stone->y = var_string[cptW+2] - 'a';
							stone->color = 'W';
							stone->visible = 1; 
							printf("\nSTONE WHITE : %d, %d\n",stone->x,stone->y);    	
							set_stone(stone);
						}   
						cptW++;
					}
					
				}
				
			} 
			if(var_string[0] == 'A'){
				if(var_string[1] == 'B'){
					printf("\n\nBLACK\n\n");

					cptB = 1;
					while(var_string[cptB] != '\0'){
						cptB++; 
						printf("{%c}",var_string[cptB]);
						if(var_string[cptB] == '['){
							printf("Entrée");
							stone = malloc(sizeof(Stone));
							stone->x = var_string[cptB+1] - 'a';
							stone->y = var_string[cptB+2] - 'a';
							stone->color = 'B';
							stone->visible = 1;
							printf("\nSTONE BLACK : %d, %d\n",stone->x,stone->y);    	
							set_stone(stone);
						}     
					} 
					
				}
			}
        }  

		redraw_win();
}
 