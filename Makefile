CC=gcc
# CFLAGS=-W -Wall -ansi -pedantic -Isrc/SDL2/include
CFLAGS=-W -Wall -O3 -Isrc/SDL2/include
LDFLAGS=-Lsrc/SDL2/lib -lmingw32 -lSDL2main -lSDL2 -mwindows

EXEC=main
SRC= $(wildcard *.c) $(wildcard src/*.c) $(wildcard src/gui/*.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

main: $(OBJ)
	$(CC) -o build/$@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@del /Q /F src\*.o src\gui\*.o 2> NUL

mrproper: clean
	@del /Q /F build\$(EXEC).exe 2> NUL

everything:
	make mrproper && make all & make clean
