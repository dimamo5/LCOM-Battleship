#ifndef __GAME
#define __GAME

#include "battleship.h"
#include "button.h"
#include "bitmap.h"
/**
 * @defgroup game Game
 * @{
 * @brief Modulo com toda a game logic
 */
#define set_ship ((SetShipState*) battle->state)
#define game_state ((GameState*) battle->state)
#define TURN_TIME 10

typedef enum {
	HUMAN, COMPUTER
} type_player;

typedef enum {
	NOTHING, FIGHTER, DEATH_STAR, CRUSER, ESCAPE_SHUTTLE, BATTLESHIP
} type_ship;

typedef enum {
	WATER, FULL, SELECTED, UPPER_LEFT, UPPER_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, FIRST, SECOND, THIRD, FOURTH, FIFTH
} type_part;
/**
 * @name Estrutura Ship Part
 * @{
 * @brief Representa uma peca da nave
 */
typedef struct {
	type_part t_part;/**<@brief tipo de parte */
	type_ship t_ship;/**<@brief tipo de nave a que a peca pertence */
	char direction;/**<@brief orientacao da peca */
	unsigned short hit;/**<@brief Atingida ou nao */
} ship_part;
/**@}fim da estrutura ship*/
/**
 * @name Estrutura Ship
 * @{
 * @brief Representa uma nave
 */
typedef struct {
	type_ship t_ship; /**<@brief tipo de nave */
	ship_part* parts_array; /**<@brief array com todas as partes da nave */
	unsigned short nr_hits; /**<@brief nr. de hits que ja levou */
	unsigned short destroyed;/**<@brief Flag destroida ou nao */
	unsigned int x_central, y_central; /**<@brief cooredenadas da peca considerada central (normalmente 1º) */
	char direction;/**<@brief direcao da nave */
	unsigned int size;/**<@brief numero de pecas */
} ship;
/**@} fim da estrutura Ship */

/**
 * @name Estrutura Tabuleiro
 * @{
 * @brief Representa o tabuleiro de um jogador
 */
typedef struct {
	ship_part* tab_array[10][10]; /**<@brief array 2d com as diferentes pecas de cada nave*/
	unsigned int selected_x;/**<@brief quadricula selecionada em x*/
	unsigned int selected_y;/**<@brief quadricula selecionada em y */
	ship ship_array[7]; /**<@brief array com as naves */
	short ship_on_board;/**<@brief numero de naves já colocadas no tabuleiro*/
} tabuleiro;
/**<@}fim da estrutura Tabulerio*/

/**
 * @name Estrutura jogador
 * @{
 * @brief Representa toda informacao de um jogador, tabuleiro e variaveis para analise de scores
 */
typedef struct {
	type_player t_player;/**<@brief tipo de jogador humano ou computador*/
	tabuleiro tab;/**<@brief tabuleiro pertencente ao jogador */
	unsigned short shots_missed;/**<@brief numero de tiros falhados */
	unsigned short time_played;/**<@brief tempo total jogado por jogador  */
	unsigned short turns_missed;/**<@brief nr de turnos que o tempo acabou */
	unsigned short destroyed_ship_turn;/**<@brief destroiu uma ship nesse turno */
} player;
/**<@}fim da estrutura jogador*/

/**
 * @name Estrutura BotAi
 * @{
 * @brief Estrutura com informacao para o computador fazer a decisao onde jogar
 */
typedef struct {
	unsigned int last_x_hit;/**<@brief ultimo x escolhido */
	unsigned int last_y_hit;/**<@brief ultimo y escolhido */
	unsigned int previous_hit; /**<@brief ultimo tiro foi um ship ou "agua" */
	char direction;/**<@brief horizontal-'h' ou vertical-'v' */
	unsigned int orientation; /**<@brief direcao a seguir na proxima jogada (1- cima/0-aixo) ou (1-direita/0-esquerda) */
	unsigned int orientation_was_inverted;/**<@brief direcao foi invertida na jogada anterior */
} bot_ai;
/**<@}fim da estrutura BotAi*/

/**
 * @name Estrutura GamePlay
 * @{
 * @brief Estrutura Do estado GamePlay
 */
