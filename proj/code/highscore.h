#ifndef _HIGHSCORE_H_
#define _HIGHSCORE_H_
/**
 * @defgroup highscore Highscore
 * @{
 * @brief Funcoes de gestao e leitura/escrita de recordes
 */
#include "graphics.h"
#include "battleship.h"
#include "game.h"
#include "bitmap.h"
#include "keyboard_mouse.h"

#define highscore ((Highscore_State*) battle->state)
/**
 * @name Estrutura Jogador
 * Contem informacao referente a um jogador que entrou na tabela de highscores
 */
typedef struct {
	char nome[10]; /**<@brief Nome do Jogador */
	char data[20]; /**<@brief Data do pontuacao do Jogador */
	int score; /**<@brief Pontuação feita */
} Jogador;
/**@} end of Estrutura Jogador */

/**
 * @name Estrutura Highscore
 * Contem a informacao dos highscores e tambem do estado
 */
typedef struct {
	unsigned int year;/**<@brief ano actual(ledo pelo rtc)*/
	unsigned int month;/**<@brief mes actual(ledo pelo rtc)*/
	unsigned int day;/**<@brief dia actual(ledo pelo rtc)*/
	unsigned int hour;/**<@brief hour actual(ledo pelo rtc)*/
	unsigned int min;/**<@brief min actual(ledo pelo rtc)*/

	Jogador jogador_array[5];/**<@brief array ordenado (decrescente) de scores*/

	unsigned short show; /**<@brief Flag para mostrar a tabela ou escrever o nome*/

	char nome_player[10];/**<@brief nome do jogador que entrou para o highscore*/

	unsigned short score_player; /**<@brief score que o jogador vez*/
	Bitmap* intro_nome; /**<@brief Bitmap do background quando um jogador entra na tabela*/
	Bitmap* show_scores; /**<@brief Bitmap do background mostrar tabela*/

	unsigned short done; /**<@brief Flag estado terminado*/

	Bitmap* fonts; /**<@brief Bitmap da fonte utilizada neste modulo*/

} Highscore_State;

/**@} end of Estutura Highscore */

/**
 * Inicializa e aloca memoria pra o novo estado
 * @param battle estrutura principal jogo
 * @return retorna pointer para o novo estado
 */
Highscore_State* newHighscore(Battleship* battle);
/**
 * Chama funcoes para desenhar no ecra o estado
 * @param battle estrutura principal jogo
 */
void drawHighscore(Battleship* battle);
/**
 * Actualiza o estado fazendo verificacoes das teclas premidas
 * @param battle estrutura principal do jogo
 */
State updateHighscore(Battleship* battle);
/**
 * Elimina e liberta memoria consomida pelo estado
 * @param battle estrutura principal do jogo
 */
void deleteHighscore(Battleship* battle);
/**
 * Mapeamendo das teclas primidas
 * @param battle estrutura principal do jogo
 * @return char correspondente à tecla primida
 */
char getKey(Battleship* battle);
/**
 * Carrega o array de jogadores do estado a partir de um ficheiro txt
 * @param state estado highscore
 */
void loadScores(Highscore_State* state);
/**
 * Concatenacao da string str1 com um char
 * @param str1 string a ser concatenado com o char
 * @param ch char a concatenar
 */
void string_conc(char* str1, char ch);
/**
 * Obtem uma string com base no "objecto" Jogador
 * @param battle estrutura principal do jogo
 * @param i posicao no vector de jogadores
 * @param temp string retornada pela funcao
 */
void getStringJogador(Battleship* battle, unsigned int i, char * temp);
/**
 * Verifica se o score feito pelo jogar entra na tabela de highscores
 * @param battle estrutura principal do jogo
 * @return 1 caso tenho entrado 0 contrario
 */
int entrouHighscore(Battleship* battle);
/**
 * Transforma uma data numa string
 * @param battle estrutura principal do jogo
 * @param temp string a ser preenchida pela data
 */
void dataToString(Battleship* battle, char *temp);
/**
 * Algoritmo de ordenacao O(n²)
 * @param jogadores array dos jogadores a ordenar
 * @param tam tamanho do array
 */
void insertionSort(Jogador jogadores[], int tam);
/**
 * Guarda os scores num ficheiro
 * @param battle estrutura principal do jogo
 */
void saveScores(Battleship* battle);

/**@}Fim do Modulo Highscore*/

#endif
