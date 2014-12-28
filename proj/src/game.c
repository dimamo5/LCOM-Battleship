#include "game.h"
#include "battleship.h"
#include "keyboard_mouse.h"
#include "graphics.h"

game* newGame() {
}
void drawGame(Battleship* battle) {

}
State updateGame(Battleship* battle) {

}
void deleteGame(Battleship* battle) {

}

SetShipState* newPlaySetship() {
	SetShipState* state = (SetShipState*) malloc(sizeof(SetShipState));
	ship_part* water = (ship_part*) malloc(sizeof(ship_part));
	water->t_part = WATER;
	water->t_ship = NOTHING;
	water->hit = 0;
	unsigned int i;
	for (i = 0; i <= 100; i++) { //preenche o tabuleiro com agua
		state->tab.tab_array[i / 10][i % 10] = water;
	}
	state->tab.selected_x = -1;
	state->tab.selected_y = -1;
	state->tab.ship_on_board = 0;
//	state->ship_selected = 1;

	initShip(state);

	state->ship_temp = &state->tab.ship_array[0];

	//Inicializacao dos butoes
	state->fighter = newButton(700, 100, 845, 225, BLUE);
	state->death_star = newButton(700, 225, 845, 350, BLUE);
	state->cruser = newButton(700, 350, 845, 475, BLUE);
	state->escape_pod = newButton(700, 475, 845, 600, BLUE);
	state->battleship = newButton(845, 100, 990, 350, BLUE);
	state->cruser_2 = newButton(845, 225, 990, 745, BLUE);
	state->escape_pod_2 = newButton(845, 350, 990, 475, BLUE);

	state->ship_map = loadBitmap("home/lcom/proj/img/mapanaves.bmp");

	return state;
}
void drawPlaySetship(Battleship* battle) {
//	drawBitmap(((SetShipState*) battle->state)->ship_selection, 700, 100);
//	drawRectangle(((SetShipState*) battle->state)->fighter);
//	drawRectangle(((SetShipState*) battle->state)->death_star);
//	drawRectangle(((SetShipState*) battle->state)->cruser);
//	drawRectangle(((SetShipState*) battle->state)->escape_pod);
//	drawRectangle(((SetShipState*) battle->state)->battleship);
//	drawRectangle(((SetShipState*) battle->state)->cruser_2);
//	drawRectangle(((SetShipState*) battle->state)->escape_pod_2);
	drawSetTabuleiro(200, 200, ((SetShipState*) battle->state)->tab, ((SetShipState*) battle->state)->ship_temp);
//	ship_part t;
//	t.t_part = FIFTH;
//	t.t_ship = BATTLESHIP;
//	t.hit = 0;
//	Bitmap * b = loadBitmap("home/lcom/proj/img/mapanaves.bmp");
//	drawQuadricula(50, 50, t, b);
}

