#include "MainMenu.h"
#include "graphics.h"
#include "bitmap.h"
#include "keyboard_mouse.h"

MainMenuState* newMainMenuState() {

	enable_packets();

	//Criacao do estado e inicializacao das suas variaveis e botoes

	MainMenuState* state = (MainMenuState*) malloc(sizeof(MainMenuState));
	state->background = loadBitmap("home/lcom/proj/img/mainmenu.bmp");
	state->done = 0;

	// PLAY AI BUTTON
	int width = 435, height = 80;
	int x1 = 287;
	int x2 = x1 + width;
	int y1 = 256;
	int y2 = y1 + height;
	state->play_ai_button = newButton(x1, y1, x2, y2, WHITE);
	// WHITE - this is the button color, but it might be useless here
	// since drawRectange is where we choose the border color

	// PLAY PVP BUTTON
	width = 583;
	height = 79;
	x1 = 212;
	x2 = x1 + width;
	y1 = 341;
	y2 = y1 + height;
	state->play_pvp_button = newButton(x1, y1, x2, y2, WHITE);

	// INSTRUCTIONS BUTTON
	width = 409;
	height = 79;
	x1 = 287;
	x2 = x1 + width;
	y1 = 449;
	y2 = y1 + height;
	state->instructions_button = newButton(x1, y1, x2, y2, WHITE);

	// HIGHSCORES BUTTON
	width = 359;
	height = 90;
	x1 = 323;
	x2 = x1 + width;
	y1 = 545;
	y2 = y1 + height;
	state->highscores_button = newButton(x1, y1, x2, y2, WHITE);

	// EXIT BUTTON
	width = 169;
	height = 79;
	x1 = 417;
	x2 = x1 + width;
	y1 = 646;
	y2 = y1 + height;
	state->exit_button = newButton(x1, y1, x2, y2, WHITE);

	return state;

}

void drawMainMenuState(Battleship* battle) {
	drawBitmap(((MainMenuState *) battle->state)->background, 0, 0);

	// Checks which button is pressed and correspondingly draws a square around it

	if (((MainMenuState *) battle->state)->play_ai_button->mouse_hover == 1) {
		drawRectangle(287, 256, 435, 80, 2, WHITE);
	}

	else if (((MainMenuState *) battle->state)->play_pvp_button->mouse_hover
			== 1) {
		drawRectangle(212, 341, 583, 79, 2, WHITE);
	}

	else if (((MainMenuState *) battle->state)->exit_button->mouse_hover == 1) {
		drawRectangle(417, 646, 165, 75, 2, WHITE);
	}

	else if (((MainMenuState *) battle->state)->instructions_button->mouse_hover
			== 1) {
		drawRectangle(287, 449, 409, 79, 2, WHITE);
	}

	else if (((MainMenuState *) battle->state)->highscores_button->mouse_hover
			== 1) {
		drawRectangle(323, 545, 359, 90, 2, WHITE);
	}

}

State updateMainMenuState(Battleship* battle) {

	Mouse* rato = getMouse();

	//Checking each button to update the button "hover" status, the states's
	// "selected_button" status and the state's "done" status.

	// CHECK PLAY AI BUTTON
	if (mouseInsideButton(((MainMenuState *) battle->state)->play_ai_button, rato)) {

		((MainMenuState *) battle->state)->selected_button = play_ai;

		((MainMenuState *) battle->state)->play_ai_button->mouse_hover = 1;
		((MainMenuState *) battle->state)->play_pvp_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->instructions_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->highscores_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->exit_button->mouse_hover = 0;

		if (getMouse()->leftButtonDown) {
			((MainMenuState *) battle->state)->done = 1;
			return GAME_PLAY_SETSHIP_STATE;
		}

	}

	// CHECK PLAY PVP BUTTON
	else if (mouseInsideButton(((MainMenuState *) battle->state)->play_pvp_button, rato)) {
		((MainMenuState *) battle->state)->selected_button = play_pvp;

		((MainMenuState *) battle->state)->play_ai_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->play_pvp_button->mouse_hover = 1;
		((MainMenuState *) battle->state)->instructions_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->highscores_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->exit_button->mouse_hover = 0;

		if (getMouse()->leftButtonDown) {
			((MainMenuState *) battle->state)->done = 1;
		}
	}

	// CHECK INSTRUCTIONS BUTTON
	else if (mouseInsideButton(((MainMenuState *) battle->state)->instructions_button, rato)) {
		((MainMenuState *) battle->state)->selected_button = instructions;

		((MainMenuState *) battle->state)->play_ai_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->play_pvp_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->instructions_button->mouse_hover = 1;
		((MainMenuState *) battle->state)->highscores_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->exit_button->mouse_hover = 0;

		if (getMouse()->leftButtonDown) {
			((MainMenuState *) battle->state)->done = 1;
		}

	}

	// CHECK HIGHSCORE BUTTON
	else if (mouseInsideButton(((MainMenuState *) battle->state)->highscores_button, rato)) {
		((MainMenuState *) battle->state)->selected_button = highscores;

		((MainMenuState *) battle->state)->play_ai_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->play_pvp_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->instructions_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->highscores_button->mouse_hover = 1;
		((MainMenuState *) battle->state)->exit_button->mouse_hover = 0;

		if (getMouse()->leftButtonDown) {
			((MainMenuState *) battle->state)->done = 1;
		}

	}
	// CHECK EXIT BUTTON
	else if (mouseInsideButton(((MainMenuState *) battle->state)->exit_button, rato)) {
		((MainMenuState *) battle->state)->selected_button = exit_game;

		((MainMenuState *) battle->state)->play_ai_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->play_pvp_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->instructions_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->highscores_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->exit_button->mouse_hover = 1;

		if (getMouse()->leftButtonDown) {
			((MainMenuState *) battle->state)->done = 1;
		}
		return EXIT_STATE;
	}
	// IF NO BUTTON IS SELECTED
	else {
		((MainMenuState *) battle->state)->play_ai_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->play_pvp_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->instructions_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->highscores_button->mouse_hover = 0;
		((MainMenuState *) battle->state)->exit_button->mouse_hover = 0;
	}
}

void deleteMainMenuState(Battleship* battle) {
	disable_packets();
	free(battle->state);
	battle->state=NULL;
}
