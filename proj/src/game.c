#include "game.h"
#include "battleship.h"
#include "keyboard_mouse.h"
#include "graphics.h"

static tabuleiro* tab_player;
static tabuleiro* tab_computer;

GameState* newGame() {
	printf("\n Entrou em newGame");
	GameState* state = (GameState*) malloc(sizeof(GameState));

	state->ship_map = loadBitmap("home/lcom/proj/img/mapanaves.bmp");
	state->alarm_clock = loadBitmap("home/lcom/proj/img/alarm_clock.bmp");
	state->ship_list = loadBitmap("home/lcom/proj/img/ship_list.bmp");
	state->turns = loadBitmap("home/lcom/proj/img/turns.bmp");
	state->pause_screen = loadBitmap("home/lcom/proj/img/pause.bmp");
	state->bmp_turns_missed = loadBitmap("home/lcom/proj/img/turnos_falhados.bmp");
	state->cross = loadBitmap("home/lcom/proj/img/red_cross.bmp");

	state->turn = 1; //1- player turn 0-player turn
	state->turn_time_counter = TURN_TIME;
	state->done = 0;
	state->pause = 0;
	state->winner = 0;

	state->ai_comp.previous_hit = 0;
	state->ai_comp.orientation = 2;
	state->ai_comp.orientation_was_inverted = 0;
	state->ai_comp.direction = 'n';

	initPlayer(HUMAN, state);

	initPlayer(COMPUTER, state);

	return state;
}

void drawGame(Battleship* battle) {
	if (!game_state->pause) {
		drawTabuleirosGame(game_state->hum.tab, game_state->com.tab, game_state->ship_map, game_state->turn);
		drawClock(game_state->turn_time_counter, game_state->alarm_clock);
		drawDestroyedList(battle);
		drawTurnsMissed(600, 200, game_state->hum.turns_missed, game_state->cross, game_state->bmp_turns_missed);
		if (game_state->winner) {
			drawWinner(game_state->winner);
		}
	} else {
		aloca_pixmap(0, 0, game_state->pause_screen->Data, game_state->pause_screen->bitmapInfo.width,
				game_state->pause_screen->bitmapInfo.height);
	}
}