State updatePlaySetship(Battleship* battle) {
	switch (battle->kb_code) {
	case KEY_ARR_UP_BRK:
		if (((SetShipState*) battle->state)->ship_temp->y_central != 0) {
			((SetShipState*) battle->state)->ship_temp->y_central--;
		}
	case KEY_ARR_DOWN_BRK:
		if (((SetShipState*) battle->state)->ship_temp->direction == 'h') {
			if (((SetShipState*) battle->state)->ship_temp->y_central + ((SetShipState*) battle->state)->ship_temp->size != 9) {
				((SetShipState*) battle->state)->ship_temp->y_central++;
			}

		} else if (((SetShipState*) battle->state)->ship_temp->y_central) {
			((SetShipState*) battle->state)->ship_temp->y_central++;
		}

		break;

	case KEY_ARR_LEFT_BRK:
		if (((SetShipState*) battle->state)->ship_temp->x_central != 0) {
			((SetShipState*) battle->state)->ship_temp->x_central--;
		}
		break;

	case KEY_ARR_RIGHT_BRK:
		if (((SetShipState*) battle->state)->ship_temp->direction == 'v') {
			if (((SetShipState*) battle->state)->ship_temp->x_central + ((SetShipState*) battle->state)->ship_temp->size != 9) {
				((SetShipState*) battle->state)->ship_temp->x_central++;
			}

		} else if (((SetShipState*) battle->state)->ship_temp->x_central) {
			((SetShipState*) battle->state)->ship_temp->x_central++;
		}
		break;
	}
	battle->kb_code = KEY_NONE;
//	switch (((SetShipState*) battle->state)->ship_selected) {
//	case 1:
//		/**--------------------------------------------------------------
//		 * HÁ 3 DIAS PARADO NESTA BOSTA DE BUG!!!!! JA ESTOU A FRITAR
//		 * ---------------------------------------------------------------
//		 */
//
//		if (((SetShipState*) battle->state)->fighter->available) {
//			((SetShipState*) battle->state)->fighter->mouse_hover = 1;
//		} else
//			((SetShipState*) battle->state)->ship_selected++;
//		break;
//	case 2:
//		if (((SetShipState*) battle->state)->death_star->available)
//			((SetShipState*) battle->state)->death_star->mouse_hover = 1;
//		else
//			((SetShipState*) battle->state)->ship_selected++;
//		break;
//	case 3:
//		if (((SetShipState*) battle->state)->cruser->available)
//			((SetShipState*) battle->state)->cruser->mouse_hover = 1;
//		else
//			((SetShipState*) battle->state)->ship_selected++;
//		break;
//	case 4:
//		if (((SetShipState*) battle->state)->escape_pod->available)
//			((SetShipState*) battle->state)->escape_pod->mouse_hover = 1;
//		else
//			((SetShipState*) battle->state)->ship_selected++;
//		break;
//	case 5:
//		if (((SetShipState*) battle->state)->battleship->available)
//			((SetShipState*) battle->state)->battleship->mouse_hover = 1;
//		else
//			((SetShipState*) battle->state)->ship_selected++;
//		break;
//	case 6:
//		if (((SetShipState*) battle->state)->cruser_2->available)
//			((SetShipState*) battle->state)->cruser_2->mouse_hover = 1;
//		else
//			((SetShipState*) battle->state)->ship_selected++;
//		break;
//	case 7:
//		if (((SetShipState*) battle->state)->escape_pod_2->available)
//			((SetShipState*) battle->state)->escape_pod_2->mouse_hover = 1;
//		else
//			((SetShipState*) battle->state)->ship_selected++;
//		break;
//	default:
//		((SetShipState*) battle->state)->ship_selected = 1;
//		break;
//	}
}

void deletePlaySetship(Battleship* battle) {
	deleteButton(((SetShipState*) battle->state)->fighter);
	deleteButton(((SetShipState*) battle->state)->death_star);
	deleteButton(((SetShipState*) battle->state)->cruser);
	deleteButton(((SetShipState*) battle->state)->escape_pod);
	deleteButton(((SetShipState*) battle->state)->battleship);
	deleteButton(((SetShipState*) battle->state)->cruser_2);
	deleteButton(((SetShipState*) battle->state)->escape_pod_2);
	free(battle->state);

}

