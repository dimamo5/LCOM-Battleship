#ifndef __MAINMENU_H_
#define __MAINMENU_H_

#include "button.h"

typedef struct{
	int done;
	Button* exit_button; // A struct destes botoes possui a variavel "hover" que esta a 1 se
	Button* play_ai_button; // o rato estiver por cima do botao
	Button* play_pvp_button;
	Button* instructions_button;
	Button* highscores_button;
	                     // Existe uma funcao que verifica a posicao do rato e enche a variavel
} MainMenuState;

////MainMenuState* newMainMenuState(Battleship* battle);
//void drawMainMenuState(Battleship* battle);
//int updateMainMenuState(Battleship* battle); // retorna se ha necessidade de draw ou nao
//void deleteMainMenuState(Battleship* battle);

#endif
