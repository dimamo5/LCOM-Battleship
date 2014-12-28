#ifndef __GAME
#define __GAME

#include "battleship.h"
#include "button.h"
#include "bitmap.h"

typedef enum {
	HUMAN, COMPUTER
} type_player;

typedef enum {
	NOTHING,FIGHTER, DEATH_STAR, CRUSER, ESCAPE_SHUTTLE, BATTLESHIP
} type_ship;

typedef enum {
	WATER,
	FULL,
	UPPER_LEFT,
	UPPER_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	FIRST,
	SECOND,
	THIRD,
	FOURTH,
	FIFTH
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
	char * player_name;
	unsigned short time_played;
} player;



typedef struct{
	unsigned short turn_time;
	unsigned short ready;
	player hum;
	player com;
}game;

typedef struct{
	tabuleiro tab;
	int ship_selected;
	ship* ship_temp;
	Button* fighter;
	Button* death_star;
	Button* cruser;
	Button* escape_pod;
	Button* battleship;
	Button* cruser_2;
	Button* escape_pod_2;
	Bitmap* ship_map;
	int done;
}SetShipState;


game* newGame();
void drawGame(Battleship* battle);
State updateGame(Battleship* battle);
void deleteGame(Battleship* battle);

SetShipState* newPlaySetship();
void drawPlaySetship(Battleship* battle);
State updatePlaySetship(Battleship* battle);
void deletePlaySetship(Battleship* battle);

void initShip(SetShipState* state);
void updateSetShipBoard(SetShipState* state);
#endif
