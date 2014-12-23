#pragma once

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdio.h>


// The enum and structs on this file are based on the ones from difusal.blogspot.pt

typedef enum {
	MAIN_MENU_STATE,
	GAME_PLAY_SETSHIP_STATE,
	GAME_PLAY_STATE,
	GAME_PAUSE_STATE,
	HIGHSCORE_WRITE_STATE,
	HIGHSCORE_MENU_STATE,
	EXIT_STATE
} State;

typedef struct {

	int IRQ_SET_KEYBOARD;
	int IRQ_SET_MOUSE;
	int IRQ_SET_TIMER;

	unsigned long kb_code;
	int done, draw;

	int timer_cnt;

	int timer_ticked;
	State currentState;
	void* state;

} Battleship;

Battleship* startBattleship();
void updateBattleship(Battleship* battleship);
void drawBattleship (Battleship* battleship);
void stopBattleship (Battleship* battleship);

void changeState(Battleship* battleship, State programState);
void updateCurrentState(Battleship* battleship);
void deleteCurrentState(Battleship* battleship);


