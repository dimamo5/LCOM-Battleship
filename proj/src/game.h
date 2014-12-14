#ifndef __GAME
#define __GAME

typedef enum {
	HUMAN, COMPUTER
} type_player;

typedef enum {
	WATER, FIGHTER, DEATH_STAR, CRUSER, ESCAPE_SHUTTLE, BATTLESHIP
} type_ship;

typedef enum {
	WATER,
	UPPER_LEFT,
	UPPER_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGTH,
	FIRST,
	SECOND,
	THIRD,
	FOURTH,
	FIFTH
} ship_part;

typedef struct {
	type_ship t_ship;
	ship_part* ship_array;
	unsigned short nr_hits;
	unsigned short destroyed;
} ship;

typedef struct {
	ship_part* tab_array[10][10];
	ship ship_array[8]; //water and the other 7 ships
} tabuleiro;

typedef struct {
	type_player t_player;
	tabuleiro tab;
	char * player_name;
	unsigned short time_played;
} player;

typedef struct {
	unsigned short selected;
	type_part t_part;
	unsigned short hit;
} ship_part;

#endif
