#include "game.h"
#include "battleship.h"
#include "keyboard_mouse.h"
#include "graphics.h"

static tabuleiro* tab_player;
static tabuleiro* tab_computer;

GameState* newGame() {
	GameState* state = (GameState*) malloc(sizeof(GameState));

	state->ship_map = loadBitmap("home/lcom/proj/img/mapanaves.bmp");

	state->turn = 1; //1- player turn 0-player turn
	state->turn_time_counter = TURN_TIME;
	state->done = 0;

	initPlayer(HUMAN, state);

	initPlayer(COMPUTER, state);

	return state;
}

void drawGame(Battleship* battle) {
	printf("entra draw");
	drawTabuleirosGame(game_state->hum.tab, game_state->com.tab, game_state->ship_map);

}

State updateGame(Battleship* battle) {

	if (game_state->turn == 0) {
		bot_play(battle);
		return GAME_PLAY_STATE;
	}
	printf("passa bot");
	if (battle->timer_cnt == 0) {
		game_state->turn_time_counter--;
	}
	printf("passa timer");

	if (game_state->turn_time_counter == 0) {
		game_state->hum.turns_missed++;
		if (game_state->hum.turns_missed == 3) {
			game_state->done = 1;
			return MAIN_MENU_STATE;
		}
	}
	printf("passa turno");

	switch (battle->kb_code) {
	case KEY_ARR_UP_BRK:
		game_state->com.tab.selected_y--;
		break;
	case KEY_ARR_DOWN_BRK:
		if (game_state->com.tab.selected_y != 9)
			game_state->com.tab.selected_y++;
		break;
	case KEY_ARR_RIGHT_BRK:
		if (game_state->com.tab.selected_x != 9)
			game_state->com.tab.selected_x++;
		break;
	case KEY_ARR_LEFT_BRK:
		game_state->com.tab.selected_x--;
		break;
	case KEY_ENTER_BRK:
		if (game_state->com.tab.tab_array[game_state->com.tab.selected_x][game_state->com.tab.selected_y]->hit) {
			return GAME_PLAY_STATE;
		} else {

			game_state->com.tab.tab_array[game_state->com.tab.selected_x][game_state->com.tab.selected_y]->hit = 1;
		}

		if (game_state->com.tab.tab_array[game_state->com.tab.selected_x][game_state->com.tab.selected_y]->t_part == WATER) {
			game_state->turn = !game_state->turn;
			game_state->hum.shots_missed++;
		} else {

			game_state->turn_time_counter = TURN_TIME;
		}
		break;
	}
	printf("passa no switch!");
	battle->kb_code = KEY_NONE;
	return GAME_PLAY_STATE;
}
void deleteGame(Battleship* battle) {

}

void bot_play(Battleship* battles) {

}

SetShipState* newPlaySetship() {
	tab_player = NULL;
	tab_computer = NULL;

	SetShipState* state = (SetShipState*) malloc(sizeof(SetShipState));

	//initialize player tab
	initShip(&state->tab);
	//initialize computer tab
	initShip(&state->tab_com);

	randTabuleiro(&state->tab_com);

	state->ship_temp = &state->tab.ship_array[0];

	state->ship_map = loadBitmap("home/lcom/proj/img/mapanaves.bmp");

	return state;
}

void drawPlaySetship(Battleship* battle) {

	drawSetTabuleiro(400, 200, set_ship->tab, set_ship->ship_temp);
}

