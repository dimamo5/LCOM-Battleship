#ifndef __GAME
#define __GAME

#include "battleship.h"
#include "button.h"
#include "bitmap.h"

#define set_ship ((SetShipState*) battle->state)
#define game_state ((GameState*) battle->state)
#define TURN_TIME 10

typedef enum {
	HUMAN, COMPUTER
} type_player;

typedef enum {
	NOTHING, FIGHTER, DEATH_STAR, CRUSER, ESCAPE_SHUTTLE, BATTLESHIP
} type_ship;

typedef enum {
	WATER, FULL,SELECTED, UPPER_LEFT, UPPER_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, FIRST, SECOND, THIRD, FOURTH, FIFTH
} type_part;

typedef struct {
	type_part t_part;
	type_ship t_ship;
	unsigned short hit;
} ship_part;

typedef struct {
	type_ship t_ship;
	ship_part* parts_array;
	unsigned short nr_hits;
	unsigned short destroyed;
	unsigned int x_central, y_central;
	char direction;
	unsigned int size;
} ship;

typedef struct {
	ship_part* tab_array[10][10];
	unsigned int selected_x;
	unsigned int selected_y;
	ship ship_array[7]; //7 ships
	short ship_on_board;
} tabuleiro;

typedef struct {
	type_player t_player;
	tabuleiro tab;
	unsigned short shots_missed;
	unsigned short time_played;
	unsigned short turns_missed;
} player;

typedef struct {
	unsigned short turn_time_counter;
	unsigned short turn;
	player hum;
	player com;

	Bitmap* ship_map;
	int done;
} GameState;

typedef struct {
	tabuleiro tab;
	tabuleiro tab_com;
	ship* ship_temp;

	Bitmap* ship_map;
	int done;
} SetShipState;

GameState* newGame();
void drawGame(Battleship* battle);
State updateGame(Battleship* battle);
void deleteGame(Battleship* battle);

SetShipState* newPlaySetship();
void drawPlaySetship(Battleship* battle);
State updatePlaySetship(Battleship* battle);
void deletePlaySetship(Battleship* battle);

void initShip(tabuleiro* tab);
void updateSetShipBoard(SetShipState* state);
int checkColission(tabuleiro tab, ship* s);
void randTabuleiro(tabuleiro* tab);

void initPlayer(type_player t, GameState* state);

//1000/0.05*hit_missed+0.01*timed_total
#endif
