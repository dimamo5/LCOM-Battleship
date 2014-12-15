#ifndef __MAINMENU
#define __MAINMENU

#include "button.h"

typedef struct{
	int done;
	Button* exit_button; // A struct destes botoes possui a variavel "hover" que esta a 1 se
	Button* play_button; // o rato estiver por cima do botao
	                     // Existe uma funcao que verifica a posicao do rato e enche a variavel
} MainMenuState;

MainMenuState* newMainMenuState(Battleship* battle);
void drawMainMenuState(Battleship* battle);
int updateMainMenuState(Battleship* battle); // retorna se ha necessidade de draw ou nao
void deleteMainMenuState(Battleship* battle);