State updatePlaySetship(Battleship* battle) {

	switch (battle->kb_code) {
	case KEY_ARR_UP_BRK:
		if (set_ship->ship_temp->y_central != 0) {
			set_ship->ship_temp->y_central--;
		}
		battle->kb_code = KEY_NONE;
		break;

	case KEY_ARR_DOWN_BRK:
		if (set_ship->ship_temp->t_ship == DEATH_STAR) {
			if (set_ship->ship_temp->y_central + 1 != 9) {
				set_ship->ship_temp->y_central++;
			}
		} else if (set_ship->ship_temp->direction == 'v') {
			if (set_ship->ship_temp->y_central + set_ship->ship_temp->size != 10) {
				set_ship->ship_temp->y_central++;
			}

		} else if (set_ship->ship_temp->y_central < 9) {
			set_ship->ship_temp->y_central++;
		}

		battle->kb_code = KEY_NONE;
		break;

	case KEY_ARR_LEFT_BRK:
		if (set_ship->ship_temp->x_central != 0) {
			set_ship->ship_temp->x_central--;
		}
		battle->kb_code = KEY_NONE;
		break;

	case KEY_ARR_RIGHT_BRK:
		if (set_ship->ship_temp->t_ship == DEATH_STAR) {
			if (set_ship->ship_temp->x_central + 1 != 9) {
				set_ship->ship_temp->x_central++;
			}
		} else if (set_ship->ship_temp->direction == 'h') {
			if (set_ship->ship_temp->x_central + set_ship->ship_temp->size != 10) {
				set_ship->ship_temp->x_central++;
			}

		} else if (set_ship->ship_temp->x_central < 9) {
			set_ship->ship_temp->x_central++;
		}

		battle->kb_code = KEY_NONE;
		break;

	case KEY_ENTER_BRK:
		if (checkColission(set_ship->tab, set_ship->ship_temp)) {
			battle->kb_code = KEY_NONE;
			return;
		}
		unsigned int i = 0;
		short x = set_ship->ship_temp->x_central;
		short y = set_ship->ship_temp->y_central;

		if (set_ship->ship_temp->t_ship == DEATH_STAR) {
			set_ship->tab.tab_array[x][y] = &set_ship->ship_temp->parts_array[0];
			set_ship->tab.tab_array[x + 1][y] = &set_ship->ship_temp->parts_array[1];
			set_ship->tab.tab_array[x][y + 1] = &set_ship->ship_temp->parts_array[2];
			set_ship->tab.tab_array[x + 1][y + 1] = &set_ship->ship_temp->parts_array[3];

		} else if (set_ship->ship_temp->direction == 'h') {
			for (i = 0; i < set_ship->ship_temp->size; i++) {
				set_ship->tab.tab_array[x + i][y] = &set_ship->ship_temp->parts_array[i];
			}

		} else if (set_ship->ship_temp->direction == 'v') {
			for (i = 0; i < set_ship->ship_temp->size; i++) {
				set_ship->tab.tab_array[x][y + i] = &set_ship->ship_temp->parts_array[i];
			}
		}

		set_ship->tab.ship_on_board++;
		set_ship->ship_temp = &set_ship->tab.ship_array[set_ship->tab.ship_on_board];

		battle->kb_code = KEY_NONE;

		if (set_ship->tab.ship_on_board == 7) {
			set_ship->done = 1;
			return GAME_PLAY_STATE;
		}

		break;

	case KEY_R_BRK:

		if (set_ship->ship_temp->direction == 'h' && set_ship->ship_temp->y_central + set_ship->ship_temp->size < 11) {
			set_ship->ship_temp->direction = 'v';

		} else if (set_ship->ship_temp->direction == 'v' && set_ship->ship_temp->x_central + set_ship->ship_temp->size < 11) {
			set_ship->ship_temp->direction = 'h';
		}

		battle->kb_code = KEY_NONE;
		break;
	}

	return GAME_PLAY_SETSHIP_STATE;
}

void deletePlaySetship(Battleship* battle) {

	deleteBitmap(set_ship->ship_map);
	tab_player = &set_ship->tab;
	tab_computer = &set_ship->tab_com;
}

