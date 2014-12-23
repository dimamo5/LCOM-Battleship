#include "game.h"
#include "battleship.h"
#include "keyboard_mouse.h"

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
	water->hit = 0;
	water->selected = 0;
	unsigned int i;
	for (i = 0; i <= 100; i++) { //preenche o tabuleiro com agua
		state->tab.tab_array[i / 10][i % 10] = water;
	}
	state->ship_selected = 1;
}
void drawPlaySetship(Battleship* battle) {

}
State updatePlaySetship(Battleship* battle) {
	if (((SetShipState*) battle->state)->ship_selected == -1) {
		updateSetShipBoard((SetShipState*) battle->state);
	} else if (battle->kb_code == KEY_ARR_UP) {
		battle->kb_code == KEY_NONE;
		((SetShipState*) battle->state)->ship_selected--;
	} else if (battle->kb_code == KEY_ARR_DOWN) {
		((SetShipState*) battle->state)->ship_selected--;
		battle->kb_code == KEY_NONE;
	} else if (battle->kb_code == KEY_ENTER) {
		battle->kb_code == KEY_NONE;
		((SetShipState*) battle->state)->ship_selected = -1;
		getShip((SetShipState*) battle->state);
	}

	switch (((SetShipState*) battle->state)->ship_selected) {
	case 1:
		if (((SetShipState*) battle->state)->fighter->available)
			((SetShipState*) battle->state)->fighter->mouse_hover = 1;
		else
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
	}
}

void deletePlaySetship(Battleship* battle) {

}

void getShip(SetShipState* state) {
	state->ship_temp.nr_hits = 0;		//Inicializacao de variaveis do ship
	state->ship_temp.destroyed = 0;		//Iguais para todas a naves no inicio do jogo
	state->ship_temp.x_central = 0;
	state->ship_temp.y_central = 0;
	state->ship_temp.direction = 'h';

	switch (state->ship_selected) {
	case 1:
		state->ship_temp.t_ship = FIGHTER;
		state->ship_temp.size = 1;
		state->ship_temp.ship_array = malloc(sizeof(ship_part) * state->ship_temp.size);
		ship_part temp_1;
		temp_1.hit=0;
		temp_1.selected=0;
		temp_1.t_part=FIRST;
		*state->ship_temp.ship_array=temp_1;
	}

}
void updateSetShipBoard(SetShipState* state) {

}

