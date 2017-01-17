all: game libdessine.a gamemake

clean:
	rm game dessine.o libdessine.a
	
game: gameLauncher.c game.c game.h libdessine.a
	gcc -g gameLauncher.c -o game -L. -ldessine -lX11 -lm

libdessine.a: dessine.c dessine.h 
	gcc -g -c dessine.c
	ar -r libdessine.a dessine.o
	
gamemake: benchmarkGame.c game.c
	gcc -o gamebench benchmarkGame.c -L. -ldessine -lX11 -lm