void initShip(tabuleiro* tab) {
	ship_part* water = (ship_part*) malloc(sizeof(ship_part));
	water->t_part = WATER;
	water->t_ship = NOTHING;
	water->hit = 0;

	unsigned int i;
	for (i = 0; i <= 100; i++) { //preenche o tabuleiro com agua
		tab->tab_array[i / 10][i % 10] = water;
	}
	tab->selected_x = -1;
	tab->selected_y = -1;
	tab->ship_on_board = 0;

	/**
	 * DEATH STAR INIT
	 */
	tab->ship_array[0].parts_array = malloc(sizeof(ship_part) * 4);
	tab->ship_array[0].t_ship = DEATH_STAR;
	tab->ship_array[0].parts_array[0].t_part = UPPER_LEFT;
	tab->ship_array[0].parts_array[0].t_ship = DEATH_STAR;
	tab->ship_array[0].parts_array[0].hit = 0;
	tab->ship_array[0].parts_array[1].t_part = UPPER_RIGHT;
	tab->ship_array[0].parts_array[1].t_ship = DEATH_STAR;
	tab->ship_array[0].parts_array[1].hit = 0;
	tab->ship_array[0].parts_array[2].t_part = BOTTOM_LEFT;
	tab->ship_array[0].parts_array[2].t_ship = DEATH_STAR;
	tab->ship_array[0].parts_array[2].hit = 0;
	tab->ship_array[0].parts_array[3].t_part = BOTTOM_RIGHT;
	tab->ship_array[0].parts_array[3].t_ship = DEATH_STAR;
	tab->ship_array[0].parts_array[3].hit = 0;
	tab->ship_array[0].nr_hits = 0;
	tab->ship_array[0].destroyed = 0;
	tab->ship_array[0].x_central = 0;
	tab->ship_array[0].y_central = 0;
	tab->ship_array[0].direction = 'v';
	tab->ship_array[0].size = 4;

	/**
	 * BATTLESHIP INIT
	 */
	tab->ship_array[1].parts_array = malloc(sizeof(ship_part) * 5);
	tab->ship_array[1].t_ship = BATTLESHIP;
	tab->ship_array[1].parts_array[0].t_part = FIRST;
	tab->ship_array[1].parts_array[0].t_ship = BATTLESHIP;
	tab->ship_array[1].parts_array[0].hit = 0;
	tab->ship_array[1].parts_array[1].t_part = SECOND;
	tab->ship_array[1].parts_array[1].t_ship = BATTLESHIP;
	tab->ship_array[1].parts_array[1].hit = 0;
	tab->ship_array[1].parts_array[2].t_part = THIRD;
	tab->ship_array[1].parts_array[2].t_ship = BATTLESHIP;
	tab->ship_array[1].parts_array[2].hit = 0;
	tab->ship_array[1].parts_array[3].t_part = FOURTH;
	tab->ship_array[1].parts_array[3].t_ship = BATTLESHIP;
	tab->ship_array[1].parts_array[3].hit = 0;
	tab->ship_array[1].parts_array[4].t_part = FIFTH;
	tab->ship_array[1].parts_array[4].t_ship = BATTLESHIP;
	tab->ship_array[1].parts_array[4].hit = 0;
	tab->ship_array[1].nr_hits = 0;
	tab->ship_array[1].destroyed = 0;
	tab->ship_array[1].x_central = 0;
	tab->ship_array[1].y_central = 0;
	tab->ship_array[1].direction = 'v';
	tab->ship_array[1].size = 5;

	/**
	 * CRUSER 1 INIT
	 */
	tab->ship_array[2].parts_array = malloc(sizeof(ship_part) * 3);
	tab->ship_array[2].t_ship = CRUSER;
	tab->ship_array[2].parts_array[0].t_part = FIRST;
	tab->ship_array[2].parts_array[0].t_ship = CRUSER;
	tab->ship_array[2].parts_array[0].hit = 0;
	tab->ship_array[2].parts_array[1].t_part = SECOND;
	tab->ship_array[2].parts_array[1].t_ship = CRUSER;
	tab->ship_array[2].parts_array[1].hit = 0;
	tab->ship_array[2].parts_array[2].t_part = THIRD;
	tab->ship_array[2].parts_array[2].t_ship = CRUSER;
	tab->ship_array[2].parts_array[2].hit = 0;
	tab->ship_array[2].nr_hits = 0;
	tab->ship_array[2].destroyed = 0;
	tab->ship_array[2].x_central = 0;
	tab->ship_array[2].y_central = 0;
	tab->ship_array[2].direction = 'v';
	tab->ship_array[2].size = 3;

	/**
	 * CRUSER 2 INIT
	 */

	tab->ship_array[3].parts_array = malloc(sizeof(ship_part) * 3);
	tab->ship_array[3].t_ship = CRUSER;
	tab->ship_array[3].parts_array[0].t_part = FIRST;
	tab->ship_array[3].parts_array[0].t_ship = CRUSER;
	tab->ship_array[3].parts_array[0].hit = 0;
	tab->ship_array[3].parts_array[1].t_part = SECOND;
	tab->ship_array[3].parts_array[1].t_ship = CRUSER;
	tab->ship_array[3].parts_array[1].hit = 0;
	tab->ship_array[3].parts_array[2].t_part = THIRD;
	tab->ship_array[3].parts_array[2].t_ship = CRUSER;
	tab->ship_array[3].parts_array[2].hit = 0;
	tab->ship_array[3].nr_hits = 0;
	tab->ship_array[3].destroyed = 0;
	tab->ship_array[3].x_central = 0;
	tab->ship_array[3].y_central = 0;
	tab->ship_array[3].direction = 'v';
	tab->ship_array[3].size = 3;

	/**
	 * FIGHTER 1 INIT
	 */
	tab->ship_array[4].parts_array = malloc(sizeof(ship_part) * 2);
	tab->ship_array[4].t_ship = FIGHTER;
	tab->ship_array[4].parts_array[0].t_part = FIRST;
	tab->ship_array[4].parts_array[0].t_ship = FIGHTER;
	tab->ship_array[4].parts_array[0].hit = 0;
	tab->ship_array[4].parts_array[1].t_part = SECOND;
	tab->ship_array[4].parts_array[1].t_ship = FIGHTER;
	tab->ship_array[4].parts_array[1].hit = 0;
	tab->ship_array[4].nr_hits = 0;
	tab->ship_array[4].destroyed = 0;
	tab->ship_array[4].x_central = 0;
	tab->ship_array[4].y_central = 0;
	tab->ship_array[4].direction = 'v';
	tab->ship_array[4].size = 2;

	/**
	 * FIGHTER 2 INIT
	 */
	tab->ship_array[5].parts_array = malloc(sizeof(ship_part) * 2);
	tab->ship_array[5].t_ship = FIGHTER;
	tab->ship_array[5].parts_array[0].t_part = FIRST;
	tab->ship_array[5].parts_array[0].t_ship = FIGHTER;
	tab->ship_array[5].parts_array[0].hit = 0;
	tab->ship_array[5].parts_array[1].t_part = SECOND;
	tab->ship_array[5].parts_array[1].t_ship = FIGHTER;
	tab->ship_array[5].parts_array[1].hit = 0;
	tab->ship_array[5].nr_hits = 0;
	tab->ship_array[5].destroyed = 0;
	tab->ship_array[5].x_central = 0;
	tab->ship_array[5].y_central = 0;
	tab->ship_array[5].direction = 'v';
	tab->ship_array[5].size = 2;

	/**
	 * ESCAPE POD INIT
	 */
	tab->ship_array[6].parts_array = malloc(sizeof(ship_part) * 2);
	tab->ship_array[6].t_ship = ESCAPE_SHUTTLE;
	tab->ship_array[6].parts_array[0].t_part = FIRST;
	tab->ship_array[6].parts_array[0].t_ship = ESCAPE_SHUTTLE;
	tab->ship_array[6].parts_array[0].hit = 0;
	tab->ship_array[6].nr_hits = 0;
	tab->ship_array[6].destroyed = 0;
	tab->ship_array[6].x_central = 0;
	tab->ship_array[6].y_central = 0;
	tab->ship_array[6].direction = 'v';
	tab->ship_array[6].size = 1;
}

