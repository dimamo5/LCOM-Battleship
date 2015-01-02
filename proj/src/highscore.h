#ifndef _HIGHSCORE_H_
#define _HIGHSCORE_H_

#include "graphics.h"
#include "battleship.h"
#include "game.h"
#include "bitmap.h"
#include "keyboard_mouse.h"

#define highscore ((Highscore_State*) battle->state)

typedef struct {
	char nome[20];
	char data[20];
	int score;
} Jogador;

typedef struct {
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int hour;
	unsigned int min;

	Jogador jogador_array[5]; //array ordenado (decrescente) de scores

	unsigned short show; //0 esta a escrever //1 esta a mostrar

	char* nome_player;

	unsigned short score_introduzido;
	unsigned short nome_introduzido;
	unsigned short done;

	Bitmap* fonts;

} Highscore_State;

Highscore_State* newHighscore(Battleship* battle);

void drawHighscore(Battleship* battle);
State updateHighscore(Battleship* battle);
void deleteHighscore(Battleship* battle);
char getKey(Battleship* battle);
void loadScores(Highscore_State* state);
void string_conc(char* str1, char ch);
void getStringJogador(Battleship* battle, unsigned int i, char * temp);
int entrouHighscore(Battleship* battle);
#endif
