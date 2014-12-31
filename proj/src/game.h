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
	WATER, FULL, SELECTED, UPPER_LEFT, UPPER_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, FIRST, SECOND, THIRD, FOURTH, FIFTH
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
	unsigned short destroyed_ship_turn;
} player;

typedef struct {
	unsigned int last_x_hit;
	unsigned int last_y_hit;
	unsigned int previous_hit; //diz se o hit anterior foi numa peca de navio
	char direction;
	unsigned int orientation; // 0 goes back and 1 goes forward
	unsigned int orientation_was_inverted;
} bot_ai;

typedef struct {
	unsigned short turn_time_counter;
	unsigned short turn; //turn 1 player turn else computer
	unsigned short winner;
	player hum;
	player com;
	bot_ai ai_comp;
	Bitmap* ship_map;
	Bitmap* alarm_clock;
	Bitmap* ship_list;
	Bitmap* turns;
	int done;
} GameState;

typedef struct {
	tabuleiro tab;
	tabuleiro tab_com;
	ship* ship_temp;
	Bitmap* ship_list;
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

void bot_play(Battleship* battle);

int checkShips(Battleship* battle);

int gameOver(Battleship* battle); //winner 1-hum     2-comp     3-none

unsigned int calculaScore(Battleship* battle);

void updateShips(Battleship* battle);

#endif