int checkColission(tabuleiro tab, ship* s) {
	unsigned int i;

	if (s->t_ship == DEATH_STAR) {
		if (s->x_central + 2 > 10 || s->y_central + 2 > 10) {
			return 1;
		}
		if (tab.tab_array[s->x_central][s->y_central]->t_part != WATER) {
			return 1;
		}
		if (tab.tab_array[s->x_central + 1][s->y_central]->t_part != WATER) {
			return 1;
		}
		if (tab.tab_array[s->x_central][s->y_central + 1]->t_part != WATER) {
			return 1;
		}
		if (tab.tab_array[s->x_central + 1][s->y_central + 1]->t_part != WATER) {
			return 1;
		}
	} else {
		for (i = 0; i < s->size; i++) {
			if (s->direction == 'h') {
				if (s->x_central + s->size > 10) {
					return 1;
				}
				if (tab.tab_array[s->x_central + i][s->y_central]->t_part != WATER) {
					return 1;
				}
			} else if (s->direction == 'v') {
				if (s->y_central + s->size > 10) {
					return 1;
				}
				if (tab.tab_array[s->x_central][s->y_central + i]->t_part != WATER) {
					return 1;
				}
			}
		}
	}
	return 0;
}

void randTabuleiro(tabuleiro* tab) {
	unsigned int x, y, i, dir, m;

	for (i = 0; i < 7; i++) {
		x = rand() % 10;
		y = rand() % 10;
		dir = rand() % 2; //1 -v 0-h

		if (dir == 0) {
			tab->ship_array[i].direction = 'h';
		} else if (dir == 1) {
			tab->ship_array[i].direction = 'v';
		}

		//Especial DEATH_STAR
		if (i == 0) {
			tab->ship_array[i].x_central = x;
			tab->ship_array[i].y_central = y;
			if (checkColission(*tab, &(tab->ship_array[i]))) {
				i--;
			} else {

				tab->tab_array[x][y] = &tab->ship_array[i].parts_array[0];
				tab->tab_array[x + 1][y] = &tab->ship_array[i].parts_array[1];
				tab->tab_array[x][y + 1] = &tab->ship_array[i].parts_array[2];
				tab->tab_array[x + 1][y + 1] = &tab->ship_array[i].parts_array[3];
			}
		} else {
			tab->ship_array[i].x_central = x;
			tab->ship_array[i].y_central = y;
			if (checkColission(*tab, &(tab->ship_array[i]))) {
				i--;
			} else {
				if (tab->ship_array[i].direction == 'h') {
					for (m = 0; m < tab->ship_array[i].size; m++)
						tab->tab_array[x + m][y] = &tab->ship_array[i].parts_array[m];

				} else if (tab->ship_array[i].direction == 'v') {
					for (m = 0; m < tab->ship_array[i].size; m++)
						tab->tab_array[x][y + m] = &tab->ship_array[i].parts_array[m];
				}
			}
		}
	}
	printf("DeathStar x:%d y:%d", tab->ship_array[0].x_central, tab->ship_array[0].y_central);
	printf("Battleship x:%d y:%d", tab->ship_array[1].x_central, tab->ship_array[1].y_central);
	printf("Cruiser1 x:%d y:%d", tab->ship_array[2].x_central, tab->ship_array[2].y_central);
	printf("Cruiser2 x:%d y:%d", tab->ship_array[3].x_central, tab->ship_array[3].y_central);
	printf("Fighter x:%d y:%d", tab->ship_array[4].x_central, tab->ship_array[4].y_central);
	printf("Fighter x:%d y:%d", tab->ship_array[5].x_central, tab->ship_array[5].y_central);
	printf("Escape pod x:%d y:%d", tab->ship_array[6].x_central, tab->ship_array[6].y_central);

}

void initPlayer(type_player t, GameState* state) {
	if (t == HUMAN) {
		state->hum.shots_missed = 0;
		state->hum.time_played = 0;
		state->hum.turns_missed = 0;
		state->hum.t_player = HUMAN;
		state->hum.tab = *tab_player;
	} else if (t == COMPUTER) {
		state->com.shots_missed = 0;
		state->com.time_played = 0;
		state->com.turns_missed = 0;
		state->com.t_player = COMPUTER;
		state->com.tab = *tab_computer;
		state->com.tab.selected_x = 0;
		state->com.tab.selected_y = 0;
	}
}
