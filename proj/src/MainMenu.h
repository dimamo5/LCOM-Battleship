#ifndef __MAINMENU_H_
#define __MAINMENU_H_

#include "button.h"
#include "battleship.h"
#include "bitmap.h"

typedef enum {
	play_ai, play_pvp, instructions, highscores, exit_game
} currently_selected;

typedef struct {
	int done;
	currently_selected selected_button;
	Button* exit_button; // A struct destes botoes possui a variavel "hover" que esta a 1 se
	Button* play_ai_button; // o rato estiver por cima do botao
	Button* play_pvp_button;
	Button* instructions_button;
	Button* highscores_button;
	Bitmap* background;
	// Existe uma funcao que verifica a posicao do rato e enche a variavel
} MainMenuState;

MainMenuState* newMainMenuState();
void drawMainMenuState(Battleship* battle);
State updateMainMenuState(Battleship* battle); // retorna se ha necessidade de draw ou nao
void deleteMainMenuState(Battleship* battle);

#endif
