#include "MainMenu.h"

MainMenuState* newMainMenuState(Battleship* battle) {

	//Criacao do estado e inicializacao das suas variaveis e botoes

	MainMenuState* state = (MainMenuState*) malloc(sizeof(MainMenuState));

	state->done = 0;

	// PLAY AI BUTTON
	int width = 435, height = 109;
	int x1 = 287;
	int x2 = x1 + width;
	int y1 = 256;
	int y2 = y1 + heigth;
	state->play_ai_button = Button * newButton(xi, yi, xf, yf, WHITE);
	// WHITE - this is the button color, but it might be useless here
	// since drawRectange is where we choose the border color

	// PLAY PVP BUTTON
	width = 583;
	height = 79;
	x1 = 212;
	x2 = x1 + width;
	y1 = 341;
	y2 = y1 + height;
	state->play_pvp_button = Button * newButton(xi, yi, xf, yf, WHITE);

	// INSTRUCTIONS BUTTON
	width = 409;
	height = 79;
	x1 = 287;
	x2 = x1 + width;
	y1 = 449;
	y2 = y1 + height;
	state->instructions_button = Button * newButton(xi, yi, xf, yf, WHITE);

	// HIGHSCORES BUTTON
	width = 359;
	height = 90;
	x1 = 323;
	x2 = x1 + width;
	y1 = 545;
	y2 = y1 + height;
	state->highscores_button = Button * newButton(xi, yi, xf, yf, WHITE);

	// EXIT BUTTON
	width = 169;
	height = 79;
	x1 = 417;
	x2 = x1 + width;
	y1 = 6646;
	y2 = y1 + height;
	state->exit_button = Button * newButton(xi, yi, xf, yf, WHITE);

	return state;

}

void drawMainMenuState(Battleship* battle) {
	Bitmap* background;
	background = loadBitmap("/home/lcom/proj/img/mainmenu.bmp");
	drawBitmap(background, 0, 0);

	// Checks which button is pressed and correspondingly draws a square around it

	if (battle->state->play_ai_button->mouse_hover == 1) {
		drawRectangle(287, 256, 435, 109, 20, WHITE);
	}

	else if (battle->state->play_pvp_button->mouse_hover == 1) {
		drawRectangle(212, 341, 583, 79, 20, WHITE);
	}

	else if (battle->state->exit_button->mouse_hover == 1) {
		drawRectangle(417, 646, 165, 75, 20, WHITE);
	}

	else if (battle->state->instructions_button->mouse_hover == 1) {
		drawRectangle(287, 449, 409, 79, 20, WHITE);
	}

	else if (battle->state->highscore_button->mouse_hover == 1) {
		drawRectangle(323, 545, 359, 90, 20, WHITE);
	}

}

int updateMainMenuState(Battleship* battle) {

	Mouse* rato = getMouse();


	//Checking each button to update the button "hover" status, the states's
	// "selected_button" status and the state's "done" status.

	// CHECK PLAY AI BUTTON
	if (mouseInsideButton(battle->state->play_ai_Button,rato)) {

		battle->state->selected_button = play_ai;

		battle->state->play_ai_button->mouse_hover = 1;
		battle->state->play_pvp_button->mouse_hover = 0;
		battle->state->instructions_button->mouse_hover = 0;
		battle->state->highscore_button->mouse_hover = 0;
		battle->state->exit_button->mouse_hover = 0;

		if (getMouse()->leftButtonDown) {
			state->done = 1;
		}

	}

	// CHECK PLAY PVP BUTTON
	else if (mouseInsideButton(state->play_pvp_Button,rato)) {
		battle->state->selected_button = play_pvp;

		battle->state->play_ai_button->mouse_hover = 0;
		battle->state->play_pvp_button->mouse_hover = 1;
		battle->state->instructions_button->mouse_hover = 0;
		battle->state->highscore_button->mouse_hover = 0;
		battle->state->exit_button->mouse_hover = 0;

		if (getMouse()->leftButtonDown) {
			state->done = 1;
		}
	}

	// CHECK INSTRUCTIONS BUTTON
	else if (mouseInsideButton(state->instructionsButton, rato)) {
		battle->state->selected_button = instructions;

		battle->state->play_ai_button->mouse_hover = 0;
		battle->state->play_pvp_button->mouse_hover = 0;
		battle->state->instructions_button->mouse_hover = 1;
		battle->state->highscore_button->mouse_hover = 0;
		battle->state->exit_button->mouse_hover = 0;

		if (getMouse()->leftButtonDown) {
			state->done = 1;
		}

	}

	// CHECK HIGHSCORE BUTTON
	else if (mouseInsideButton(state->highscoreButton,rato)) {
		battle->state->selected_button = highscores;

		battle->state->play_ai_button->mouse_hover = 0;
		battle->state->play_pvp_button->mouse_hover = 0;
		battle->state->instructions_button->mouse_hover = 0;
		battle->state->highscore_button->mouse_hover = 1;
		battle->state->exit_button->mouse_hover = 0;

		if (getMouse()->leftButtonDown) {
					state->done = 1;
				}

	}
	// CHECK EXIT BUTTON
	else if (mouseInsideButton(state->exitButton, rato)) {
		battle->state->selected_button = exit;

		battle->state->play_ai_button->mouse_hover = 0;
		battle->state->play_pvp_button->mouse_hover = 0;
		battle->state->instructions_button->mouse_hover = 0;
		battle->state->highscore_button->mouse_hover = 0;
		battle->state->exit_button->mouse_hover = 1;

		if (getMouse()->leftButtonDown) {
					state->done = 1;
				}
	}
	// IF NO BUTTON IS SELECTED
	else {
		battle->state->play_ai_button->mouse_hover = 0;
		battle->state->play_pvp_button->mouse_hover = 0;
		battle->state->instructions_button->mouse_hover = 0;
		battle->state->highscore_button->mouse_hover = 0;
		battle->state->exit_button->mouse_hover = 0;
	}
}
