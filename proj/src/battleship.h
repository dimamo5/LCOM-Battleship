#ifndef _BATTLESHIP_H_
#define _BATTLESHIP_H_

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdio.h>

/**
 * @defgroup StateMachine BattleShip
 * @{
 * State Machine do jogo
 */

#define PATH_IMG "/home/lcom/proj/img/"

typedef enum {
	MAIN_MENU_STATE, GAME_PLAY_SETSHIP_STATE, GAME_PLAY_STATE, HIGHSCORE_STATE, EXIT_STATE
} State; /**<@brief Estado Possiveis*/

/**
 * @name Estrutura battleship
 * @{
 * Estrutura Principal do Jogo (contemo toda a informcao do jogo)
 */

typedef struct {

	int IRQ_SET_KEYBOARD; /**<@brief IRQ SET Keyboard*/
	int IRQ_SET_MOUSE;/**<@brief IRQ SET Mouse*/
	int IRQ_SET_TIMER;/**<@brief IRQ SET Timer*/

	unsigned long kb_code;/**<@brief codigo retornado pelo Keyboard*/
	int done;/**<@brief Flag Jogo Terminado*/

	int timer_cnt;/**<@brief Contador de tick do timer */

	int highscore_winner;/**<@brief highscore do winner (inicio =-1) */

	int timer_ticked; /**<@brief Tick consoante os fps estipulados*/
	State currentState; /**<@brief Indentificador do estado actual*/
	void* state;/**<@brief Apontador generico para a estrutura do mouse*/

} Battleship;
/**@} fim da estrutual battleship*/

/**
 * Inizializa a Battleship
 * @return objecto battleship
 */
Battleship* startBattleship();
/**
 * Atendimento de Interrupcoes/Actualiza flags e contadores
 * @param battleship Estrutura principal do jogo
 */
void updateBattleship(Battleship* battleship);
/**
 * Chama a funcao draw de cada estado e actualiza os buffers
 * @param battleship Estrutura principal do jogo
 */
void drawBattleship(Battleship* battleship);
/**
 * Liberta memoria alocada pelo battleship e  desativa todas as interrupcoes
 *  @param battleship Estrutura principal do jogo
 */
void stopBattleship(Battleship* battleship);

/**
 * Muda de estado chama delete de cada estado e cria novo
 *  @param programState Estado a mudar
 *  @param battleship Estrutura principal do jogo
 */
void changeState(Battleship* battleship, State programState);
/**
 * Chama a funcao correspondente ao update em cada estado
 * @param battleship Estrutura principal do jogo
 */
void updateCurrentState(Battleship* battleship);
/**
 * Chama a funcao correspondente ao delete em cada estado
 *  @param battleship Estrutura principal do jogo
 */
void deleteCurrentState(Battleship* battleship);
/**@}fim do modulo Battleship*/

#endif
