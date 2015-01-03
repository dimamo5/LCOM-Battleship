#ifndef __MAINMENU_H_
#define __MAINMENU_H_

#include "button.h"
#include "battleship.h"
#include "bitmap.h"
/**
 * @defgroup game Game
 * @{
 * @brief Modulo as estruturas do Main Menu da state machine
 */
#define main_menu ((MainMenuState *) battle->state)

typedef enum {
	play_ai, instructions, highscores, exit_game
} currently_selected;
/**
 * @name Estrutura Estado Main Menu
 * @{
 * @brief Representa o estado do main menu
 */
typedef struct {
	int done; /**<@brief Flag de terminacao de estado */
	currently_selected selected_button; /**<@brief botao do menu principal selecionado de momento */
	unsigned int instruct; /**<@brief Flag que diz se estao a ser visualisadas as instrucoes ou nao*/
	Button* exit_button;/**<@brief Botao para sair do jogo */
	Button* play_ai_button; /**<@brief Botao para jogar */
	Button* instructions_button; /**<@brief Flag do botao de instrucoes */
	Button* highscores_button; /**<@brief Flag do botao dos highscores */
	Bitmap* background; /**<@brief Imagem de fundo do menu principal*/
	Bitmap* instructions; /**<@brief Imagem das instrucoes, para quando a variavel instruct e igual a 1 */
	// Existe uma funcao que verifica a posicao do rato e enche a variavel
} MainMenuState;
/**@}fim da estrutura Estado Main Menu*/

/**
 * Cria a estrutura do menu principal, inicializa as suas variaveis e retorna esta estrutura
 * @return retorna uma estrutura MainMenuState inicializada
 */
MainMenuState* newMainMenuState();
/**
 * Desenha o menu principal, e os quadrados a volta dos botoes, consoante a posicao do rato.
 * Desenha as instrucoes consoante a variavel instruct
 * @param battle estrutura principal do jogo
 */
void drawMainMenuState(Battleship* battle);
/**
 * Atualiza o menu principal, atualizando as posicoes do rato e outras variaveis,
 * desenhando consoante os seus valores, ou ate alterando o estado
 * @param battle estrutura principal do jogo
 * @return retorna o estado para o qual vai mudar. Retorna o proprio estado se nao mudar para nenhum
 */
State updateMainMenuState(Battleship* battle);
/**
 * Apaga a estrutura do menu principal, libertando memoria. Esta funcao e chamada quando se
 * pretende mudar de estado.
 * @param battle estrutura principal do jogo
 */
void deleteMainMenuState(Battleship* battle);

/**@}fim do modulo do MainMenu*/
#endif
