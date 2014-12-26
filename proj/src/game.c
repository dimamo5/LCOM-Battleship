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
	water->t_ship = WATER;
	water->hit = 0;
	water->selected = 0;
	unsigned int i;
	for (i = 0; i <= 100; i++) { //preenche o tabuleiro com agua
		state->tab.tab_array[i / 10][i % 10] = water;
	}
	state->ship_selected = 1;

	//Inicializacao dos butoes
	state->fighter = newButton(700, 100, 845, 225, BLUE);
	state->death_star = newButton(700, 225, 845, 350, BLUE);
	state->cruser = newButton(700, 350, 845, 475, BLUE);
	state->escape_pod = newButton(700, 475, 845, 600, BLUE);
	state->battleship = newButton(845, 100, 990, 350, BLUE);
	state->cruser_2 = newButton(845, 225, 990, 745, BLUE);
	state->escape_pod_2 = newButton(845, 350, 990, 475, BLUE);

	state->ship_selection = loadBitmap("home/lcom/proj/img/shipselection.bmp");

	return state;
}
void drawPlaySetship(Battleship* battle) {
	drawBitmap(((SetShipState*) battle->state)->ship_selection, 700, 100);
	drawRectangle(((SetShipState*) battle->state)->fighter);
	drawRectangle(((SetShipState*) battle->state)->death_star);
	drawRectangle(((SetShipState*) battle->state)->cruser);
	drawRectangle(((SetShipState*) battle->state)->escape_pod);
	drawRectangle(((SetShipState*) battle->state)->battleship);
	drawRectangle(((SetShipState*) battle->state)->cruser_2);
	drawRectangle(((SetShipState*) battle->state)->escape_pod_2);
	draw_board(200,200,BIG);
}
State updatePlaySetship(Battleship* battle) {
	if (((SetShipState*) battle->state)->ship_selected == -1) {
		updateSetShipBoard((SetShipState*) battle->state);

	} else if (battle->kb_code == KEY_ARR_UP) {

		printf("keyup");
		//desseleciona todos os botoes quando uma tecla é premida
		((SetShipState*) battle->state)->fighter->mouse_hover = 0;
		((SetShipState*) battle->state)->death_star->mouse_hover = 0;
		((SetShipState*) battle->state)->cruser->mouse_hover = 0;
		((SetShipState*) battle->state)->escape_pod->mouse_hover = 0;
		((SetShipState*) battle->state)->battleship->mouse_hover = 0;
		((SetShipState*) battle->state)->cruser_2->mouse_hover = 0;
		((SetShipState*) battle->state)->escape_pod_2->mouse_hover = 0;
		battle->kb_code = KEY_NONE;
		((SetShipState*) battle->state)->ship_selected--;
		printf("\nsel:%d", ((SetShipState*) battle->state)->ship_selected);

	} else if (battle->kb_code == KEY_ARR_DOWN) {

		printf("keydown");
		//desseleciona todos os botoes quando uma tecla é premida
		((SetShipState*) battle->state)->fighter->mouse_hover = 0;
		((SetShipState*) battle->state)->death_star->mouse_hover = 0;
		((SetShipState*) battle->state)->cruser->mouse_hover = 0;
		((SetShipState*) battle->state)->escape_pod->mouse_hover = 0;
		((SetShipState*) battle->state)->battleship->mouse_hover = 0;
		((SetShipState*) battle->state)->cruser_2->mouse_hover = 0;
		((SetShipState*) battle->state)->escape_pod_2->mouse_hover = 0;
		((SetShipState*) battle->state)->ship_selected++;
		printf("\nsel:%d", ((SetShipState*) battle->state)->ship_selected);
		battle->kb_code = KEY_NONE;

	} else if (battle->kb_code == KEY_ENTER) {
		printf("\nsel:%d", ((SetShipState*) battle->state)->ship_selected);
		printf("keyenter");
		battle->kb_code = KEY_NONE;
		((SetShipState*) battle->state)->ship_selected = -1;
		getShip((SetShipState*) battle->state);
	}

	switch (((SetShipState*) battle->state)->ship_selected) {
	case 1:
		/**--------------------------------------------------------------
		 * HÁ 3 DIAS PARADO NESTA BOSTA DE BUG!!!!! JA ESTOU A FRITAR
		 * ---------------------------------------------------------------
		 */

		if (((SetShipState*) battle->state)->fighter->available) {
			((SetShipState*) battle->state)->fighter->mouse_hover = 1;
		} else
			((SetShipState*) battle->state)->ship_selected++;
		break;
	case 2:
		if (((SetShipState*) battle->state)->death_star->available)
			((SetShipState*) battle->state)->death_star->mouse_hover = 1;
		else
			((SetShipState*) battle->state)->ship_selected++;
		break;
	case 3:
		if (((SetShipState*) battle->state)->cruser->available)
			((SetShipState*) battle->state)->cruser->mouse_hover = 1;
		else
			((SetShipState*) battle->state)->ship_selected++;
		break;
	case 4:
		if (((SetShipState*) battle->state)->escape_pod->available)
			((SetShipState*) battle->state)->escape_pod->mouse_hover = 1;
		else
			((SetShipState*) battle->state)->ship_selected++;
		break;
	case 5:
		if (((SetShipState*) battle->state)->battleship->available)
			((SetShipState*) battle->state)->battleship->mouse_hover = 1;
		else
			((SetShipState*) battle->state)->ship_selected++;
		break;
	case 6:
		if (((SetShipState*) battle->state)->cruser_2->available)
			((SetShipState*) battle->state)->cruser_2->mouse_hover = 1;
		else
			((SetShipState*) battle->state)->ship_selected++;
		break;
	case 7:
		if (((SetShipState*) battle->state)->escape_pod_2->available)
			((SetShipState*) battle->state)->escape_pod_2->mouse_hover = 1;
		else
			((SetShipState*) battle->state)->ship_selected++;
		break;
	default:
		((SetShipState*) battle->state)->ship_selected = 1;
		break;
	}
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

void getShip(SetShipState* state) {
	state->ship_temp.nr_hits = 0; //Inicializacao de variaveis do ship
	state->ship_temp.destroyed = 0; //Iguais para todas a naves no inicio do jogo
	state->ship_temp.x_central = 0;
	state->ship_temp.y_central = 0;
	state->ship_temp.direction = 'h';

	switch (state->ship_selected) {
	case 1:
		state->ship_temp.t_ship = FIGHTER;
		state->ship_temp.size = 1;
		state->ship_temp.ship_array = malloc(
				sizeof(ship_part) * state->ship_temp.size);
		ship_part temp_1;
		temp_1.hit = 0;
		temp_1.selected = 0;
		temp_1.t_part = FIRST;
		*state->ship_temp.ship_array = temp_1;
	}

}
void updateSetShipBoard(SetShipState* state) {

}