State updateGame(Battleship* battle) {
	static int tempo_bot_espera;
	if (game_state->winner) {
		printf("\nentrou na winner");
		if (game_state->winner == 1) {
			printf("\njogador ganhou");
			battle->highscore_winner = calculaScore(battle);
		} else if (game_state->winner == 2) {
			printf("\ncomputador ganhou");
		}
		if (battle->kb_code == KEY_ESC_BRK) {
			printf("\nalguem ganhou e tem de carregar esc");
			game_state->done = 1;
			battle->kb_code = KEY_NONE;
			if (game_state->winner == 1) {
				printf("jogador ganhou e carregou esc");
				return HIGHSCORE_STATE;
			} else
				return MAIN_MENU_STATE;
		}
		return GAME_PLAY_STATE;
	}

	if (battle->kb_code == KEY_P_BRK) {
		game_state->pause = !game_state->pause;
		battle->kb_code = KEY_NONE;
		if (game_state->pause)
			printf("\nEntrou em pausa");
		else
			printf("\nSaiu de pausa");
	}

	if (game_state->pause) {

		if (battle->kb_code == KEY_ESC_BRK) {
			battle->kb_code = KEY_NONE;
			game_state->done = 1;
			return MAIN_MENU_STATE;
		}

	} else {
		if (battle->timer_cnt == 0) {
			game_state->turn_time_counter--;
		}

		if (game_state->turn == 0) {
			if (game_state->turn_time_counter == tempo_bot_espera) {
				bot_play(battle);
				printf("\nsaiu da bot");
			}
			return GAME_PLAY_STATE;
		}
		printf("\nturno do jogador");
		printf("\ntempo restante: %d", game_state->turn_time_counter);
		if (game_state->turn_time_counter == 0) {
			printf("\n counter chegou a 0, passou o turno");
			tempo_bot_espera = rand() % 3 + 7;
			game_state->hum.turns_missed++;
			if (game_state->hum.turns_missed == 3) {
				printf("saiu");
				game_state->winner = 2;
				return GAME_PLAY_STATE;
			} else {
				game_state->turn = !game_state->turn;
				printf("turno missed: %d", game_state->hum.turns_missed);
				game_state->turn_time_counter = TURN_TIME;
				return GAME_PLAY_STATE;
			}
		}

		switch (battle->kb_code) {

		case KEY_ARR_UP_BRK:
			if (game_state->com.tab.selected_y > 0) {
				game_state->com.tab.selected_y--;
			}
			battle->kb_code = KEY_NONE;
			break;

		case KEY_ARR_DOWN_BRK:
			if (game_state->com.tab.selected_y != 9) {
				game_state->com.tab.selected_y++;
			}
			battle->kb_code = KEY_NONE;
			break;

		case KEY_ARR_RIGHT_BRK:
			if (game_state->com.tab.selected_x != 9) {
				game_state->com.tab.selected_x++;
			}
			battle->kb_code = KEY_NONE;
			break;

		case KEY_ARR_LEFT_BRK:
			if (game_state->com.tab.selected_x > 0) {
				game_state->com.tab.selected_x--;
			}
			battle->kb_code = KEY_NONE;
			break;

		case KEY_ENTER_BRK:
			if (game_state->com.tab.tab_array[game_state->com.tab.selected_x][game_state->com.tab.selected_y]->hit) {

				return GAME_PLAY_STATE;
			} else {
				game_state->com.tab.tab_array[game_state->com.tab.selected_x][game_state->com.tab.selected_y]->hit = 1;
			}

			if (game_state->com.tab.tab_array[game_state->com.tab.selected_x][game_state->com.tab.selected_y]->t_part == WATER) {
				game_state->turn = !game_state->turn;
				tempo_bot_espera = rand() % 3 + 7;
				game_state->hum.shots_missed++;
			}

			game_state->hum.time_played += (TURN_TIME - game_state->turn_time_counter);

			updateShips(battle);

			game_state->winner = gameOver(battle);

			game_state->turn_time_counter = TURN_TIME;
			battle->kb_code = KEY_NONE;
			break;
		}
	}
	return GAME_PLAY_STATE;
}

void deleteGame(Battleship* battle) {
	free(game_state);
}

SetShipState* newPlaySetship() {

	printf("\n newPlaySetShip");

	tab_player = NULL;
	tab_computer = NULL;

	SetShipState* state = (SetShipState*) malloc(sizeof(SetShipState));

	state->done = 0;

//initialize player tab
	initShip(&state->tab);
//initialize computer tab
	initShip(&state->tab_com);

	randTabuleiro(&state->tab_com);

	state->ship_temp = &state->tab.ship_array[0];

	state->ship_map = loadBitmap("home/lcom/proj/img/mapanaves.bmp");

	state->ship_list = loadBitmap("home/lcom/proj/img/ship_list.bmp");
	return state;
}

void drawPlaySetship(Battleship* battle) {

	drawSetTabuleiro(300, 200, set_ship->tab, set_ship->ship_temp, set_ship->ship_map);

	drawListShipSet(750, 200, set_ship->tab.ship_on_board, set_ship->ship_list);
}