typedef struct {
	unsigned short turn_time_counter;/**<@brief contador de tempo restante do turno */
	unsigned short turn; /**<@brief turn 1 humano 0-computador  */
	unsigned short winner;/**<@brief vencedor 1 humano 0-computador*/
	player hum;/**<@brief jogador humano*/
	player com;/**<@brief jogador com*/
	bot_ai ai_comp;/**<@brief BotAi*/
	Bitmap* ship_map;/**<@brief Bitmap com imagnes da ship*/
	Bitmap* alarm_clock;/**<@brief Bitmap com numero para o tempo do turno*/
	Bitmap* ship_list;/**<@brief Bitmap com Lista de Ships*/
	Bitmap* turns;/**<@brief Bitmap com indentificacao de turno*/
	Bitmap* bmp_turns_missed;/**<@brief Bitmap com turnos falhados*/
	Bitmap* cross;/**<@brief Bitmap com cruz vermalha*/
	Bitmap* pause_screen;/**<@brief Bitmap com pause*/
	Bitmap* winner_bmp;/**<@brief Bitmap com a imagem winner*/
	Bitmap* loser_bmp;/**<@brief Bitmap com a imagem loser*/
	Bitmap* background;/**<@brief Bitmap com o background do jogo*/
	int done;/**<@brief Flag estado terminado*/
	unsigned int pause;/**<@brief Flag estado pausado*/
} GameState;
/**<@}fim da estrutura GameState*/

/**
 * @name Estrutura SetShipState
 * @{
 * @brief Estrutura Do estado SetShipState
 */
typedef struct {
	tabuleiro tab;/**<@brief Tabuleiro jogador a ser preenchido*/
	tabuleiro tab_com;/**<@brief Tabuleiro computador preenchido de formaa aleatoria*/
	ship* ship_temp;/**<@brief Nave a ser colocada no tabuleiro */
	Bitmap* ship_list;/**<@brief Bitmap com a lista de ships*/
	Bitmap* ship_map;/**<@brief Bitmap com as naves*/
	Bitmap* background;/**<@brief Bitmap background*/
	int done;/**<@brief Flag estado terminado*/
} SetShipState;
/**<@}fim da estrutura SetShipState*/

/**
 * Inicializa um jogo com o tabuleiro ja pronto
 * @return pointer para o estado
 */
GameState* newGame();

/**
 * Desenha o jogo
 * @param battle estrutura principal do jogo
 */
void drawGame(Battleship* battle);

/**
 * Update o jogo->Move as pecas no tabuleiro/Dispara/Muda de turnos/pause
 * @param battle estrutura principal do jogo
 */
State updateGame(Battleship* battle);

/**
 * Elimina  estado e liberta toda a memoria alocada
 * @param battle estrutura principal do jogo
 */
void deleteGame(Battleship* battle);

/**
 * Inicializa um jogo com o tabuleiro vazio e o do computador random
 * @return pointer para o estado
 */
SetShipState* newPlaySetship();
/**
 * Desenha o estado
 * @param battle estrutura principal do jogo
 */
void drawPlaySetship(Battleship* battle);
/**
 * Move a nave a ser colocada no tabuleiro/ introduz essa nave e passa para a proxima na lista
 * @param battle estrutura principal do jogo
 */
State updatePlaySetship(Battleship* battle);
/**
 *  Elimina  estado e liberta toda a memoria alocada
 *@param battle estrutura principal do jogo
 */
void deletePlaySetship(Battleship* battle);

/**
 * Inizializa todas a noves e coloca as no array respetivo do tabuleiro tab
 * @param tab tabuleiro onde colocar as pecas
 */
void initShip(tabuleiro* tab);

/**
 * Verifica se existem colisoes ou colocar uma ship s no tabuleiro tab
 * @param tab tabuleiro onde se coloca ship
 * @param s ship a ser colocada
 * @return 1 - colide 0 - nao colide
 */
int checkColission(tabuleiro tab, ship* s);
/**
 * Gera um tabuleiro aleatorio
 * @param tab tabuleiro vazio
 */
void randTabuleiro(tabuleiro* tab);

/**
 * Inicializa Player
 * @param t tipo de player a inizializar (HUM/COM)
 * @param state estado do jogo
 */
void initPlayer(type_player t, GameState* state);

/**
 * Pequena AI que decide onde o computador joga
 * @param battle estrutura principal do jogo
 */
void bot_play(Battleship* battle);
/**
 * Verifica se houve alguma ship destruida naquela jogada
 * @param battle estrutura principal do jogo
 * @return 1- destruido 2- nao destruido
 */
int checkShips(Battleship* battle);

/**
 * Verifica quem foi o vencedor do jogo
 * @param battle estrutura principal do jogo
 * @return ganhou 1-humano 2-computador 3-ninguem
 */
int gameOver(Battleship* battle); //winner 1-hum     2-comp     3-none

/**
 * Calcula o score do jogador no final do jogo
 * @param battle estrutura principal do jogo
 * @return pontuacao
 */
unsigned int calculaScore(Battleship* battle);

/**
 * Faz update à flag destroyed de cada shit consoalnte o nr de hits que sofreu
 * @param battle estrutura principal do jogo
 */
void updateShips(Battleship* battle);


/**@} end of Game*/

#endif
