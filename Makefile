VPATH= src/dessine src/files/ src/game/ src/
SRC= $(wildcard src/dessine/*.c src/files/*.c src/game/*.c)
OBJ= $(notdir $(patsubst %.c,%.o,$(SRC)))
LDFLAGS = -lX11 -lm -std=c99 -g -W -Wall -Wmissing-declarations -Wmissing-prototypes -Wredundant-decls -Wbad-function-cast -Wcast-qual

all: game

game: gameLauncher.o $(OBJ)
	gcc -o $@ $^ $(LDFLAGS)

benchmark: benchmarkGame.o $(OBJ)
	gcc -o $@ $^ $(LDFLAGS)

%.o: %.c
	gcc -c $< -I./header

clean:
	rm game *.o