State updatePlaySetship(Battleship* battle) {

	switch (battle->kb_code) {
	case KEY_ARR_UP_BRK:
		printf("\n Up Arrow");
		if (set_ship->ship_temp->y_central != 0) {
			set_ship->ship_temp->y_central--;
		}
		battle->kb_code = KEY_NONE;
		break;

	case KEY_ARR_DOWN_BRK:
		printf("\n Down Arrow");
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
		printf("\n Left Arrow");
		if (set_ship->ship_temp->x_central != 0) {
			set_ship->ship_temp->x_central--;
		}
		battle->kb_code = KEY_NONE;
		break;

	case KEY_ARR_RIGHT_BRK:
		printf("\n Right Arrow");
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

	case KEY_ESC_BRK:
		printf("\n Esc");
		battle->kb_code = KEY_NONE;
		set_ship->done = 1;
		return MAIN_MENU_STATE;
		break;
	case KEY_ENTER_BRK:
		printf("\n Enter");
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

		battle->kb_code = KEY_NONE;
		break;

	case KEY_R_BRK:
		if (set_ship->ship_temp->t_ship == DEATH_STAR) {
			battle->kb_code = KEY_NONE;
			return GAME_PLAY_SETSHIP_STATE;
		}
		if (set_ship->ship_temp->t_ship == ESCAPE_SHUTTLE) {
					battle->kb_code = KEY_NONE;
					return GAME_PLAY_SETSHIP_STATE;
				}
		if (set_ship->ship_temp->direction == 'h' && set_ship->ship_temp->y_central + set_ship->ship_temp->size < 11) {
			set_ship->ship_temp->direction = 'v';
			for (i = 0; i < set_ship->ship_temp->size; i++) {
				set_ship->ship_temp->parts_array[i].direction = 'v';
			}

		} else if (set_ship->ship_temp->direction == 'v' && set_ship->ship_temp->x_central + set_ship->ship_temp->size < 11) {
			set_ship->ship_temp->direction = 'h';
			for (i = 0; i < set_ship->ship_temp->size; i++) {
				set_ship->ship_temp->parts_array[i].direction = 'h';
			}
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
	unsigned int i;
	ship_part* water = (ship_part*) malloc(sizeof(ship_part) * 100);
	for (i = 0; i < 100; i++) {
		water->t_part = WATER;
		water->t_ship = NOTHING;
		water->hit = 0;
		water->direction = 'v';
		tab->tab_array[i / 10][i % 10] = water;

		water++;
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
	tab->ship_array[0].parts_array[0].direction = 'v';
	tab->ship_array[0].parts_array[1].t_part = UPPER_RIGHT;
	tab->ship_array[0].parts_array[1].t_ship = DEATH_STAR;
	tab->ship_array[0].parts_array[1].hit = 0;
	tab->ship_array[0].parts_array[1].direction = 'v';
	tab->ship_array[0].parts_array[2].t_part = BOTTOM_LEFT;
	tab->ship_array[0].parts_array[2].t_ship = DEATH_STAR;
	tab->ship_array[0].parts_array[2].hit = 0;
	tab->ship_array[0].parts_array[2].direction = 'v';
	tab->ship_array[0].parts_array[3].t_part = BOTTOM_RIGHT;
	tab->ship_array[0].parts_array[3].t_ship = DEATH_STAR;
	tab->ship_array[0].parts_array[3].hit = 0;
	tab->ship_array[0].parts_array[3].direction = 'v';
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
	tab->ship_array[1].parts_array[0].direction = 'v';
	tab->ship_array[1].parts_array[1].t_part = SECOND;
	tab->ship_array[1].parts_array[1].t_ship = BATTLESHIP;
	tab->ship_array[1].parts_array[1].hit = 0;
	tab->ship_array[1].parts_array[1].direction = 'v';
	tab->ship_array[1].parts_array[2].t_part = THIRD;
	tab->ship_array[1].parts_array[2].t_ship = BATTLESHIP;
	tab->ship_array[1].parts_array[2].hit = 0;
	tab->ship_array[1].parts_array[2].direction = 'v';
	tab->ship_array[1].parts_array[3].t_part = FOURTH;
	tab->ship_array[1].parts_array[3].t_ship = BATTLESHIP;
	tab->ship_array[1].parts_array[3].hit = 0;
	tab->ship_array[1].parts_array[3].direction = 'v';
	tab->ship_array[1].parts_array[4].t_part = FIFTH;
	tab->ship_array[1].parts_array[4].t_ship = BATTLESHIP;
	tab->ship_array[1].parts_array[4].hit = 0;
	tab->ship_array[1].parts_array[4].direction = 'v';
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
	tab->ship_array[2].parts_array[0].direction = 'v';
	tab->ship_array[2].parts_array[1].t_part = SECOND;
	tab->ship_array[2].parts_array[1].t_ship = CRUSER;
	tab->ship_array[2].parts_array[1].hit = 0;
	tab->ship_array[2].parts_array[1].direction = 'v';
	tab->ship_array[2].parts_array[2].t_part = THIRD;
	tab->ship_array[2].parts_array[2].t_ship = CRUSER;
	tab->ship_array[2].parts_array[2].hit = 0;
	tab->ship_array[2].parts_array[2].direction = 'v';
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
	tab->ship_array[3].parts_array[0].direction = 'v';
	tab->ship_array[3].parts_array[1].t_part = SECOND;
	tab->ship_array[3].parts_array[1].t_ship = CRUSER;
	tab->ship_array[3].parts_array[1].hit = 0;
	tab->ship_array[3].parts_array[1].direction = 'v';
	tab->ship_array[3].parts_array[2].t_part = THIRD;
	tab->ship_array[3].parts_array[2].t_ship = CRUSER;
	tab->ship_array[3].parts_array[2].hit = 0;
	tab->ship_array[3].parts_array[2].direction = 'v';
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
	tab->ship_array[4].parts_array[0].direction = 'v';
	tab->ship_array[4].parts_array[1].t_part = SECOND;
	tab->ship_array[4].parts_array[1].t_ship = FIGHTER;
	tab->ship_array[4].parts_array[1].hit = 0;
	tab->ship_array[4].parts_array[1].direction = 'v';
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
	tab->ship_array[5].parts_array[0].direction = 'v';
	tab->ship_array[5].parts_array[1].t_part = SECOND;
	tab->ship_array[5].parts_array[1].t_ship = FIGHTER;
	tab->ship_array[5].parts_array[1].hit = 0;
	tab->ship_array[5].parts_array[1].direction = 'v';
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
	tab->ship_array[5].parts_array[0].direction = 'v';
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

//	printf("DeathStar x:%d\ty:%d", tab->ship_array[0].x_central,
//			tab->ship_array[0].y_central);
//	printf("Battleship x:%d\ty:%d", tab->ship_array[1].x_central,
//			tab->ship_array[1].y_central);
//	printf("Cruiser1 x:%d\ty:%d", tab->ship_array[2].x_central,
//			tab->ship_array[2].y_central);
//	printf("Cruiser2 x:%d\ty:%d", tab->ship_array[3].x_central,
//			tab->ship_array[3].y_central);
//	printf("Fighter x:%d\ty:%d", tab->ship_array[4].x_central,
//			tab->ship_array[4].y_central);
//	printf("Fighter x:%d\ty:%d", tab->ship_array[5].x_central,
//			tab->ship_array[5].y_central);
//	printf("Escape pod x:%d\ty:%d", tab->ship_array[6].x_central,
//			tab->ship_array[6].y_central);

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

int checkShips(Battleship* battle) {
	unsigned int i, m, count;
	for (i = 0; i < 7; i++) {
		count = 0;
		if (game_state->turn) {
			//Verifica naves do jogador
			for (m = 0; m < game_state->hum.tab.ship_array[i].size; m++) {
				if (game_state->hum.tab.ship_array[i].parts_array[m].hit) {
					count++;
				}
			}
			if (count != game_state->hum.tab.ship_array[i].nr_hits) {
				game_state->hum.tab.ship_array[i].nr_hits = count;
				if (game_state->hum.tab.ship_array[i].nr_hits == game_state->hum.tab.ship_array[i].size) {
					return i + 1;
				}
			}
		} else {
			//verifica naves bot
			for (m = 0; m < game_state->com.tab.ship_array[i].size; m++) {
				if (game_state->com.tab.ship_array[i].parts_array[m].hit) {
					count++;
				}
			}
			if (count != game_state->com.tab.ship_array[i].nr_hits) {
				game_state->com.tab.ship_array[i].nr_hits = count;
				if (game_state->com.tab.ship_array[i].nr_hits == game_state->com.tab.ship_array[i].size) {
					return i + 1;
				}
			}
		}
	}
	return 0;
}

void bot_play(Battleship* battle) {

	unsigned int selected_x;
	unsigned int selected_y;
	unsigned int bot_destroyed_ship = checkShips(battle);
	if (bot_destroyed_ship)
		printf("\nDestruiu humano:%d", bot_destroyed_ship);

	if (!game_state->ai_comp.previous_hit) { // Se antes falhou o hit,
		printf("\nEstado 1");
		selected_x = rand() % 10; // agora vai fazer random do sitio onde jogar
		selected_y = rand() % 10;
		game_state->ai_comp.last_x_hit = selected_x;
		game_state->ai_comp.last_y_hit = selected_y;
	} else if (bot_destroyed_ship) {
		printf("\nEstado 2");
		selected_x = rand() % 10; // agora vai fazer random do sitio onde jogar
		selected_y = rand() % 10;
		game_state->ai_comp.last_x_hit = selected_x;
		game_state->ai_comp.last_y_hit = selected_y;

	} else { // Se acertou o ultimo hit mas nao destruiu um ship
		printf("\nEstado 3");
		if (game_state->ai_comp.direction == 'n') {
			//se ainda nao tinha direcao definida porque
			unsigned int dir = rand() % 2; //era a 1a vez que tinha acertado, decide
			if (dir) //direcao para onde acha que o navio esta
				game_state->ai_comp.direction = 'h';
			else
				game_state->ai_comp.direction = 'v';
		}

		if (game_state->ai_comp.direction == 'h') { //Quando chega aqui, ja tem dir. definida
//			printf("\ndirecao horizontal!");
			if ((game_state->ai_comp.orientation != 0) && (game_state->ai_comp.orientation != 1)) { // E se ainda nao tinha orientacao, define-a
				if (game_state->ai_comp.last_x_hit == 9) {
					game_state->ai_comp.orientation = 0;
					game_state->ai_comp.orientation_was_inverted = 1;

				} else if (game_state->ai_comp.last_x_hit == 0) {
					game_state->ai_comp.orientation = 1;
					game_state->ai_comp.orientation_was_inverted = 1;

				} else {
					game_state->ai_comp.orientation = rand() % 2;

				}
			}
			if (game_state->ai_comp.orientation == 1) { // ORIENTATION = 1 E FORWARD, ORIENTATION = 0 E BACKWARD
//				printf("\n orientacao 1");
				if (game_state->ai_comp.last_x_hit < 9) {
					selected_x = game_state->ai_comp.last_x_hit + 1;
					selected_y = game_state->ai_comp.last_y_hit;
					game_state->ai_comp.last_x_hit = selected_x;

//					printf("\ny>0");
				} else if (!game_state->ai_comp.orientation_was_inverted) { // se nunca inverteu orientacao
					game_state->ai_comp.orientation = !game_state->ai_comp.orientation; //inverte-a
					game_state->ai_comp.orientation_was_inverted = 1;
					selected_y = game_state->ai_comp.last_y_hit;
					selected_x = game_state->ai_comp.last_x_hit;
//					printf("dir inv");
				} else { // se ja tiver invertido alguma vez a orientacao
					selected_x = rand() % 10;
					selected_y = rand() % 10;
					game_state->ai_comp.last_x_hit = selected_x;
					game_state->ai_comp.last_y_hit = selected_y;
					game_state->ai_comp.orientation_was_inverted = 0;
					game_state->ai_comp.direction = 'n'; // dar reset a direcao, para ser desconhecida
					game_state->ai_comp.orientation = 2; // dar reset a orientacao, para ser desconhecida
//					printf("y=0");
				}
			} else if (game_state->ai_comp.orientation == 0) {
//				printf("\n orientacao 0");
				if (game_state->ai_comp.last_x_hit > 0) {
					selected_x = game_state->ai_comp.last_x_hit - 1;
					selected_y = game_state->ai_comp.last_y_hit;
					game_state->ai_comp.last_x_hit = selected_x;

//					printf("\ny>0");
				} else if (!game_state->ai_comp.orientation_was_inverted) { // se nunca inverteu orientacao
					game_state->ai_comp.orientation = !game_state->ai_comp.orientation; //inverte-a
					game_state->ai_comp.orientation_was_inverted = 1;
					selected_y = game_state->ai_comp.last_y_hit;
					selected_x = game_state->ai_comp.last_x_hit;
//					printf("dir inv");
				} else { // se ja tiver invertido alguma vez a orientacao
					selected_x = rand() % 10;
					selected_y = rand() % 10;
					game_state->ai_comp.last_x_hit = selected_x;
					game_state->ai_comp.last_y_hit = selected_y;
					game_state->ai_comp.orientation_was_inverted = 0;
					game_state->ai_comp.direction = 'n'; // dar reset a direcao, para ser desconhecida
					game_state->ai_comp.orientation = 2; // dar reset a orientacao, para ser desconhecida
//					printf("y=0");
				}
			}
		}
		if (game_state->ai_comp.direction == 'v') { //Quando chega aqui, ja tem dir. definida
//			printf("\ndireccao vertical!");
			if ((game_state->ai_comp.orientation != 0) && (game_state->ai_comp.orientation != 1)) { // E se ainda nao tinha orientacao, define-a
				if (game_state->ai_comp.last_y_hit == 9) {
					game_state->ai_comp.orientation = 0;
					game_state->ai_comp.orientation_was_inverted = 1;

				} else if (game_state->ai_comp.last_y_hit == 0) {
					game_state->ai_comp.orientation = 1;
					game_state->ai_comp.orientation_was_inverted = 1;

				} else {
					game_state->ai_comp.orientation = rand() % 2;

				}
			}
			if (game_state->ai_comp.orientation == 1) { // ORIENTATION = 1 E FORWARD, ORIENTATION = 0 E BACKWARD
//				printf("\n orientacao 1");
				if (game_state->ai_comp.last_y_hit < 9) {
					selected_y = game_state->ai_comp.last_y_hit + 1;
					selected_x = game_state->ai_comp.last_x_hit;
					game_state->ai_comp.last_y_hit = selected_y;

//					printf("\ny>0");
				} else if (!game_state->ai_comp.orientation_was_inverted) { // se nunca inverteu orientacao
					game_state->ai_comp.orientation = !game_state->ai_comp.orientation; //inverte-a
					game_state->ai_comp.orientation_was_inverted = 1;
					selected_y = game_state->ai_comp.last_y_hit;
					selected_x = game_state->ai_comp.last_x_hit;
//					printf("dir inv");
				} else { // se ja tiver invertido alguma vez a orientacao
					selected_x = rand() % 10;
					selected_y = rand() % 10;
					game_state->ai_comp.last_x_hit = selected_x;
					game_state->ai_comp.last_y_hit = selected_y;
					game_state->ai_comp.orientation_was_inverted = 0;
					game_state->ai_comp.direction = 'n'; // dar reset a direcao, para ser desconhecida
					game_state->ai_comp.orientation = 2; // dar reset a orientacao, para ser desconhecida
//					printf("y=0");
				}
			} else if (game_state->ai_comp.orientation == 0) {
//				printf("\n orientacao 0");
				if (game_state->ai_comp.last_y_hit > 0) {
					selected_y = game_state->ai_comp.last_y_hit - 1;
					selected_x = game_state->ai_comp.last_x_hit;
					game_state->ai_comp.last_y_hit = selected_y;
//					printf("\ny>0");
				} else if (!game_state->ai_comp.orientation_was_inverted) { // se nunca inverteu orientacao
					game_state->ai_comp.orientation = !game_state->ai_comp.orientation; //inverte-a
					game_state->ai_comp.orientation_was_inverted = 1;
					selected_y = game_state->ai_comp.last_y_hit;
					selected_x = game_state->ai_comp.last_x_hit;
//					printf("dir inv");
				} else { // se ja tiver invertido alguma vez a orientacao
					selected_x = rand() % 10;
					selected_y = rand() % 10;
					game_state->ai_comp.last_x_hit = selected_x;
					game_state->ai_comp.last_y_hit = selected_y;
					game_state->ai_comp.orientation_was_inverted = 0;
					game_state->ai_comp.direction = 'n'; // dar reset a direcao, para ser desconhecida
					game_state->ai_comp.orientation = 2; // dar reset a orientacao, para ser desconhecida
//					printf("y=0");
				}
			}
		}

	}

	if (game_state->hum.tab.tab_array[selected_x][selected_y]->hit) { //peca ja tinha sido atingida antes
		printf("\nPeca ja estava atingida");
		return;
	} else { // Se a peca ainda nao tinha sido atingida, passa a estar -> hit = 1
		printf("\nPeca atingida pela 1a vez");
		game_state->hum.tab.tab_array[selected_x][selected_y]->hit = 1;
		game_state->ai_comp.last_x_hit = selected_x;
		game_state->ai_comp.last_y_hit = selected_y;
	}

	if (game_state->hum.tab.tab_array[selected_x][selected_y]->t_part == WATER) {
		printf("\nAgua!");
		game_state->ai_comp.previous_hit = 0;
		game_state->turn = !game_state->turn;
	} else {
		printf("\nShip");
		game_state->ai_comp.previous_hit = 1;
	}

	game_state->turn_time_counter = TURN_TIME;
	updateShips(battle);
	game_state->winner = gameOver(battle);
}

unsigned int calculaScore(Battleship* battle) {
	return battle->highscore_winner = 1000 / (0.05 * game_state->hum.shots_missed + 0.01 * game_state->hum.time_played);
}

int gameOver(Battleship* battle) {
	unsigned int i, count = 0;
	for (i = 0; i < 7; i++) {
		if (game_state->hum.tab.ship_array[i].destroyed) {
			count++;
		}
	}
	if (count == 7) {
		return 1;
	}
	for (i = 0; i < 7; i++) {
		if (game_state->com.tab.ship_array[i].destroyed) {
			count++;
		}
	}
	if (count == 7) {
		return 2;
	}
	return 0;
}

void updateShips(Battleship* battle) {
	unsigned int i, m, count_hum, count_com;
	for (i = 0; i < 7; i++) {
		count_hum = 0;
		count_com = 0;
		//Actualiza contador das partes hit do jogador e computador
		for (m = 0; m < game_state->hum.tab.ship_array[i].size; m++) {
			if (game_state->hum.tab.ship_array[i].parts_array[m].hit) {
				count_hum++;
			}
			if (game_state->com.tab.ship_array[i].parts_array[m].hit) {
				count_com++;
			}
		}
		//actualiza  a flag
		if (count_com == game_state->com.tab.ship_array[i].size) {
			game_state->com.tab.ship_array[i].destroyed = 1;
		}
		if (count_hum == game_state->hum.tab.ship_array[i].size) {
			game_state->hum.tab.ship_array[i].destroyed = 1;
		}
	}
}
