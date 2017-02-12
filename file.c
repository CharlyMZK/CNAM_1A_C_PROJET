#include "FILE.h"
#include "game.h"
File* FILE;

///////////////////// FILE SAVER /////////////////////

/*
 * Permet de sauvegarder la partie dans un fichier
 */
void save_game(Board* board){
  create_header(board->size, board->handicap);
  write_game(board);
  fclose(FILE);
}

/*
 * Permet de crééer le fichier et de mettre l'entête dedans
 */
void create_header(int board_size, int handicap){
  time_t rawtime; // Pour avoir la date du jour
  FILE = fopen("score", "w"); // Recreer un fichier et écris de dans
  fprintf(FILE, "(;FF[4]GM[1]SZ[%i])", board_size);
  fprintf(FILE, "");
  fprintf(FILE, "GN[Go Game Mraled]");
  fprintf(FILE, "PB[Black]");
  fprintf(FILE, "HA[%i]", handicap);
  fprintf(FILE, "PW[White]");
  fprintf(FILE, "KM[%f]", 7.5+handicap);
  fprintf(FILE, "DT[%s]", localtime(&rawtime));
  fprintf(FILE, "TM[1800]"); // Non renseigner
  fprintf(FILE, "RU[Japanese]");
  fprintf(FILE, "");
}

/*
 * Permet d'écrire la partie dans le fichier
 */
void write_game(Board* board){
  int first_time = true; // Premiere fois qu'on mets une pierre de cette couleur
  Stone * stone_to_save = malloc(sizeof(Stone));
  fprint(FILE, ";");
  for(int i = 0; i < board->size; i++){
    for(int j = 0; j < board->size; j++){ // Mets les pierres blanches
      stone_to_save = get_stone(i,j);
      if(stone_to_save != NULL && stone_to_save->color = 'W'){
        if(first_time)
          fprint(FILE, "AW");
        fprint(FILE, "[%s%s]", stone_to_save->x + 'a', stone_to_save->y + 'a');
      }
    }
  }
  first_time = true;
  for(int i = 0; i < board->size; i++){
    for(int j = 0; j < board->size; j++){ // Mets les pierres noires
      stone_to_save = get_stone(i,j);
      if(stone_to_save != NULL && stone_to_save->color = 'B'){
        if(first_time)
          fprint(FILE, "AB");
        fprint(FILE, "[%s%s]", stone_to_save->x + 'a', stone_to_save->y + 'a');
      }
    }
  }
  fprint(")");
}

///////////////////// FILE IMPORTER /////////////////////
/*
 * Permet d'importer un fichier
 */
void import_file(string file_name, Board* board){
  FILE = fopen(file_name, "r");
  if(FILE != NULL){
    import_game(board);
    fclose(FILE);
  } else {
    printf("Fichier non ouvert, nom ou chemin incorrect");
  }
}

/*
 * Permet d'importer les pions sur le board
 */
void import_game(Board* board){
  int character; // Le caractère à prendre
  while((character = fgetc(fp)) != EOF)
    printf("%c",character);
}