void initShip(SetShipState* state) {
//	state->ship_temp.nr_hits = 0; //Inicializacao de variaveis do ship
//	state->ship_temp.destroyed = 0; //Iguais para todas a naves no inicio do jogo
//	state->ship_temp.x_central = 0;
//	state->ship_temp.y_central = 0;
//	state->ship_temp.direction = 'h';
//
//	switch (state->ship_selected) {
//	case 1:
//		state->ship_temp.t_ship = FIGHTER;
//		state->ship_temp.size = 1;
//		state->ship_temp.ship_array = malloc(
//				sizeof(ship_part) * state->ship_temp.size);
//		ship_part temp_1;
//		temp_1.hit = 0;
//		temp_1.selected = 0;
//		temp_1.t_part = FIRST;
//		*state->ship_temp.ship_array = temp_1;
//	}

	/**
	 * DEATH STAR INIT
	 */
	state->tab.ship_array[0].parts_array = malloc(sizeof(ship_part) * 4);
	state->tab.ship_array[0].t_ship = DEATH_STAR;
	state->tab.ship_array[0].parts_array[0].t_part = UPPER_LEFT;
	state->tab.ship_array[0].parts_array[0].t_ship = DEATH_STAR;
	state->tab.ship_array[0].parts_array[0].hit = 0;
	state->tab.ship_array[0].parts_array[1].t_part = UPPER_RIGHT;
	state->tab.ship_array[0].parts_array[1].t_ship = DEATH_STAR;
	state->tab.ship_array[0].parts_array[1].hit = 0;
	state->tab.ship_array[0].parts_array[2].t_part = BOTTOM_LEFT;
	state->tab.ship_array[0].parts_array[2].t_ship = DEATH_STAR;
	state->tab.ship_array[0].parts_array[2].hit = 0;
	state->tab.ship_array[0].parts_array[3].t_part = BOTTOM_RIGHT;
	state->tab.ship_array[0].parts_array[3].t_ship = DEATH_STAR;
	state->tab.ship_array[0].parts_array[3].hit = 0;
	state->tab.ship_array[0].nr_hits = 0;
	state->tab.ship_array[0].destroyed = 0;
	state->tab.ship_array[0].x_central = 0;
	state->tab.ship_array[0].y_central = 0;
	state->tab.ship_array[0].direction = 'v';
	state->tab.ship_array[0].size = 4;

	/**
	 * BATTLESHIP INIT
	 */
	state->tab.ship_array[1].parts_array = malloc(sizeof(ship_part) * 5);
	state->tab.ship_array[1].t_ship = BATTLESHIP;
	state->tab.ship_array[1].parts_array[0].t_part = FIRST;
	state->tab.ship_array[1].parts_array[0].t_ship = BATTLESHIP;
	state->tab.ship_array[1].parts_array[0].hit = 0;
	state->tab.ship_array[1].parts_array[1].t_part = SECOND;
	state->tab.ship_array[1].parts_array[1].t_ship = BATTLESHIP;
	state->tab.ship_array[1].parts_array[1].hit = 0;
	state->tab.ship_array[1].parts_array[2].t_part = THIRD;
	state->tab.ship_array[1].parts_array[2].t_ship = BATTLESHIP;
	state->tab.ship_array[1].parts_array[2].hit = 0;
	state->tab.ship_array[1].parts_array[3].t_part = FOURTH;
	state->tab.ship_array[1].parts_array[3].t_ship = BATTLESHIP;
	state->tab.ship_array[1].parts_array[3].hit = 0;
	state->tab.ship_array[1].parts_array[4].t_part = FIFTH;
	state->tab.ship_array[1].parts_array[4].t_ship = BATTLESHIP;
	state->tab.ship_array[1].parts_array[4].hit = 0;
	state->tab.ship_array[1].nr_hits = 0;
	state->tab.ship_array[1].destroyed = 0;
	state->tab.ship_array[1].x_central = 0;
	state->tab.ship_array[1].y_central = 0;
	state->tab.ship_array[1].direction = 'v';
	state->tab.ship_array[1].size = 5;

	/**
	 * CRUSER 1 INIT
	 */
	state->tab.ship_array[2].parts_array = malloc(sizeof(ship_part) * 3);
	state->tab.ship_array[2].t_ship = CRUSER;
	state->tab.ship_array[2].parts_array[0].t_part = FIRST;
	state->tab.ship_array[2].parts_array[0].t_ship = CRUSER;
	state->tab.ship_array[2].parts_array[0].hit = 0;
	state->tab.ship_array[2].parts_array[1].t_part = SECOND;
	state->tab.ship_array[2].parts_array[1].t_ship = CRUSER;
	state->tab.ship_array[2].parts_array[1].hit = 0;
	state->tab.ship_array[2].parts_array[2].t_part = THIRD;
	state->tab.ship_array[2].parts_array[2].t_ship = CRUSER;
	state->tab.ship_array[2].parts_array[2].hit = 0;
	state->tab.ship_array[2].nr_hits = 0;
	state->tab.ship_array[2].destroyed = 0;
	state->tab.ship_array[2].x_central = 0;
	state->tab.ship_array[2].y_central = 0;
	state->tab.ship_array[2].direction = 'v';
	state->tab.ship_array[2].size = 3;

	/**
	 * CRUSER 2 INIT
	 */

	state->tab.ship_array[3].parts_array = malloc(sizeof(ship_part) * 3);
	state->tab.ship_array[3].t_ship = CRUSER;
	state->tab.ship_array[3].parts_array[0].t_part = FIRST;
	state->tab.ship_array[3].parts_array[0].t_ship = CRUSER;
	state->tab.ship_array[3].parts_array[0].hit = 0;
	state->tab.ship_array[3].parts_array[1].t_part = SECOND;
	state->tab.ship_array[3].parts_array[1].t_ship = CRUSER;
	state->tab.ship_array[3].parts_array[1].hit = 0;
	state->tab.ship_array[3].parts_array[2].t_part = THIRD;
	state->tab.ship_array[3].parts_array[2].t_ship = CRUSER;
	state->tab.ship_array[3].parts_array[2].hit = 0;
	state->tab.ship_array[3].nr_hits = 0;
	state->tab.ship_array[3].destroyed = 0;
	state->tab.ship_array[3].x_central = 0;
	state->tab.ship_array[3].y_central = 0;
	state->tab.ship_array[3].direction = 'v';
	state->tab.ship_array[3].size = 3;

	/**
	 * FIGHTER 2 INIT
	 */
	state->tab.ship_array[4].parts_array = malloc(sizeof(ship_part) * 2);
	state->tab.ship_array[4].t_ship = FIGHTER;
	state->tab.ship_array[4].parts_array[0].t_part = FIRST;
	state->tab.ship_array[4].parts_array[0].t_ship = FIGHTER;
	state->tab.ship_array[4].parts_array[0].hit = 0;
	state->tab.ship_array[4].parts_array[1].t_part = SECOND;
	state->tab.ship_array[4].parts_array[1].t_ship = FIGHTER;
	state->tab.ship_array[4].parts_array[1].hit = 0;
	state->tab.ship_array[4].nr_hits = 0;
	state->tab.ship_array[4].destroyed = 0;
	state->tab.ship_array[4].x_central = 0;
	state->tab.ship_array[4].y_central = 0;
	state->tab.ship_array[4].direction = 'v';
	state->tab.ship_array[4].size = 2;

	/**
	 * FIGHTER 2 INIT
	 */
	state->tab.ship_array[5].parts_array = malloc(sizeof(ship_part) * 2);
	state->tab.ship_array[5].t_ship = FIGHTER;
	state->tab.ship_array[5].parts_array[0].t_part = FIRST;
	state->tab.ship_array[5].parts_array[0].t_ship = FIGHTER;
	state->tab.ship_array[5].parts_array[0].hit = 0;
	state->tab.ship_array[5].parts_array[1].t_part = SECOND;
	state->tab.ship_array[5].parts_array[1].t_ship = FIGHTER;
	state->tab.ship_array[5].parts_array[1].hit = 0;
	state->tab.ship_array[5].nr_hits = 0;
	state->tab.ship_array[5].destroyed = 0;
	state->tab.ship_array[5].x_central = 0;
	state->tab.ship_array[5].y_central = 0;
	state->tab.ship_array[5].direction = 'v';
	state->tab.ship_array[5].size = 2;

	/**
	 * ESCAPE POD INIT
	 */
	state->tab.ship_array[6].parts_array = malloc(sizeof(ship_part) * 2);
	state->tab.ship_array[6].t_ship = ESCAPE_SHUTTLE;
	state->tab.ship_array[6].parts_array[0].t_part = FIRST;
	state->tab.ship_array[6].parts_array[0].t_ship = ESCAPE_SHUTTLE;
	state->tab.ship_array[6].parts_array[0].hit = 0;
	state->tab.ship_array[6].nr_hits = 0;
	state->tab.ship_array[6].destroyed = 0;
	state->tab.ship_array[6].x_central = 0;
	state->tab.ship_array[6].y_central = 0;
	state->tab.ship_array[6].direction = 'v';
	state->tab.ship_array[6].size = 1;

}

void updateSetShipBoard(SetShipState* state) {

}

