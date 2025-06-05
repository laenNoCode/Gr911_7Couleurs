FLAGS=-Wall -Werror -lm -g

7color : o/GameState.o
	gcc $(FLAGS) o/GameState.o -o 7color

o/GameState.o: head/GameState.h src/GameState.c
	gcc -o votre_programme src/GameState.c -lSDL2 -lSDL2_image -lSDL2_ttf

