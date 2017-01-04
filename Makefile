all: game

clean:
	rm game dessine.o libdessine.a
	
game: game.c game.h libdessine.a
	gcc -g game.c -o game -L. -ldessine -lX11

libdessine.a: dessine.c dessine.h
	gcc -g -c dessine.c
	ar -r libdessine.a dessine.o
	
gamemake: benchmarkGame.c game.c
	gcc -o gamemake benchmarkGame.c game.c -I. 
