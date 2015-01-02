#include "MainMenu.h"
#include "graphics.h"
#include "bitmap.h"
#include "keyboard_mouse.h"

MainMenuState* newMainMenuState() {

	enable_packets();

	//Criacao do estado e inicializacao das suas variaveis e botoes

	MainMenuState* state = (MainMenuState*) malloc(sizeof(MainMenuState));
	state->background = loadBitmap("home/lcom/proj/img/mainmenu.bmp");
	state->instructions = loadBitmap("home/lcom/proj/img/regras.bmp");
	state->instruct = 0;
	state->done = 0;

	// PLAY AI BUTTON
	int x1 = 383;
	int x2 = 612;
	int y1 = 310;
	int y2 = 387;
	state->play_ai_button = newButton(x1, y1, x2, y2, WHITE);

	// WHITE - this is the button color, but it might be useless here
	// since drawRectange is where we choose the border color

	// HELP BUTTON
	x1 = 384;
	x2 = 612;
	y1 = 516;
	y2 = 599;
	state->instructions_button = newButton(x1, y1, x2, y2, WHITE);

	// HIGHSCORES BUTTON

	x1 = 328;
	x2 = 644;
	y1 = 418;
	y2 = 486;
	state->highscores_button = newButton(x1, y1, x2, y2, WHITE);

	// EXIT BUTTON
	x1 = 416;
	x2 = 575;
	y1 = 622;
	y2 = 685;
	state->exit_button = newButton(x1, y1, x2, y2, WHITE);

	resetMouse();

	return state;

}

void drawMainMenuState(Battleship* battle) {
	drawBitmap(((MainMenuState *) battle->state)->background, 0, 0);

	// Checks which button is pressed and correspondingly draws a square around it
	if (main_menu->instruct) {
		aloca_pixmap(0, 0, main_menu->instructions->Data, main_menu->instructions->bitmapInfo.width,
				main_menu->instructions->bitmapInfo.height);
	} else {
		if (((MainMenuState *) battle->state)->play_ai_button->mouse_hover == 1) {
			drawRectangle(((MainMenuState *) battle->state)->play_ai_button);
		}

		else if (((MainMenuState *) battle->state)->exit_button->mouse_hover == 1) {
			drawRectangle(((MainMenuState *) battle->state)->exit_button);
		}

		else if (((MainMenuState *) battle->state)->instructions_button->mouse_hover == 1) {
			drawRectangle(((MainMenuState *) battle->state)->instructions_button);
		}

		else if (((MainMenuState *) battle->state)->highscores_button->mouse_hover == 1) {
			drawRectangle(((MainMenuState *) battle->state)->highscores_button);
		}
	}
}

State updateMainMenuState(Battleship* battle) {

	Mouse* rato = getMouse();

	// Checks if the user presses ESC while reading the instructions or while not
	if (main_menu->instruct) {
		if ((battle->kb_code == KEY_ESC_BRK)) {
			printf("\n 1a condicao");
			main_menu->instruct = 0;
			battle->kb_code = KEY_NONE;
			return MAIN_MENU_STATE;
		}
	} else if (battle->kb_code == KEY_ESC_BRK) {
		printf("\n 2a condicao");
		battle->kb_code = KEY_NONE;
		((MainMenuState *) battle->state)->done = 1;
		return EXIT_STATE;
	}

	else {
		//Checking each button to update the button "hover" status, the states's
		// "selected_button" status and the state's "done" status.

		// CHECK PLAY AI BUTTON
		if (mouseInsideButton(((MainMenuState *) battle->state)->play_ai_button, rato)) {

			((MainMenuState *) battle->state)->selected_button = play_ai;

			((MainMenuState *) battle->state)->play_ai_button->mouse_hover = 1;
			((MainMenuState *) battle->state)->instructions_button->mouse_hover = 0;
			((MainMenuState *) battle->state)->highscores_button->mouse_hover = 0;
			((MainMenuState *) battle->state)->exit_button->mouse_hover = 0;

			if (getMouse()->leftButtonDown) {
				((MainMenuState *) battle->state)->done = 1;
				return GAME_PLAY_SETSHIP_STATE;
			}

		}

		// CHECK INSTRUCTIONS BUTTON
		else if (mouseInsideButton(((MainMenuState *) battle->state)->instructions_button, rato)) {
			((MainMenuState *) battle->state)->selected_button = instructions;

			((MainMenuState *) battle->state)->play_ai_button->mouse_hover = 0;
			((MainMenuState *) battle->state)->instructions_button->mouse_hover = 1;
			((MainMenuState *) battle->state)->highscores_button->mouse_hover = 0;
			((MainMenuState *) battle->state)->exit_button->mouse_hover = 0;

			if (getMouse()->leftButtonDown) {
				((MainMenuState *) battle->state)->instruct = 1;
				return MAIN_MENU_STATE;
			}

		}

		// CHECK HIGHSCORE BUTTON
		else if (mouseInsideButton(((MainMenuState *) battle->state)->highscores_button, rato)) {
			((MainMenuState *) battle->state)->selected_button = highscores;

			((MainMenuState *) battle->state)->play_ai_button->mouse_hover = 0;
			((MainMenuState *) battle->state)->instructions_button->mouse_hover = 0;
			((MainMenuState *) battle->state)->highscores_button->mouse_hover = 1;
			((MainMenuState *) battle->state)->exit_button->mouse_hover = 0;

			if (getMouse()->leftButtonDown) {
				((MainMenuState *) battle->state)->done = 1;
				return HIGHSCORE_STATE;
			}

		}
		// CHECK EXIT BUTTON
		else if (mouseInsideButton(((MainMenuState *) battle->state)->exit_button, rato)) {
			((MainMenuState *) battle->state)->selected_button = exit_game;

			((MainMenuState *) battle->state)->play_ai_button->mouse_hover = 0;
			((MainMenuState *) battle->state)->instructions_button->mouse_hover = 0;
			((MainMenuState *) battle->state)->highscores_button->mouse_hover = 0;
			((MainMenuState *) battle->state)->exit_button->mouse_hover = 1;

			if (getMouse()->leftButtonDown) {
				((MainMenuState *) battle->state)->done = 1;
				return EXIT_STATE;
			}

		}
		// IF NO BUTTON IS SELECTED
		else {
			((MainMenuState *) battle->state)->play_ai_button->mouse_hover = 0;
			((MainMenuState *) battle->state)->instructions_button->mouse_hover = 0;
			((MainMenuState *) battle->state)->highscores_button->mouse_hover = 0;
			((MainMenuState *) battle->state)->exit_button->mouse_hover = 0;
		}
	}
}
void deleteMainMenuState(Battleship* battle) {
	deleteBitmap(((MainMenuState *) battle->state)->background);
	disable_packets();
	deleteButton(((MainMenuState *) battle->state)->play_ai_button);
	deleteButton(((MainMenuState *) battle->state)->instructions_button);
	deleteButton(((MainMenuState *) battle->state)->highscores_button);
	deleteButton(((MainMenuState *) battle->state)->exit_button);
	free(battle->state);
	battle->state = NULL;
}
